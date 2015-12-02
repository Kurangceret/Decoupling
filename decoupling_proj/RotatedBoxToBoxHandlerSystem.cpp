#include "RotatedBoxToBoxHandlerSystem.h"
#include "RotatedBoxCollisionComponent.h"
#include "Entity.h"
#include "BoxCollisionComponent.h"
#include "Utility.h"

RotatedBoxToBoxHandlerSystem::RotatedBoxToBoxHandlerSystem()
{
	pushEntity1Requirement(ComponentIdentifier::RotatedBoxCollisionComponent);
	pushEntity2Requirement(ComponentIdentifier::BoxCollisionComponent);
}


RotatedBoxToBoxHandlerSystem::~RotatedBoxToBoxHandlerSystem()
{
}


bool RotatedBoxToBoxHandlerSystem::isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
	CollisionComponent*& entity1Col, CollisionComponent*& entity2Col)
{
	RotatedBoxCollisionComponent* rotatedBoxComp1 = entity1->comp<RotatedBoxCollisionComponent>();
	BoxCollisionComponent* boxComp2 = entity2->comp<BoxCollisionComponent>();

	RotatedRect rotatedRect1 = rotatedBoxComp1->getTransformedRotatedRect();
	RotatedRect rotatedRect2(boxComp2->getTransfromedRect());

	if (!Utility::rotatedCollision(rotatedRect1, rotatedRect2))
		return false;

	entity1Col = rotatedBoxComp1;
	entity2Col = boxComp2;
	return true;
}