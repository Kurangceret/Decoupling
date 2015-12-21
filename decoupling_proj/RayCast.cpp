#include "RayCast.h"
#include "Utility.h"
#include "PathFinder.h"
#include <algorithm>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

/*if (!designatedNode)
falseFlag = true;

if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
designatedNode) != toBeAvoidedNodes.end())
{
falseFlag = true;
if (contactedAvoidedNodes)
*contactedAvoidedNodes = true;
}

if (designatedNode && designatedNode->tile)
collidedTileList.push_back(designatedNode->tile);*/
RayCast::RayCast()
{
}


RayCast::~RayCast()
{
}

RayCast::TileChecker RayCast::mStandardTileChecker = [](AStarNode* designatedNode) -> bool{
	if (!designatedNode || designatedNode->tile || designatedNode->isFallable)
		return false;
	return true;
};

bool RayCast::castRayLinesFromRect(const sf::Vector2f& initialMidRectPos,
	const sf::FloatRect& rect, const sf::Vector2f& targetPos,
	PathFinder* pathFinder, const TileChecker& tileChecker)
{
	sf::Vector2f direction = Utility::unitVector(targetPos - initialMidRectPos);

	if (direction == sf::Vector2f())
		return true;

	sf::Vector2f tileSize = pathFinder->getTileSize();
	std::vector<sf::Vector2f> otherRayPoints;

	const sf::Vector2f topLeft = initialMidRectPos +
		sf::Vector2f((-rect.width / 2.f), (-rect.height / 2.f));
	const sf::Vector2f topRight = initialMidRectPos +
		sf::Vector2f((rect.width / 2.f), (-rect.height / 2.f));
	const sf::Vector2f bottomRight = initialMidRectPos +
		sf::Vector2f((rect.width / 2.f), (rect.height / 2.f) - 1.f);
	const sf::Vector2f bottomLeft = initialMidRectPos +
		sf::Vector2f((-rect.width / 2.f), (rect.height / 2.f) - 1.f);

	otherRayPoints.push_back(topLeft);
	otherRayPoints.push_back(topRight);
	otherRayPoints.push_back(bottomRight);
	otherRayPoints.push_back(bottomLeft);

	int widthRemainder = static_cast<int>(std::floor(((rect.width - 1.f) / tileSize.x)));
	int heightRemainder = static_cast<int>(std::floor(((rect.height - 1.f) / tileSize.y)));


	/*
	|---------------|
	|				|
	|				|
	|---------------|

	----------> Width
	|
	|
	|
	V
	Height

	is the indicator

	if direction going down or up, the width is the one to be ray cast
	if direction going left or right, the height is the one to be ray cast
	*/

	if (direction.x != 0.f && heightRemainder > 0){
		sf::Vector2f pointToBeUsed = direction.x > 0.f ? topRight : topLeft;
		float addY = rect.height / (heightRemainder + 1);

		int i = 1;
		while (i <= heightRemainder)
			otherRayPoints.push_back(pointToBeUsed + sf::Vector2f(0.f, addY * i++));

	}

	if (direction.y != 0.f && widthRemainder > 0){
		sf::Vector2f pointToBeUsed = direction.y > 0.f ? bottomLeft : topLeft;

		float addX = rect.width / (widthRemainder + 1);

		int i = 1;
		while (i <= widthRemainder)
			otherRayPoints.push_back(pointToBeUsed + sf::Vector2f(addX * i++, 0.f));
	}

	return castMultipleRayLines(initialMidRectPos, otherRayPoints,
		targetPos, pathFinder, tileChecker);
}

bool RayCast::castMultipleRayLines(const sf::Vector2f& initialPos,
	std::vector<sf::Vector2f>& otherPos,
	const sf::Vector2f& targetPos, PathFinder* pathFinder, const TileChecker& tileChecker)
{
	sf::Vector2f direction = Utility::unitVector(targetPos - initialPos);

	std::vector<sf::Vector2f> anotherPos = otherPos;
	if (direction.y != 0.f){
		if (!checkHorizontalTile(initialPos, targetPos, direction,
			otherPos, pathFinder, tileChecker))
			return false;
	}
	if (direction.x != 0.f){
		if (!checkVerticalTile(initialPos, targetPos, direction,
			anotherPos, pathFinder, tileChecker))
			return false;
	}
	return true;
}




bool RayCast::castRayLineScript(float initialX, float initialY,
	float targetX, float targetY, PathFinder* pathFinder, lua_State* luaState)
{
	
	luabridge::LuaRef luaTileChecker = luabridge::LuaRef::fromStack(luaState, 6);
	TileChecker tileChecker;

	if (!luaTileChecker.isNil()){
		tileChecker = [&](AStarNode* curNode) -> bool{
			return luaTileChecker(curNode);
		};
	}
	else
		tileChecker = mStandardTileChecker;
	

	return castRayLine(sf::Vector2f(initialX, initialY), 
		sf::Vector2f(targetX, targetY), pathFinder, tileChecker);
}



bool RayCast::castRayLine(const sf::Vector2f& initialPos, const sf::Vector2f& targetPos,
	PathFinder* pathFinder,
	const RayCast::TileChecker& nodeChecker)
{
	sf::Vector2f direction = Utility::unitVector(targetPos - initialPos);
	//std::vector<Entity*> tilesHorizontal, tilesVertical;
	std::vector<sf::Vector2f> otherPos, anotherPos;
	if (direction.y != 0.f){
		if (!checkHorizontalTile(initialPos, targetPos, direction,
			otherPos, pathFinder, nodeChecker))
			return false;
	}
	if (direction.x != 0.f){
		if (!checkVerticalTile(initialPos, targetPos, direction,
			anotherPos, pathFinder, nodeChecker))
			return false;
	}

	return true;
}

bool RayCast::checkHorizontalTile(const sf::Vector2f& initialPos,
	const sf::Vector2f& targetPos, const sf::Vector2f& direction,
	std::vector<sf::Vector2f>& otherPos,
	PathFinder* pathFinder, const RayCast::TileChecker& tileChecker)
{
	AStarNode *targetNode = pathFinder->sceneToGraph(targetPos);

	if (!targetNode)
		return false;

	sf::Vector2f tileSize = pathFinder->getTileSize();

	sf::Vector2f curPosition = initialPos;

	//here it is y / x, since we would like to know
	//how much distance has X travelled when Y reached the tileSizeHeight
	//float dirRadian = std::atan2(std::abs(direction.y), std::abs(direction.x));
	float dirRadian = std::atan2(direction.y, direction.x);
	float degree = Utility::toDegree(dirRadian);
	float anotherDegree = Utility::vectorToDegree(direction, false);

	float yA = tileSize.y;
	if (direction.y < 0.f)
		yA = -tileSize.y;

	float xA = tileSize.x / (std::tan(dirRadian));
	if ((direction.x > 0.f && xA < 0.f)
		|| (direction.x < 0.f && xA > 0.f))
		xA *= -1.f;

	//int index = -1;
	int index = 0;
	sf::Vector2f* indexPos = &curPosition;

	std::vector<sf::Vector2f> designatedPosList;


	sf::Vector2f diffToTarget = targetPos - initialPos;

	otherPos.push_back(initialPos);

	while (index < static_cast<int>(otherPos.size())){
		//if (index >= 0){
		indexPos = &otherPos[index];

		designatedPosList.push_back(*indexPos + diffToTarget);
		//}



		float newY = std::floor(indexPos->y / tileSize.y) * tileSize.y - 1.f;
		if (direction.y > 0.f){
			newY = std::floor(indexPos->y / tileSize.y) * tileSize.y + tileSize.y;

		}

		float additionalX = (std::abs(indexPos->y - newY)) / std::tan(dirRadian);

		if ((direction.x > 0.f && additionalX < 0.f)
			|| (direction.x < 0.f && additionalX > 0.f))
			additionalX *= -1.f;

		float newX = indexPos->x + additionalX;

		indexPos->x = newX;
		indexPos->y = newY;
		index++;
	}

	//designatedPosList.push_back(targetPos);

	while (true){



		bool falseFlag = false;

		int i = 0;
		for (auto iterPos = otherPos.begin(); iterPos != otherPos.end();){

			const sf::Vector2f& designatedPos = designatedPosList[i];

			if (
				(direction.x == 0.f || (direction.x > 0.f && iterPos->x >= designatedPos.x)
				|| (direction.x < 0.f && iterPos->x <= designatedPos.x))
				&&
				(direction.y == 0.f || (direction.y > 0.f && iterPos->y >= designatedPos.y)
				|| (direction.y < 0.f && iterPos->y <= designatedPos.y))
				)
			{
				AStarNode* designatedNode = pathFinder->sceneToGraph(designatedPos);

				/*if (!designatedNode)
					falseFlag = true;

				if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
					designatedNode) != toBeAvoidedNodes.end())
				{
					falseFlag = true;
					if (contactedAvoidedNodes)
						*contactedAvoidedNodes = true;
				}

				if (designatedNode && designatedNode->tile)
					collidedTileList.push_back(designatedNode->tile);*/
				if (!tileChecker(designatedNode))
					falseFlag = true;
								
				iterPos = otherPos.erase(iterPos);
				designatedPosList.erase(designatedPosList.begin() + i);
				continue;
				
			}

			AStarNode* node = pathFinder->sceneToGraph((*iterPos));
			if (!tileChecker(node))
				falseFlag = true;

			/*if (!node)
				falseFlag = true;

			if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
				node) != toBeAvoidedNodes.end())
			{
				falseFlag = true;
				if (contactedAvoidedNodes)
					*contactedAvoidedNodes = true;
			}

			if (node && node->tile)
				collidedTileList.push_back(node->tile);*/



			iterPos->x += xA;
			iterPos->y += yA;

			i++;
			iterPos++;
		}
		if (falseFlag /*|| !collidedTileList.empty()*/)
			return false;

		if (otherPos.empty())
			return true;

	}

	return true;
}

/*bool RayCast::checkHorizontalTile(const sf::Vector2f& initialPos,
	const sf::Vector2f& targetPos, const sf::Vector2f& direction,
	std::vector<sf::Vector2f>& otherPos,
	PathFinder* pathFinder,
	const std::vector<AStarNode*>& toBeAvoidedNodes,
	bool* contactedAvoidedNodes,
	std::vector<Entity*>& collidedTileList)
{
	AStarNode *targetNode = pathFinder->sceneToGraph(targetPos);

	if (!targetNode)
		return false;

	sf::Vector2f tileSize = pathFinder->getTileSize();
	
	sf::Vector2f curPosition = initialPos;

	//here it is y / x, since we would like to know
	//how much distance has X travelled when Y reached the tileSizeHeight
	//float dirRadian = std::atan2(std::abs(direction.y), std::abs(direction.x));
	float dirRadian = std::atan2(direction.y, direction.x);
	float degree = Utility::toDegree(dirRadian);
	float anotherDegree = Utility::vectorToDegree(direction, false);

	float yA = tileSize.y;
	if (direction.y < 0.f)
		yA = -tileSize.y;

	float xA = tileSize.x / (std::tan(dirRadian));
	if ((direction.x > 0.f && xA < 0.f)
		|| (direction.x < 0.f && xA > 0.f))
		xA *= -1.f;

	//int index = -1;
	int index = 0;
	sf::Vector2f* indexPos = &curPosition;

	std::vector<sf::Vector2f> designatedPosList;
	

	sf::Vector2f diffToTarget = targetPos - initialPos;
	
	otherPos.push_back(initialPos);

	while (index < static_cast<int>(otherPos.size())){
		//if (index >= 0){
			indexPos = &otherPos[index];

			designatedPosList.push_back(*indexPos + diffToTarget);
		//}

		

		float newY = std::floor(indexPos->y / tileSize.y) * tileSize.y - 1.f;
		if (direction.y > 0.f){
			newY = std::floor(indexPos->y / tileSize.y) * tileSize.y + tileSize.y;
			
		}

		float additionalX = (std::abs(indexPos->y - newY)) / std::tan(dirRadian);

		if ((direction.x > 0.f && additionalX < 0.f)
			|| (direction.x < 0.f && additionalX > 0.f))
			additionalX *= -1.f;

		float newX = indexPos->x + additionalX;

		indexPos->x = newX;
		indexPos->y = newY;
		index++;
	}

	//designatedPosList.push_back(targetPos);
		
	while (true){
		
		

		bool falseFlag = false;

		int i = 0;
		for (auto iterPos = otherPos.begin(); iterPos != otherPos.end();){

			const sf::Vector2f& designatedPos = designatedPosList[i];

			if	(
				(direction.x == 0.f || (direction.x > 0.f && iterPos->x >= designatedPos.x)
				|| (direction.x < 0.f && iterPos->x <= designatedPos.x))
				&&
				(direction.y == 0.f || (direction.y > 0.f && iterPos->y >= designatedPos.y)
				|| (direction.y < 0.f && iterPos->y <= designatedPos.y))
				)
			{
				AStarNode* designatedNode = pathFinder->sceneToGraph(designatedPos);

				if (!designatedNode)
					falseFlag = true;

				if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
					designatedNode) != toBeAvoidedNodes.end())
				{
					falseFlag = true;
					if (contactedAvoidedNodes)
						*contactedAvoidedNodes = true;
				}

				if (designatedNode && designatedNode->tile)
					collidedTileList.push_back(designatedNode->tile);

				//if everything is safe, we delete it from further calculation
				//if (!falseFlag && collidedTileList.empty()){
				iterPos = otherPos.erase(iterPos);
				designatedPosList.erase(designatedPosList.begin() + i);
				continue;
				//}
			}

			AStarNode* node = pathFinder->sceneToGraph((*iterPos));
			if (!node)
				falseFlag = true;

			if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
				node) != toBeAvoidedNodes.end())
			{
				falseFlag = true;
				if (contactedAvoidedNodes)
					*contactedAvoidedNodes = true;
			}

			if (node && node->tile)
				collidedTileList.push_back(node->tile);
			
			iterPos->x += xA;
			iterPos->y += yA;

			i++;
			iterPos++;
		}
		if (falseFlag || !collidedTileList.empty())
			return false;

		if (otherPos.empty())
			return true;
				
	}
	
	return true;
}*/

bool RayCast::checkVerticalTile(const sf::Vector2f& initialPos,
	const sf::Vector2f& targetPos, const sf::Vector2f& direction,
	std::vector<sf::Vector2f>& otherPos,
	PathFinder* pathFinder, const TileChecker& tileChecker)
{

	AStarNode *targetNode = pathFinder->sceneToGraph(targetPos);
	if (!targetNode)
		return false;

	sf::Vector2f tileSize = pathFinder->getTileSize();


	sf::Vector2f curPosition = initialPos;

	//float dirRadian = std::atan2(std::abs(direction.x), std::abs(direction.y));

	float dirRadian = std::atan2(direction.x, direction.y);
	float xA = tileSize.x;
	if (direction.x < 0.f)
		xA = -tileSize.x;

	float yA = tileSize.y / std::tan(dirRadian);

	if ((direction.y > 0.f && yA < 0.f)
		|| (direction.y < 0.f && yA > 0.f))
		yA *= -1.f;

	int index = 0;
	sf::Vector2f* indexPos = &curPosition;

	std::vector<sf::Vector2f> designatedPosList;


	sf::Vector2f diffToTarget = targetPos - initialPos;

	otherPos.push_back(initialPos);

	while (index < static_cast<int>(otherPos.size())){
		//if (index >= 0){
		indexPos = &otherPos[index];

		designatedPosList.push_back(*indexPos + diffToTarget);
		//}

		float newX = std::floor(indexPos->x / tileSize.x) * tileSize.x - 1.f;
		if (direction.x > 0.f){
			newX = std::floor(indexPos->x / tileSize.x) * tileSize.x + tileSize.x;
			//if (index < 0)
			//	newX--;
		}

		float additionalY = (std::abs(indexPos->x - newX)) / std::tan(dirRadian);

		if ((direction.y > 0.f && additionalY < 0.f)
			|| (direction.y < 0.f && additionalY > 0.f))
			additionalY *= -1.f;

		float newY = indexPos->y + additionalY;

		indexPos->x = newX;
		indexPos->y = newY;

		index++;
	}

	//horizontal line checking
	while (true){
		bool falseFlag = false;

		int i = 0;
		for (auto iterPos = otherPos.begin(); iterPos != otherPos.end();){

			const sf::Vector2f& designatedPos = designatedPosList[i];

			if (
				(direction.x == 0.f || (direction.x > 0.f && iterPos->x >= designatedPos.x)
				|| (direction.x < 0.f && iterPos->x <= designatedPos.x))
				&&
				(direction.y == 0.f || (direction.y > 0.f && iterPos->y >= designatedPos.y)
				|| (direction.y < 0.f && iterPos->y <= designatedPos.y))
				)
			{
				AStarNode* designatedNode = pathFinder->sceneToGraph(designatedPos);

				/*if (!designatedNode)
					falseFlag = true;

				if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
					designatedNode) != toBeAvoidedNodes.end())
				{
					falseFlag = true;
					if (contactedAvoidedNodes)
						*contactedAvoidedNodes = true;
				}

				if (designatedNode && designatedNode->tile)
					collidedTileList.push_back(designatedNode->tile);*/
				if (!tileChecker(designatedNode))
					falseFlag = true;

				//if everything is safe, we delete it from further calculation
				//if (!falseFlag && collidedTileList.empty()){
				iterPos = otherPos.erase(iterPos);
				designatedPosList.erase(designatedPosList.begin() + i);
				continue;
				//}
			}

			AStarNode* node = pathFinder->sceneToGraph((*iterPos));

			if (!tileChecker(node))
				falseFlag = true;

			iterPos->x += xA;
			iterPos->y += yA;

			i++;
			iterPos++;
		}
		if (falseFlag)
			return false;

		if (otherPos.empty())
			return true;

	}
	return true;
}


/*bool RayCast::checkVerticalTile(const sf::Vector2f& initialPos,
	const sf::Vector2f& targetPos, const sf::Vector2f& direction,
	std::vector<sf::Vector2f>& otherPos,
	PathFinder* pathFinder,
	const std::vector<AStarNode*>& toBeAvoidedNodes,
	bool* contactedAvoidedNodes,
	std::vector<Entity*>& collidedTileList)
{

	AStarNode *targetNode = pathFinder->sceneToGraph(targetPos);
	if (!targetNode)
		return false;

	sf::Vector2f tileSize = pathFinder->getTileSize();


	sf::Vector2f curPosition = initialPos;

	//float dirRadian = std::atan2(std::abs(direction.x), std::abs(direction.y));

	float dirRadian = std::atan2(direction.x, direction.y);
	float xA = tileSize.x;
	if (direction.x < 0.f)
		xA = -tileSize.x;

	float yA = tileSize.y / std::tan(dirRadian);

	if ((direction.y > 0.f && yA < 0.f)
		|| (direction.y < 0.f && yA > 0.f))
		yA *= -1.f;

	int index = 0;
	sf::Vector2f* indexPos = &curPosition;

	std::vector<sf::Vector2f> designatedPosList;


	sf::Vector2f diffToTarget = targetPos - initialPos;

	otherPos.push_back(initialPos);

	while (index < static_cast<int>(otherPos.size())){
		//if (index >= 0){
		indexPos = &otherPos[index];

		designatedPosList.push_back(*indexPos + diffToTarget);
		//}

		float newX = std::floor(indexPos->x / tileSize.x) * tileSize.x - 1.f;
		if (direction.x > 0.f){
			newX = std::floor(indexPos->x / tileSize.x) * tileSize.x + tileSize.x;
			//if (index < 0)
			//	newX--;
		}

		float additionalY = (std::abs(indexPos->x - newX)) / std::tan(dirRadian);

		if ((direction.y > 0.f && additionalY < 0.f)
			|| (direction.y < 0.f && additionalY > 0.f))
			additionalY *= -1.f;

		float newY = indexPos->y + additionalY;

		indexPos->x = newX;
		indexPos->y = newY;

		index++;
	}

	//horizontal line checking
	while (true){
		bool falseFlag = false;

		int i = 0;
		for (auto iterPos = otherPos.begin(); iterPos != otherPos.end();){

			const sf::Vector2f& designatedPos = designatedPosList[i];

			if (
				(direction.x == 0.f || (direction.x > 0.f && iterPos->x >= designatedPos.x)
				|| (direction.x < 0.f && iterPos->x <= designatedPos.x))
				&&
				(direction.y == 0.f || (direction.y > 0.f && iterPos->y >= designatedPos.y)
				|| (direction.y < 0.f && iterPos->y <= designatedPos.y))
				)
			{
				AStarNode* designatedNode = pathFinder->sceneToGraph(designatedPos);

				if (!designatedNode)
					falseFlag = true;

				if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
					designatedNode) != toBeAvoidedNodes.end())
				{
					falseFlag = true;
					if (contactedAvoidedNodes)
						*contactedAvoidedNodes = true;
				}

				if (designatedNode && designatedNode->tile)
					collidedTileList.push_back(designatedNode->tile);

				//if everything is safe, we delete it from further calculation
				//if (!falseFlag && collidedTileList.empty()){
				iterPos = otherPos.erase(iterPos);
				designatedPosList.erase(designatedPosList.begin() + i);
				continue;
				//}
			}

			AStarNode* node = pathFinder->sceneToGraph((*iterPos));
			if (!node)
				falseFlag = true;

			if (std::find(toBeAvoidedNodes.begin(), toBeAvoidedNodes.end(),
				node) != toBeAvoidedNodes.end())
			{
				falseFlag = true;
				if (contactedAvoidedNodes)
					*contactedAvoidedNodes = true;
			}

			if (node && node->tile)
				collidedTileList.push_back(node->tile);

			iterPos->x += xA;
			iterPos->y += yA;

			i++;
			iterPos++;
		}
		if (falseFlag || !collidedTileList.empty())
			return false;

		if (otherPos.empty())
			return true;

	}
	return true;
}*/