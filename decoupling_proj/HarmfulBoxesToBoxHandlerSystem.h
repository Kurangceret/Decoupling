#pragma once
#include "CollisionHandlerSystem.h"
class HarmfulBoxesToBoxHandlerSystem :public CollisionHandlerSystem{
public:
	HarmfulBoxesToBoxHandlerSystem();
	virtual ~HarmfulBoxesToBoxHandlerSystem();

private:
	virtual bool isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
		CollisionComponent*& entity1Col, CollisionComponent*& entity2Col);
};

