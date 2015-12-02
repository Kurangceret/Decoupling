#include "RotatedBoxCollisionComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"

RotatedBoxCollisionComponent::RotatedBoxCollisionComponent(Entity* entity)
:CollisionComponent(entity)
{
	mIdentifier = ComponentIdentifier::RotatedBoxCollisionComponent;
}


RotatedBoxCollisionComponent::~RotatedBoxCollisionComponent()
{
}


RotatedRect RotatedBoxCollisionComponent::getTransformedRotatedRect() const
{
	if (!mOwnerEntity->hasComp<TransformableComponent>())
		return mRotatedRect;
	
	sf::Transform transform = mOwnerEntity->comp<TransformableComponent>()->getWorldTransform();
	
	RotatedRect newRect(mRotatedRect);

	newRect.mPoints[RotatedRect::TopLeft] = transform.
		transformPoint(mRotatedRect.mPoints[RotatedRect::TopLeft]);
	newRect.mPoints[RotatedRect::TopRight] = transform.
		transformPoint(mRotatedRect.mPoints[RotatedRect::TopRight]);
	newRect.mPoints[RotatedRect::BottomRight] = transform.
		transformPoint(mRotatedRect.mPoints[RotatedRect::BottomRight]);
	newRect.mPoints[RotatedRect::BottomLeft] = transform.
		transformPoint(mRotatedRect.mPoints[RotatedRect::BottomLeft]);
	
	return newRect;
}