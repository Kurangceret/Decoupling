#include "MeleeRectToBoxHandlerSystem.h"
#include "MeleeRectComponent.h"
#include "BoxCollisionComponent.h"
#include "Utility.h"

MeleeRectToBoxHandlerSystem::MeleeRectToBoxHandlerSystem(CommonBattleLogic& commonBattleLogic)
:mCommonBattleLogic(commonBattleLogic)
{
	pushEntity1Requirement(ComponentIdentifier::MeleeRectComponent);
	pushEntity2Requirement(ComponentIdentifier::BoxCollisionComponent);
}


MeleeRectToBoxHandlerSystem::~MeleeRectToBoxHandlerSystem()
{
}


bool MeleeRectToBoxHandlerSystem::isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
	CollisionComponent*& entity1Col, CollisionComponent*& entity2Col)
{
	MeleeRectComponent* entity1MeleeComp = entity1->comp<MeleeRectComponent>();
	BoxCollisionComponent* entity2BoxComp = entity2->comp<BoxCollisionComponent>();
	if (!entity1MeleeComp->isNowCollidedable() || 
		entity1MeleeComp->entityHasBeenTouched(entity2))
		return false;
	

	const RotatedRect& lhs = entity1MeleeComp->getRotatedRect();
	RotatedRect rhs(entity2BoxComp->getTransfromedRect());

	if(!Utility::rotatedCollision(lhs, rhs))
		return false;

	entity1Col = entity1MeleeComp;
	entity2Col = entity2BoxComp;

	mCommonBattleLogic.insertNewPair(entity1MeleeComp, entity2);
	return true;
}