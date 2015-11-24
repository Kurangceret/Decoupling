#include "MeleeRectSystem.h"
#include "MeleeRectComponent.h"
#include "Entity.h"
#include "Utility.h"

MeleeRectSystem::MeleeRectSystem()
{
	pushRequiredComponent(ComponentIdentifier::MeleeRectComponent);
}


MeleeRectSystem::~MeleeRectSystem()
{
}


void MeleeRectSystem::processEntity(sf::Time dt, Entity* entity)
{
	MeleeRectComponent* meleeRectComp = entity->comp<MeleeRectComponent>();
	meleeRectComp->update(dt);

	if (meleeRectComp->isNowCollidedable())
		mRotatedRectList.push_back(&meleeRectComp->getRotatedRect());
}

void MeleeRectSystem::drawCollideableMeleeRect(sf::RenderTarget& target)
{
	for (const auto rotatedRect : mRotatedRectList)
		Utility::drawRotatedRect(target, rotatedRect);

	mRotatedRectList.clear();
}