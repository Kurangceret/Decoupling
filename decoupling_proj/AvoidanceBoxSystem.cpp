#include "AvoidanceBoxSystem.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "Entity.h"
#include "AvoidanceBoxComponent.h"
#include "TransformableComponent.h"
#include "Utility.h"
#include "CategoryComponent.h"
#include "AutomaticPathComponent.h"
#include "SteeringBehavior.h"
#include "RayCast.h"
#include "PathFinder.h"

AvoidanceBoxSystem::AvoidanceBoxSystem()
{
	pushRequiredComponent(ComponentIdentifier::BoxCollisionComponent);
	pushRequiredComponent(ComponentIdentifier::AvoidanceBoxComponent);
	pushRequiredComponent(ComponentIdentifier::VelocityComponent);
	pushRequiredComponent(ComponentIdentifier::TransformableComponent);
}


AvoidanceBoxSystem::~AvoidanceBoxSystem()
{
}


void AvoidanceBoxSystem::processEntity(sf::Time dt, Entity* entity)
{
	BoxCollisionComponent* boxCollisionComp = entity->comp<BoxCollisionComponent>();
	VelocityComponent* veloComp = entity->comp<VelocityComponent>();
	AvoidanceBoxComponent* avoidBoxComp = entity->comp<AvoidanceBoxComponent>();
	if (!avoidBoxComp->currentlyCanBeRayCast())
		return;

	TransformableComponent* transformComp = entity->comp<TransformableComponent>();


	RotatedRect& entityRayRect = avoidBoxComp->getRayRect();
	sf::Vector2f entityV = veloComp->getVelocity();

	if (entityV == sf::Vector2f()){
		for (int i = 0; i < entityRayRect.PositionCount; i++)
			entityRayRect.mPoints[i] = sf::Vector2f();
		return;
	}

	sf::FloatRect entityRect = boxCollisionComp->mBoundingRect;
	sf::Vector2f entityWorldPos = transformComp->getWorldPosition(true);


	
	float rayLength = avoidBoxComp->getRayLength();
		
	float rayThickness = avoidBoxComp->getRayThickness();
	
	sf::Vector2f dirPlus = Utility::degreeToVector(Utility::vectorToDegree(entityV, false) + 90.f);
	sf::Vector2f dirMinus = Utility::degreeToVector(Utility::vectorToDegree(entityV, false) - 90.f);

	sf::Vector2f leftNavelPoint = (dirPlus * rayThickness) + entityWorldPos;
	sf::Vector2f rightNavelPoint = (dirMinus * rayThickness) + entityWorldPos;

	sf::Vector2f tipRightPoint, tipLeftPoint;

	tipLeftPoint = (entityV * rayLength) + leftNavelPoint;
	tipRightPoint = (entityV * rayLength) + rightNavelPoint;
	
	entityRayRect.mPoints[RotatedRect::TopLeft] = leftNavelPoint;
	entityRayRect.mPoints[RotatedRect::BottomLeft] = rightNavelPoint;
	entityRayRect.mPoints[RotatedRect::TopRight] = tipLeftPoint;
	entityRayRect.mPoints[RotatedRect::BottomRight] = tipRightPoint;

}


/*void AvoidanceBoxSystem::checkForStaticTile(Entity* entity, std::vector<Entity*>& staticTile)
{
	AutomaticPathComponent* automaticPath = entity->comp<AutomaticPathComponent>();
	TransformableComponent* transformComp = entity->comp<TransformableComponent>();
	BoxCollisionComponent* boxCollisionComp = entity->comp<BoxCollisionComponent>();

	sf::Vector2f entityWorldPos = transformComp->getWorldPosition(true);

	AutomaticPathComponent::AutomaticPathList& pathList = automaticPath->getAutomaticPaths();

	RayCast::castRayLinesFromRect(entityWorldPos, boxCollisionComp->getTransfromedRect(), 
		pathList.back().starNode->pos,
		PathFinder::getInstance(),
		std::vector<AStarNode*>(),
		nullptr, staticTile);

}*/


bool AvoidanceBoxSystem::checkForPathWill(const sf::Vector2f& entityWorldPos, 
	AutomaticPathComponent* autoPathComp,
	BoxCollisionComponent* boxColComp,
	BoxCollisionComponent* boxColCompToBeAvoided)
{
	if (autoPathComp->isAutomaticPathsEmpty())
		return true;

	AStarNode* curDestNode = autoPathComp->getAutomaticPaths().back().starNode;
	std::vector<AStarNode*> nodesToBeAvoided;
	sf::FloatRect transformedEntityRect = boxColComp->getTransfromedRect();

	PathFinder::getInstance()->getListOfNodesBasedOnBoundingRect(
		boxColCompToBeAvoided->getTransfromedRect(), nodesToBeAvoided);

	bool isSafe = true;

	RayCast::TileChecker tileChecker = 
		[&nodesToBeAvoided, &isSafe](AStarNode* node) -> bool
	{
		if (!RayCast::mStandardTileChecker(node)){
			isSafe = false;
			return false;
		}

		if (std::find(nodesToBeAvoided.begin(), nodesToBeAvoided.end(),
			node) != nodesToBeAvoided.end())
		{
			isSafe = false;
			return false;
		}
		return true;
	};


	/*RayCast::castRayLinesFromRect(entityWorldPos, transformedEntityRect,
		curDestNode->pos, PathFinder::getInstance(), nodesToBeAvoided, &isContacted);*/

	return RayCast::castRayLinesFromRect(entityWorldPos, transformedEntityRect,
		curDestNode->pos, PathFinder::getInstance(), tileChecker);

	//return !isSafe;
}

void AvoidanceBoxSystem::performUnalignedAvoidance(Entity* entity, 
	std::vector<Entity*>& toBeCheckedList, sf::Time dt)
{
	if (!checkForEntity(entity))
		return;

	BoxCollisionComponent* boxCollisionComp = entity->comp<BoxCollisionComponent>();
	VelocityComponent* veloComp = entity->comp<VelocityComponent>();
	AvoidanceBoxComponent* avoidBoxComp = entity->comp<AvoidanceBoxComponent>();

	avoidBoxComp->setJustGotRayed(false);
	if (!avoidBoxComp->currentlyCanBeRayCast())
		return;

	TransformableComponent* transformComp = entity->comp<TransformableComponent>();

	AutomaticPathComponent* automaticPathComp = entity->nonCreateComp<AutomaticPathComponent>();

	RotatedRect& entityRayRect = avoidBoxComp->getRayRect();

	if (entityRayRect.rectIsAllZeroes())
		return;

	sf::Vector2f entityWorldPos = transformComp->getWorldPosition(true);
	sf::Vector2f entityRayPos = entityWorldPos + (veloComp->getVelocity() * avoidBoxComp->getRayLength());
	//sf::Vector2f entityRayPos = entityWorldPos;

	std::vector<CollidedAvoidBoxEntityData> listOfCollided;
	listOfCollided.clear();

	std::vector<Entity*> staticTile;
	sf::Vector2f entityV = veloComp->getVelocity();

	if (automaticPathComp && !automaticPathComp->isAutomaticPathsEmpty()){
		AutomaticPathComponent::AutomaticPathList& pathList = automaticPathComp->getAutomaticPaths();

		AStarNode* nextNode = pathList.back().starNode;

		sf::Vector2f nextDir = Utility::unitVector(nextNode->pos - entityWorldPos);

		bool isOnCorrectVelo = true;
		//if (entityV.x * nextDir.x < 0.f) isOnCorrectVelo = false;
		//if (entityV.y * nextDir.y < 0.f) isOnCorrectVelo = false;
		if (Utility::getDotProduct(entityV, nextDir) < 0.f) isOnCorrectVelo = false;

		RayCast::TileChecker tileChecker =
			[&staticTile](AStarNode* node) -> bool
		{
			bool flag = true;
			if (!RayCast::mStandardTileChecker(node))
				flag = false;
			

			if (node && node->tile)
			{
				flag = false;
				staticTile.push_back(node->tile);
			}
			return flag;
		};

		if (isOnCorrectVelo)
			RayCast::castRayLinesFromRect(entityWorldPos, boxCollisionComp->getTransfromedRect(),
				pathList.back().starNode->pos,
				PathFinder::getInstance(), tileChecker);
	}
	
	

	for (auto testEntity : toBeCheckedList){
		if (testEntity == entity)
			continue;
		if (testEntity->comp<CategoryComponent>()->getCategory() & Category::Player)
			continue;

		VelocityComponent* testVeloComp = testEntity->nonCreateComp<VelocityComponent>();
		if (!testVeloComp &&	std::find(staticTile.begin(),
			staticTile.end(), testEntity) == staticTile.end())
			continue;
		

		AvoidanceBoxComponent* checkingBox = testEntity->nonCreateComp<AvoidanceBoxComponent>();

		sf::Vector2f testEntityRay = testEntity->comp<TransformableComponent>()->getWorldPosition(true);

		float dotProd = 0.f;
		float velocityDotProd = -1.f;

		if (testVeloComp && checkingBox)
			testEntityRay += testEntity->comp<VelocityComponent>()->getVelocity() * checkingBox->getRayLength();

		if (testVeloComp){
			velocityDotProd = Utility::getDotProduct(testVeloComp->getVelocity(), veloComp->getVelocity());
			//velocityDotProd = std::floor(velocityDotProd);
		}
		sf::Vector2f diff;
		//float parallelTest = 0.707f;
		float parallelTest = 0.00f;
		//paraller line
		if (velocityDotProd > parallelTest && checkingBox){
			//testEntityRay += testEntity->comp<VelocityComponent>()->getVelocity() * checkingBox->getRayLength();
			diff = testEntityRay - entityRayPos;
			//diff = testEntityRay - entityWorldPos;

			dotProd = Utility::getDotProduct(diff, entityV);
		}
		else{
			diff = testEntityRay - entityWorldPos;

			dotProd = Utility::getDotProduct(diff, entityV);
			if (dotProd <= 0.f && testVeloComp){
				diff = testEntity->comp<TransformableComponent>()->getWorldPosition(true) -
					entityWorldPos;

				dotProd = Utility::getDotProduct(diff, entityV);
			}
		}

		/*if (testVeloComp && checkingBox)
			testEntityRay += testEntity->comp<VelocityComponent>()->getVelocity() * checkingBox->getRayLength();
		
		sf::Vector2f diff = testEntityRay - entityRayPos;
	
		float dotProd = Utility::getDotProduct(diff, entityV);
	
		
		//if checking testEntity's ray box failed, check its world position
		if (dotProd <= 0.f && checkingBox && testVeloComp
			&& testEntity->hasComp<TransformableComponent>())
		{
			dotProd = Utility::getDotProduct(testEntity->comp<TransformableComponent>()->getWorldPosition(true) -
				entityRayPos, entityV);
		}*/
		
		

		if (dotProd <= 0.f)
			continue;
				
		bool useNormalRect = checkingBox == nullptr;
		BoxCollisionComponent* checkingCol = testEntity->comp<BoxCollisionComponent>();
		
		RotatedRect* checkingRayRect = nullptr;
		if (checkingBox)
			checkingRayRect = &checkingBox->getRayRect();

		if (checkingRayRect)
			useNormalRect = checkingRayRect->rectIsAllZeroes();

		if (useNormalRect){
			
			RotatedRect newRay(checkingCol->getTransfromedRect());

			bool rotatedCol = Utility::rotatedCollision(entityRayRect, newRay);
			bool pathFindPerfect = false;

			if (rotatedCol)
				pathFindPerfect = checkForPathWill(entityWorldPos, automaticPathComp, boxCollisionComp, checkingCol);

			if (rotatedCol && !pathFindPerfect){
				CollidedAvoidBoxEntityData collidedData;
				collidedData.collidedEntity = testEntity;
				collidedData.dotProd = dotProd;
				collidedData.diff = diff;
				listOfCollided.push_back(collidedData);
			}

			continue;
		}

		bool rotatedCol = Utility::rotatedCollision(entityRayRect, *checkingRayRect);
		bool pathFindPerfect = false;

		if (rotatedCol)
			pathFindPerfect = checkForPathWill(entityWorldPos, automaticPathComp, boxCollisionComp, checkingCol);

		if (rotatedCol && !pathFindPerfect){
			CollidedAvoidBoxEntityData collidedData;
			collidedData.collidedEntity = testEntity;
			collidedData.dotProd = dotProd;
			collidedData.diff = diff;
			listOfCollided.push_back(collidedData);
		}
	}

	if (listOfCollided.empty())
		return;
	

	

	CollidedAvoidBoxEntityData* closestData = calculateClosestEntity(entityWorldPos, listOfCollided);
	sf::Vector2f closestDiff = closestData->diff;
	float closestDotProd = closestData->dotProd;
	Entity* closestEntity = closestData->collidedEntity;
		
	TransformableComponent* testTransformComp = nullptr;
	if (closestEntity->hasComp<TransformableComponent>())
		testTransformComp = closestEntity->comp<TransformableComponent>();

	VelocityComponent* testVeloComp = nullptr;
	if (closestEntity->hasComp<VelocityComponent>())
		testVeloComp = closestEntity->comp<VelocityComponent>();

	AvoidanceBoxComponent* testAvoidBoxComp = nullptr;
	if (closestEntity->hasComp<AvoidanceBoxComponent>())
		testAvoidBoxComp = closestEntity->comp<AvoidanceBoxComponent>();
	

	sf::Vector2f testEntityRayPos = testTransformComp->getWorldPosition(true);
	
	if (testVeloComp && testAvoidBoxComp)
		testEntityRayPos += testAvoidBoxComp->getRayLength() * testVeloComp->getVelocity();

	//sf::Vector2f diff = testEntityRayPos - entityRayPos;
	sf::Vector2f diff = closestDiff;
	float dotProd = Utility::getDotProduct(diff, entityV);
	//float dotProd = Utility::getDotProduct(testEntityRayPos - entityRayPos, entityV);
	//float dotProd = closestData->dotProd;

	sf::Vector2f entityRay = entityV * avoidBoxComp->getRayLength();
	sf::Vector2f entityProj = entityV * dotProd;

	float entityRayLength = Utility::vectorLength(entityRay);
	float entityProjLength = Utility::vectorLength(entityProj);

	sf::Vector2f perpendicular(-diff.y, diff.x);
	float sign = Utility::getDotProduct(perpendicular, entityV) < 0 ? -1.f : 1.f;

	float additionAngle = Utility::toDegree(sign * 3.141592653589793238462643383f / 4.f);

	float entityVeloDegree = Utility::vectorToDegree(entityV, false);

	sf::Vector2f entityForce = Utility::degreeToVector(entityVeloDegree + additionAngle);

	entityForce *= veloComp->getSpeed();
	entityForce *= (1 - entityProjLength / entityRayLength);

	entityForce += (entityV * veloComp->getSpeed());
	//entityForce *= (entityProjLength / entityRayLength);

	sf::Vector2f newVDir = Utility::unitVector(entityForce);
	/*if (std::abs(newVDir.x) > std::abs(newVDir.y)){
		newVDir.x *= -1.f;
	}
	else
		newVDir.y *= -1.f;*/

	sf::Vector2f newV = SteeringBehavior::seek(veloComp, newVDir, dt);
	//sf::Vector2f newV = Utility::unitVector(entityForce);
	veloComp->setVelocity(newV, true);
	avoidBoxComp->setJustGotRayed(true);
}

CollidedAvoidBoxEntityData* AvoidanceBoxSystem::calculateClosestEntity(const sf::Vector2f& centerPos,
	 std::vector<CollidedAvoidBoxEntityData>& listEntities) const
{
	CollidedAvoidBoxEntityData* closestData = nullptr;


	float closestDifference = std::numeric_limits<float>().max();
	for (auto& collidedData : listEntities){

		float nowLength = Utility::vectorLength(collidedData.collidedEntity->comp<TransformableComponent>()
			->getWorldPosition(true) - centerPos);

		if (nowLength < closestDifference){
			closestData = &collidedData;
			closestDifference = nowLength;
		}
	}
	return closestData;
}

