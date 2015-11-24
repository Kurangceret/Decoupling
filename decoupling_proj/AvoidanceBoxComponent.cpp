#include "AvoidanceBoxComponent.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "Entity.h"

AvoidanceBoxComponent::AvoidanceBoxComponent(Entity* entity)
:Component(entity),
mRayLength(0.f),
mRayThickness(0.f),
mCurrentlyCanBeRayCast(true),
mJustGotRayed(false)
{
	mIdentifier = ComponentIdentifier::AvoidanceBoxComponent;
}


AvoidanceBoxComponent::~AvoidanceBoxComponent()
{
}


float AvoidanceBoxComponent::getRayLength() const
{
	float rayLength = mRayLength;

	if (rayLength == 0.f)
		rayLength = mOwnerEntity->comp<VelocityComponent>()->getSpeed() * 1.25f;

	
	return rayLength;
}

float AvoidanceBoxComponent::getRayThickness() const
{
	float rayThickness = mRayThickness;
	if (rayThickness == 0.f)
		rayThickness = mOwnerEntity->comp<BoxCollisionComponent>()->mBoundingRect.width / 1.3f;

	return rayThickness;
}


RotatedRect& AvoidanceBoxComponent::getRayRect()
{
	return mRayRect;
}

void AvoidanceBoxComponent::setCanBeRayCast(bool flag)
{
	mCurrentlyCanBeRayCast = flag;
}

bool AvoidanceBoxComponent::currentlyCanBeRayCast() const
{
	return mCurrentlyCanBeRayCast;
}

void  AvoidanceBoxComponent::setJustGotRayed(bool flag)
{
	mJustGotRayed = flag;
}

bool  AvoidanceBoxComponent::justGotRayed() const
{
	return mJustGotRayed;
}