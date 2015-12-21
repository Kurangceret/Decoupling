#include "AutomaticMovementSystem.h"
#include "ComponentIdentifier.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "VelocityComponent.h"
#include "Utility.h"
#include "SteeringBehavior.h"
#include "PathFinder.h"
#include "BoxCollisionComponent.h"
#include "RayCast.h"
#include "AvoidanceBoxComponent.h"
#include "EntityExpertiseComponent.h"

AutomaticMovementSystem::AutomaticMovementSystem()
{
	pushRequiredComponent(ComponentIdentifier::AutomaticPathComponent);
	pushRequiredComponent(ComponentIdentifier::VelocityComponent);
	pushRequiredComponent(ComponentIdentifier::TransformableComponent);
}


AutomaticMovementSystem::~AutomaticMovementSystem()
{
}


void AutomaticMovementSystem::processEntity(sf::Time dt, Entity* entity)
{
	if (!entity->hasComp<AutomaticPathComponent>() || 
		!entity->hasComp<VelocityComponent>() || 
		!entity->hasComp<TransformableComponent>())
		return;

	AutomaticPathComponent* pathComponent = entity->comp<AutomaticPathComponent>();

	PathFinder& pathFinder = *PathFinder::getInstance();
	if (pathComponent->isUncalculatedPosValid())
		pathFinder.moveSceneTo(entity, pathComponent->getUncalculatedDest());
	

	AutomaticPathComponent::AutomaticPathList& pathList = 
		pathComponent->getAutomaticPaths();

	VelocityComponent* velocityComponent = entity->comp<VelocityComponent>();

	if (pathList.empty())
		return;
		
	if (entity->hasComp<AvoidanceBoxComponent>()){
		AvoidanceBoxComponent* avoidBox = entity->comp<AvoidanceBoxComponent>();
		if (avoidBox->justGotRayed()){
			avoidBox->setJustGotRayed(false);
			return;
		}
	}

	//sf::Transformable& transformable = entity->comp<TransformableComponent>()->mTransformable;
	sf::Vector2f worldPos = entity->comp<TransformableComponent>()->getWorldPosition(true);

	EntityExpertiseComponent* entExpertiseComp = nullptr;
	if (entity->hasComp<EntityExpertiseComponent>())
		entExpertiseComp = entity->comp<EntityExpertiseComponent>();

	smoothAutomaticPath(worldPos, entity->comp<BoxCollisionComponent>()->mBoundingRect, pathList, entExpertiseComp);

	MovingNode& movingNode = pathList.back();

	sf::Vector2f dirToFront = Utility::unitVector(movingNode.starNode->pos - worldPos);

	if (Utility::getDotProduct(movingNode.direction, dirToFront) <= 0.f){
		pathList.erase(pathList.end() - 1);

		if (pathList.empty()) velocityComponent->setVelocity(0.f, 0.f, false);
	}
	
	if (!pathList.empty()){
		sf::Vector2f finalDir = SteeringBehavior::seekTarget(velocityComponent, 
			worldPos, pathList.back().starNode->pos, dt);
		velocityComponent->setVelocity(finalDir);
		//velocityComponent->setVelocity(Utility::unitVector(pathList.back().starNode->pos - worldPos));
	}
}


void AutomaticMovementSystem::smoothAutomaticPath(const sf::Vector2f& agentPos,
	const sf::FloatRect& boundingRect,
	AutomaticPathComponent::AutomaticPathList& pathList,
	EntityExpertiseComponent* entExpertiseComp)
{
	

	RayCast::TileChecker tileChecker = RayCast::mStandardTileChecker;
	if (entExpertiseComp){
		tileChecker = [&entExpertiseComp](AStarNode* curNode){
			if (!curNode || curNode->tile)
				return false;
			if (!entExpertiseComp->isAbleToFloat() && curNode->isFallable)
				return false;

			return true;
		};
	}

	while (pathList.size() > 1 && RayCast::castRayLinesFromRect(agentPos, boundingRect,
		(pathList.end() - 2)->starNode->pos, PathFinder::getInstance(), tileChecker))
	{
		pathList.erase(pathList.end() - 1);
	}
}