#pragma once
#include "CollisionHandlerSystem.h"
class RotatedBoxToBoxHandlerSystem :	public CollisionHandlerSystem{
public:
	typedef std::unique_ptr<RotatedBoxToBoxHandlerSystem> Ptr;
public:
	RotatedBoxToBoxHandlerSystem();
	virtual ~RotatedBoxToBoxHandlerSystem();

private:
	virtual bool isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
		CollisionComponent*& entity1Col, CollisionComponent*& entity2Col);
};

