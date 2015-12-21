#include "FallingSystem.h"
#include "Entity.h"
//#include "FloatableComponent.h"
#include "PathFinder.h"
#include "TransformableComponent.h"
#include "HealthComponent.h"
#include "Utility.h"
#include "EntityExpertiseComponent.h"

FallingSystem::FallingSystem()
{
	pushRequiredComponent(ComponentIdentifier::TransformableComponent);
	pushRequiredComponent(ComponentIdentifier::VelocityComponent);
	pushRequiredComponent(ComponentIdentifier::HealthComponent);
	//pushRequiredComponent(ComponentIdentifier::FloatableComponent);
}


FallingSystem::~FallingSystem()
{
}


void FallingSystem::processEntity(sf::Time dt, Entity* entity)
{
	if (entity->hasComp<EntityExpertiseComponent>() && entity->comp<EntityExpertiseComponent>()->isAbleToFloat())
		return;

	sf::Vector2f entityWorldPos = entity->comp<TransformableComponent>()->getWorldPosition(true);

	PathFinder* pathFinder = PathFinder::getInstance();
	

	AStarNode* curNode = pathFinder->sceneToGraph(entityWorldPos);
	if (!curNode->isFallable)
		return;
	
	sf::Vector2f tileSize = pathFinder->getTileSize();

	if (Utility::vectorLength(curNode->pos - 
		entityWorldPos) <= (tileSize.x * 0.5f))
	{
		HealthComponent* healthComp = entity->comp<HealthComponent>();
		healthComp->damage(healthComp->getMaxHealth(), entity);
	}
}