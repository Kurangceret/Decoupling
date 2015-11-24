#include "BoxCollisionComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"

BoxCollisionComponent::BoxCollisionComponent(Entity* entity)
:CollisionComponent(entity)
{
	mIdentifier = ComponentIdentifier::BoxCollisionComponent;
}


BoxCollisionComponent::~BoxCollisionComponent()
{
}


sf::FloatRect BoxCollisionComponent::getTransfromedRect() const
{
	if (!mOwnerEntity)
		return mBoundingRect;

	if (!mOwnerEntity->hasComp<TransformableComponent>())
		return mBoundingRect;

	sf::FloatRect transformedRect = mOwnerEntity->comp<TransformableComponent>()->getWorldTransform(true).
		transformRect(mBoundingRect);

	transformedRect.left -= transformedRect.width / 2.f;
	transformedRect.top -= transformedRect.height / 2.f;
	return transformedRect;
}