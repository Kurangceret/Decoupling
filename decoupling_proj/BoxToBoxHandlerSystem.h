#pragma once
#include "CollisionHandlerSystem.h"

class CollisionComponent;
class BoxToBoxHandlerSystem :	public CollisionHandlerSystem{
public:
	typedef std::unique_ptr<BoxToBoxHandlerSystem> Ptr;
public:
	BoxToBoxHandlerSystem();
	virtual ~BoxToBoxHandlerSystem();

private:
	virtual bool isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
		CollisionComponent*& entity1Col, CollisionComponent*& entity2Col);
};

