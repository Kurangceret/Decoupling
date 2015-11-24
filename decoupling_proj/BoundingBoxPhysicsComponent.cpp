#include "BoundingBoxPhysicsComponent.h"
#include "GameObject.h"

BoundingBoxPhysicsComponent::BoundingBoxPhysicsComponent(sf::FloatRect boundingSize)
:mBoundingRect(boundingSize)
{
}


BoundingBoxPhysicsComponent::~BoundingBoxPhysicsComponent()
{
}


void BoundingBoxPhysicsComponent::update(GameObject* gameObject,
	sf::Time dt, CommandQueue& commandQueue)
{
	if(!mGameObject)mGameObject = gameObject;
}

sf::FloatRect BoundingBoxPhysicsComponent::getTransformedBoundingBox() const
{
	if (mGameObject)
		return mGameObject->getTransform().transformRect(mBoundingRect);

	return mBoundingRect;
}

CollisionType::ID BoundingBoxPhysicsComponent::getCollisionType() const
{
	return CollisionType::BoundingBoxType;
}