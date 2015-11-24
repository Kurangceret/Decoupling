#pragma once
#include "System.h"
#include "EntityManager.h"
#include <unordered_map>
#include <SFML/System/Vector2.hpp>

class CollisionComponent;
class CollisionHandlerSystem :	public System{
public:
	CollisionHandlerSystem();
	virtual ~CollisionHandlerSystem();

	void checkingTwoEntities(Entity* entity1, Entity* entity2, sf::Time dt);

	sf::Vector2f getCalculatedSafeOffSet(Entity* keyEntity);
	void prepareForNextCollisionTest();
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
	void pushCollidedEntity(Entity* entity, const sf::Vector2f& safeMovePos);

	void pushEntity1Requirement(std::size_t componentIdentifier);
	void pushEntity2Requirement(std::size_t componentIdentifier);
private:
	virtual bool isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
		CollisionComponent*& entity1Col, CollisionComponent*& entity2Col) = 0;

	virtual bool entityOneRequirement(Entity* entity1) const;
	virtual bool entityTwoRequirement(Entity* entity2) const;
private:
	std::unordered_map<Entity*, sf::Vector2f> mCurrentCollidedEntities;

	std::vector<std::size_t> mRequiredComponentsEntity1;
	std::vector<std::size_t> mRequiredComponentsEntity2;
};

