#include "PathFinder.h"
#include "Utility.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "AutomaticPathComponent.h"
#include "BoxCollisionComponent.h"
#include "RayCast.h"
#include <unordered_map>
#include <SFML/Graphics/VertexArray.hpp>

PathFinder* PathFinder::mPathFinder = nullptr;

PathFinder::PathFinder()
:mTileSize(0.f, 0.f),
mMapSize(0.f, 0.f)
{
}


PathFinder::~PathFinder()
{
}

PathFinder* PathFinder::getInstance()
{
	if (!mPathFinder)
		mPathFinder = new PathFinder();

	return mPathFinder;
}

void PathFinder::resetAllMapData()
{
	mNodes.clear();
	mMapSize = sf::Vector2f();
	mTileSize = sf::Vector2f();
}

sf::Vector2f PathFinder::coordToPosition(int x, int y) const
{
	return sf::Vector2f(x * mTileSize.x + mTileSize.x / 2,
		y * mTileSize.y + mTileSize.y / 2);
}

AStarNode* PathFinder::At(int x, int y) const
{
	if (x < 0 || y < 0 || x >= mMapSize.x || y >= mMapSize.y)
		return nullptr;


	int a = x + mMapSize.x * y;
	
	if (a >= mNodes.size())
		return nullptr;

	return mNodes[a].get();
}

AStarNode* PathFinder::At(const sf::Vector2i& coordinate) const
{
	return At(coordinate.x, coordinate.y);
}

AStarNode* PathFinder::AtScript(int x, int y) const
{
	return At(x, y);
}

void PathFinder::clearParent()
{
	for (AStarNode::Ptr& node : mNodes)
		node->parent = nullptr;
}

void PathFinder::initializeAStarNodes(const sf::Vector2f& tileSize,
	const sf::Vector2f& mapSize)
{
	mTileSize = tileSize;
	mMapSize = mapSize;
	for (int y = 0; y < mapSize.y; y++){
		for (int x = 0; x < mapSize.x; x++){
			addNode(coordToPosition(x, y));
		}
	}
}


void PathFinder::addNode(const sf::Vector2f& pos)
{
	AStarNode::Ptr node(new AStarNode);
	node->pos = pos;
	node->f = node->g = node->h = 0.f;
	node->parent = nullptr;
	node->adjacent.resize(0);
	node->tile = nullptr;
	node->isInsideQueue = false;
	
	mNodes.push_back(std::move(node));
}

void PathFinder::setAdjacentNode(const sf::Vector2f& tileSize)
{
	int mapX = static_cast<int>(mMapSize.x);
	int mapY = static_cast<int>(mMapSize.y);
	for (int x = 0; x < mapX; x++){
		for (int y = 0; y < mapY; y++){
			if (At(x - 1, y - 1))	At(x, y)->adjacent.push_back(At(x - 1, y - 1));
			if (At(x - 0, y - 1))	At(x, y)->adjacent.push_back(At(x - 0, y - 1));
			if (At(x + 1, y - 1))	At(x, y)->adjacent.push_back(At(x + 1, y - 1));
			if (At(x - 1, y - 0))	At(x, y)->adjacent.push_back(At(x - 1, y - 0));
			if (At(x + 1, y - 0))	At(x, y)->adjacent.push_back(At(x + 1, y - 0));
			if (At(x - 1, y + 1))	At(x, y)->adjacent.push_back(At(x - 1, y + 1));
			if (At(x - 0, y + 1))	At(x, y)->adjacent.push_back(At(x - 0, y + 1));
			if (At(x + 1, y + 1))	At(x, y)->adjacent.push_back(At(x + 1, y + 1));
		}
	}
}


AStarNode* PathFinder::sceneToGraph(Entity *entity)
{
	if (!entity->hasComp<TransformableComponent>())
		return nullptr;

	return sceneToGraph(entity->comp<TransformableComponent>()->getWorldPosition());
}

AStarNode* PathFinder::sceneToGraph(const sf::Vector2f& pos)
{
	return sceneToGraph(pos.x, pos.y);
}

AStarNode* PathFinder::sceneToGraph(const float x, const float y)
{
	float xs = x / mTileSize.x;
	float ys = y / mTileSize.y;

	if (xs < 0.f || ys < 0.f)
		return nullptr;

	return At(static_cast<int>(xs), static_cast<int>(ys));
}

AStarNode* PathFinder::sceneToGraphScript(const float x, const float y)
{
	return sceneToGraph(x, y);
}

sf::Vector2i PathFinder::sceneToCoordinate(Entity *entity)
{
	if (!entity->hasComp<TransformableComponent>())
		return sf::Vector2i(-1, -1);

	return sceneToCoordinate(entity->comp<TransformableComponent>()->getWorldPosition());
}

sf::Vector2i PathFinder::sceneToCoordinate(const sf::Vector2f& pos)
{
	return sceneToCoordinate(pos.x, pos.y);
}

sf::Vector2i PathFinder::sceneToCoordinate(const float x, const float y)
{
	float xs = x / mTileSize.x;
	float ys = y / mTileSize.y;

	return sf::Vector2i(static_cast<int>(xs), static_cast<int>(ys));
}

sf::Vector2i PathFinder::sceneToCoordinateScript(const float x, const float y)
{
	return sceneToCoordinate(x, y);
}


AStarNode* PathFinder::moveSceneTo(Entity *entity, const sf::Vector2f& targetPos)
{
	if (!sceneToGraph(targetPos))
		return nullptr;
	
	if (!entity->hasComp<AutomaticPathComponent>())
		return nullptr;
	
	AutomaticPathComponent* automaticComponent = entity->comp<AutomaticPathComponent>();

	AStarNode *finalNode = nullptr;


	std::vector<MovingNode> path = constructPath(entity, targetPos, finalNode);
	automaticComponent->setAutomaticPaths(path, finalNode);

	return finalNode;
}



std::vector<MovingNode> PathFinder::constructPath(Entity *entity, const sf::Vector2f& targetPos,
	AStarNode*& reacheableNode)
{
	
	//sf::FloatRect entityRect = entity->getBoundingRect();
	if (!entity->hasComp<TransformableComponent>())
		return std::vector<MovingNode>();

	TransformableComponent* transformableComponent = entity->comp<TransformableComponent>();

	sf::Vector2f entityWorldPos = transformableComponent->getWorldPosition(true);
	sf::FloatRect entityRect = entity->comp<BoxCollisionComponent>()->mBoundingRect;
	/*if (entity->hasComp<BoxCollisionComponent>())
		finalPos = Utility::getMidOfRect(entity->comp<BoxCollisionComponent>()->getTransfromedRect());*/
	

	AStarNode *startNode = sceneToGraph(entityWorldPos);
	AStarNode *endNode = sceneToGraph(targetPos);


	if (!startNode || startNode == endNode){
		reacheableNode = endNode;
		return std::vector<MovingNode>();
	}

	AStarNode *currentNode = startNode;

	//both these two variables semantic is as follow
	//costSoFar[current] = 'current' cost so far is, float var
	//cameFrom[past] = 'past' came from, AStarNode* pointer
	//isInsideQueue[now] = 'now' is inside the queue ? bool var
	std::unordered_map<AStarNode*, float> costSoFar;
	std::unordered_map<AStarNode*, AStarNode*> cameFrom;
	std::unordered_map<AStarNode*, bool> isInsideQueue;
		
	

	//closed set is the one which all value has been calculated
	//and currentNode has already traversed, parent cannot be changed
	//open set is the one which all value has been calculated
	//however currentNode hasn't traversed it yet so the parent can still be changed
	//std::unordered_map<AStarNode*, bool> closedSet;
	std::vector<AStarNode*> closedSet;

	std::priority_queue< std::pair<AStarNode*, float>, 
		std::vector<std::pair<AStarNode*, float> >, ComparePair> openSet;

	//openSetRev.insert(std::make_pair(0.f, currentNode));
	openSet.push(std::make_pair(currentNode, 0.f));
	costSoFar[currentNode] = 0.f;
	cameFrom[currentNode] = nullptr;
	isInsideQueue[currentNode] = true;

	std::vector<MovingNode> Path;

	float new_g;
	float new_f;
	
	//auto closedSetIter = closedSet.begin();
	std::vector<AStarNode*>::const_iterator closedSetIter;

	//this shit is only when an entity cannot reach the original, target node
	//and now will going to find the closest, trackable loc
	std::vector<std::pair<AStarNode*, float>> mFList;
	
	bool firstTime = true;

	while (!openSet.empty()){
		
		currentNode = openSet.top().first;
		float tempF = openSet.top().second;

		openSet.pop();
	

		isInsideQueue[currentNode] = false;
		
		closedSet.push_back(currentNode);
		if (currentNode != startNode)
			mFList.push_back(std::make_pair(currentNode, Utility::vectorLength(currentNode->pos - endNode->pos)));

		
		if (currentNode == endNode)
			break;

		sf::Vector2f originForRay = currentNode->pos;
		if (firstTime){
			firstTime = false;
			originForRay = entityWorldPos;
		}

		for (AStarNode* n : currentNode->adjacent){
		
			if (n->tile || isInsideQueue[n]  /*!isRectOverallNodeSafe(n->pos, entityRect)*/
				|| !RayCast::castRayLinesFromRect(originForRay, entityRect, n->pos, this)
					/*|| !isRectOverallNodeSafe(n->pos, entityRect)*/)
			{
				if (n == endNode){
					endNode = currentNode;
					break;
				}
				continue;
			}
			
			
			closedSetIter = getIter(closedSet, n);
			//closedSetIter = closedSet.find(n);
			new_g = costSoFar[currentNode] + Utility::vectorLength(currentNode->pos - n->pos);
			if (closedSetIter != closedSet.end()){
				if (new_g < costSoFar[n]){
					//mFList.erase(std::make_pair(n, costSoFar[n] + length(n->pos - endNode->pos)));

					closedSet.erase(closedSetIter);
					cameFrom[n] = currentNode;
					costSoFar[n] = new_g;
					new_f = new_g + Utility::vectorLength(n->pos - endNode->pos);
					openSet.push(std::make_pair(n, new_f));
					
					isInsideQueue[n] = true;
				}
				continue;
			}

			cameFrom[n] = currentNode;
			costSoFar[n] = new_g;
			new_f = new_g + Utility::vectorLength(n->pos - endNode->pos);
			openSet.push(std::make_pair(n, new_f));
						
			isInsideQueue[n] = true;
			
		}
		//another test in case the final node is reached
		//but it is not movable to that loc because
		//of certain circumstances like width/height too tall
		if (currentNode == endNode)
			break;
	}
	
	//FLIST algorithm began
	if (currentNode != endNode && !mFList.empty()){
		std::sort(mFList.begin(), mFList.end(), [&]
			(std::pair<AStarNode*, float>& a, 
			std::pair<AStarNode*, float>& b)
		{
			return a.second < b.second;
		});
		
		currentNode = mFList.front().first;
		endNode = currentNode;
	}
	
	reacheableNode = currentNode;

	if (currentNode == endNode){
		AStarNode *n = endNode;
		AStarNode *nextNode = nullptr;
		while (n != nullptr){

			MovingNode movingNode;
			movingNode.starNode = n;
			//movingNode.isExactPos = false;
			//movingNode.isRayedNode = false;
			movingNode.direction = sf::Vector2f(0.f, 0.f);


			if (nextNode){
				MovingNode& rNode = Path.back();

				if (nextNode->pos.x > n->pos.x)
					rNode.direction.x = 1.f;
				else if (nextNode->pos.x < n->pos.x)
					rNode.direction.x = -1.f;

				if (nextNode->pos.y > n->pos.y)
					rNode.direction.y = 1.f;
				else if (nextNode->pos.y < n->pos.y)
					rNode.direction.y = -1.f;
			}

			if (n != startNode)
				Path.push_back(movingNode);
			//if (n == startNode)
				//doFirstNodeJudgment(Path, entity);

			nextNode = n;
			n = cameFrom[n];
		}
	}
	return Path;
}

void PathFinder::getListOfNodesBasedOnBoundingRect(const sf::FloatRect& rect,
	std::vector<AStarNode*>& markedNodes)
{
	float startX = rect.left;
	float startY = rect.top;

	float totalWidth = rect.width - 1;
	float totalHeight = rect.height - 1;

	float incrementX = 0.f;
	float incrementY = 0.f;

	


	sf::Vector2f tileSize = getTileSize();
	bool totalWidthIsFinished = false;
	bool totalHeightIsFinished = false;
	//if there are still height to be checked
	while (totalHeight > 0){
		//we minus it first, indicating a new loop is running
		//if (!firstY)
		if (totalHeightIsFinished){
			//totalHeightIsFinished = true;
			totalHeight = 0.f;
		}




		//if there are still width to be checked
		while (totalWidth > 0){
			if (totalWidthIsFinished){
				//totalWidthIsFinished = true;
				totalWidth = 0.f;
			}
			
			//if (!firstX)
			totalWidth -= tileSize.x;


			//we calculate the next node
			AStarNode *newNode = sceneToGraph(sf::Vector2f(startX + incrementX, startY + incrementY));

			//we increases the increment to pursue our test further
			//assuming the next loops is actually there
			incrementX += tileSize.x;
			//but if our current to be tested width is alreayd less
			//then the standard tile size, we have to change the added value
			//accordingly
			if (totalWidth < 0.f && !totalWidthIsFinished){
				incrementX -= tileSize.x;
				incrementX += std::abs(tileSize.x - (std::abs(totalWidth)));
				totalWidth = incrementX;
				totalWidthIsFinished = true;
			}
			

			
			//if this node is already included before, stop them from duplicating
			if (std::find(markedNodes.begin(),
				markedNodes.end(), newNode) != markedNodes.end())
				continue;
			//newNode->tile = entity;
			markedNodes.push_back(newNode);


		}
		totalHeight -= tileSize.y;

		totalWidth = rect.width - 1.f;
		incrementX = 0.f;

		incrementY += tileSize.y;

		if (totalHeight < 0.f && !totalHeightIsFinished){
			incrementY -= tileSize.y;
			incrementY += std::abs(tileSize.y - std::abs(totalHeight));
			totalHeight = incrementY;
			totalHeightIsFinished = true;
		}

		totalWidthIsFinished = false;
	}
}

bool PathFinder::isRectOverallNodeSafe(const sf::Vector2f& entityPos,
	const sf::FloatRect& entityRect)
{
	sf::FloatRect newRect = entityRect;
	newRect.left = entityPos.x - entityRect.width / 2.f;
	newRect.top = entityPos.y - entityRect.height / 2.f;

	std::vector<AStarNode*> markedNodes;
	getListOfNodesBasedOnBoundingRect(newRect, markedNodes);

	for (auto node : markedNodes){
		if (!node || node->tile)
			return false;
	}
	return true;
		
}


bool PathFinder::hasNode(const std::vector<AStarNode*>& set, const AStarNode *val)
{
	return (std::find(set.begin(), set.end(), val)) != set.end();
}

std::vector<AStarNode*>::const_iterator PathFinder::
	getIter(const std::vector<AStarNode*>& set, const AStarNode *val)
{
	return (std::find(set.begin(), set.end(), val));
}


void PathFinder::deleteSpecificNode(std::vector<AStarNode*>& set, AStarNode* specificNode)
{
	auto iterator = std::find_if(set.begin(), set.end(), 
		[&](AStarNode* ptr) -> bool { return ptr == specificNode; });

	if (iterator == set.end())
		return;
	
	specificNode->parent = nullptr;
	specificNode->f = 0.f;
	specificNode->g = 0.f;
	specificNode->h = 0.f;
	set.erase(iterator);
}

void PathFinder::deleteSpecificNode(std::map<AStarNode*, int>& map, AStarNode* specificNode)
{
	auto iterator = map.find(specificNode);
	if (iterator == map.end())
		return;
	map.erase(iterator);
}

AStarNode* PathFinder::nodeWithLowestF(std::vector<AStarNode*>& set)
{
	int count = 0;
	int finalC = 0;
	AStarNode *pointer = nullptr;
	float lowestF = std::numeric_limits<float>::max();

	for (AStarNode* ptr : set){
		if (lowestF > ptr->f){
			pointer = ptr;
			lowestF = ptr->f;
			finalC = count;
		}
		count++;
	}
	set.erase(set.begin() + finalC);
	return pointer;
}

AStarNode* PathFinder::nodeWithLowestF(std::set<AStarNode*>& set)
{
	int count = 0;
	int finalC = 0;
	AStarNode *pointer = nullptr;
	float lowestF = std::numeric_limits<float>::max();

	for (AStarNode* ptr : set){
		if (lowestF > ptr->f){
			pointer = ptr;
			lowestF = ptr->f;
			finalC = count;
		}
		count++;
	}
	
	set.erase(pointer);
	return pointer;
}

void PathFinder::drawDebugGraph(sf::RenderWindow& window)
{
	sf::VertexArray vertex(sf::Lines);

	float xSize = mMapSize.x;
	float ySize = mMapSize.y;

	//draw horizontal line
	for (float i = 0.f; i < xSize; i += 1.f){
		vertex.append(sf::Vertex(sf::Vector2f(i * mTileSize.x, 0.f), sf::Color::Blue));
		vertex.append(sf::Vertex(sf::Vector2f(i * mTileSize.x, ySize * mTileSize.y), sf::Color::Blue));
	}

	//draw vertical line
	for (float i = 0.f; i < ySize; i += 1.f){
		vertex.append(sf::Vertex(sf::Vector2f(0.f, i * mTileSize.y), sf::Color::Blue));
		vertex.append(sf::Vertex(sf::Vector2f(xSize * mTileSize.x, i * mTileSize.y), sf::Color::Blue));
	}
	window.draw(vertex);
}