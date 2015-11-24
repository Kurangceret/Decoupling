#pragma once
#include "CollisionHandlerSystem.h"
#include "CommonBattleLogic.h"

class CollisionComponent;
class MeleeRectToBoxHandlerSystem :	public CollisionHandlerSystem{
public:
	typedef std::unique_ptr<MeleeRectToBoxHandlerSystem> Ptr;
public:
	MeleeRectToBoxHandlerSystem(CommonBattleLogic& commonBattleLogic);
	virtual ~MeleeRectToBoxHandlerSystem();

private:
	virtual bool isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
		CollisionComponent*& entity1Col, CollisionComponent*& entity2Col);

private:
	CommonBattleLogic& mCommonBattleLogic;
};

