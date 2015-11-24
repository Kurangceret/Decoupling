#include "GroupCollisionSystem.h"
#include "Entity.h"
#include "CollisionComponent.h"
#include "CollisionHandlerSystem.h"
#include "VelocityComponent.h"

GroupCollisionSystem::GroupCollisionSystem()
{
}


GroupCollisionSystem::~GroupCollisionSystem()
{
}

void GroupCollisionSystem::handleEntityWithItsGroup(Entity* entity, sf::Time dt, const
	std::vector<Entity*>& availableEntities)
{
	if (!entity->hasComp <VelocityComponent>())
		return;

	for (auto& entity2 : availableEntities){
		if (entity == entity2)
			continue;

		for (auto& system : mSystems){
			CollisionHandlerSystem* handlerSystem = dynamic_cast<CollisionHandlerSystem*>(system);
			if (!handlerSystem)
				continue;

			handlerSystem->checkingTwoEntities(entity, entity2, dt);
			//handlerSystem->checkingTwoEntities
		}
	}
}



