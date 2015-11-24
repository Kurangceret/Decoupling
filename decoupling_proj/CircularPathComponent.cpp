#include "CircularPathComponent.h"
#include "EntityParentComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"

CircularPathComponent::CircularPathComponent(Entity* entity)
:Component(entity),
mIsRotatingAroundParentPos(false),
mCurRadius(0.f),
mMaxRadius(0.f),
mMinRadius(0.f),
mSpeed(0.f),
mCenter(),
mIsClockwise(true)
{
	mIdentifier = ComponentIdentifier::CircularPathComponent;
}


CircularPathComponent::~CircularPathComponent()
{
}



void CircularPathComponent::setIsRotatingAroundParentPos(bool flag)
{
	mIsRotatingAroundParentPos = flag;
}

bool CircularPathComponent::isRotatingAroundParentPos() const
{
	return mIsRotatingAroundParentPos;
}

void CircularPathComponent::setCenter(sf::Vector2f center)
{
	mCenter = center;
}

sf::Vector2f CircularPathComponent::getCenter() const
{
	if (!mIsRotatingAroundParentPos)
		return mCenter;
	
	EntityParentComponent* parentComp = mOwnerEntity->comp<EntityParentComponent>();
	return parentComp->mParent->comp<TransformableComponent>()->getWorldPosition(true);
}

void CircularPathComponent::increaseCurRadius(float val)
{
	mCurRadius += std::abs(val);
	if (mCurRadius > mMaxRadius)
		mCurRadius = mMaxRadius;
}

void CircularPathComponent::decreaseCurRadius(float val)
{
	mCurRadius -= std::abs(val);
	if (mCurRadius < mMinRadius)
		mCurRadius = mMinRadius;
}

void CircularPathComponent::setCurRadius(float val)
{
	mCurRadius = val;
}

float CircularPathComponent::getCurRadius() const
{
	return mCurRadius;
}

void CircularPathComponent::setSpeed(float speed)
{
	mSpeed = speed;
}

float CircularPathComponent::getSpeed() const
{
	return mSpeed;
}


void CircularPathComponent::setClockwise(bool flag)
{
	mIsClockwise = flag;
}

bool CircularPathComponent::isClockwise() const
{
	return mIsClockwise;
}