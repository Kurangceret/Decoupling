#include "CollisionHandlerSystem.h"
#include "ComponentIdentifier.h"
#include "CollisionComponent.h"
#include "ScriptAIComponent.h"

CollisionHandlerSystem::CollisionHandlerSystem()
{
	//pushRequiredComponent(ComponentIdentifier::CollisionComponent);
}


CollisionHandlerSystem::~CollisionHandlerSystem()
{
}



void CollisionHandlerSystem::processEntity(sf::Time dt, Entity* entity)
{

}

sf::Vector2f CollisionHandlerSystem::getCalculatedSafeOffSet(Entity* keyEntity)
{
	auto iter = mCurrentCollidedEntities.find(keyEntity);
	if (iter == mCurrentCollidedEntities.end())
		return sf::Vector2f();

	return iter->second;
}

void CollisionHandlerSystem::prepareForNextCollisionTest()
{
	mCurrentCollidedEntities.clear();
}

void CollisionHandlerSystem::pushCollidedEntity(Entity* entity, const sf::Vector2f& safeMovePos)
{
	mCurrentCollidedEntities[entity] = safeMovePos;
}

void CollisionHandlerSystem::checkingTwoEntities(Entity* entity1, Entity* entity2, sf::Time dt)
{
	/*if (!checkForEntity(entity1) || !checkForEntity(entity2))
		return;*/
	if (!entityOneRequirement(entity1) || !entityTwoRequirement(entity2))
		return;
	CollisionComponent* collisionComponent1 = nullptr;
	CollisionComponent* collisionComponent2 = nullptr;
	if (!isBothEntitesCollided(entity1, entity2, dt, collisionComponent1, collisionComponent2))
		return;

	if (collisionComponent1)
		collisionComponent1->callCollisionReactor(entity1, entity2, this);
	
	if (collisionComponent2)
		collisionComponent2->callCollisionReactor(entity2, entity1, this);

	prepareForNextCollisionTest();
}

bool CollisionHandlerSystem::entityOneRequirement(Entity* entity1) const
{
	if (mRequiredComponentsEntity1.empty())
		return true;

	/*Entity::EntityBitSet compBit = entity1->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponentsEntity1){
		if (!compBit.test(identifier))
			return false;
	}*/

	std::size_t compBit = entity1->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponentsEntity1){
		if (!(identifier & compBit))
			return false;
	}

	return true;
}

bool CollisionHandlerSystem::entityTwoRequirement(Entity* entity2) const
{
	if (mRequiredComponentsEntity2.empty())
		return true;

	/*Entity::EntityBitSet compBit = entity2->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponentsEntity2){
		if (!compBit.test(identifier))
			return false;
	}*/
	std::size_t compBit = entity2->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponentsEntity2){
		if (!(identifier & compBit))
			return false;
	}

	return true;
}


void CollisionHandlerSystem::pushEntity1Requirement(std::size_t componentIdentifier)
{
	mRequiredComponentsEntity1.push_back(componentIdentifier);
}

void CollisionHandlerSystem::pushEntity2Requirement(std::size_t componentIdentifier)
{
	mRequiredComponentsEntity2.push_back(componentIdentifier);
}