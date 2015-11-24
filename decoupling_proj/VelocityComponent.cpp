#include "VelocityComponent.h"
#include "CategoryComponent.h"
#include "Entity.h"

VelocityComponent::VelocityComponent(Entity* entity)
:Component(entity),
mVelocity(0.f, 0.f),
mSpeed(0.f),
mSprintSpeed(0.f),
mFacingDirection(0.f, 1.f),
mIsSprintingOneFrame(false),
mSpeedIdentifier(1.f),
mDrawFacingDirection(true)
{
	mIdentifier = ComponentIdentifier::VelocityComponent;
}


VelocityComponent::~VelocityComponent()
{
}

void VelocityComponent::updateDirBasedOnVelo()
{
	
	/*The four direction algorithm exclusive*/
	/*
	if (mVelocity.y != 0.f){
		mFacingDirection.y = mVelocity.y > 0.f ? 1.f : -1.f;
		if (mVelocity.x == 0.f)mFacingDirection.x = 0.f;
	}

	if (mVelocity.x != 0.f){
		mFacingDirection.y = 0.f;
		mFacingDirection.x = mVelocity.x > 0.f ? 1.f : -1.f;
	}*/

	if (mVelocity == sf::Vector2f())
		return;
	
	if (mVelocity.y != 0.f)	mFacingDirection.y = mVelocity.y > 0.f ? 1.f : -1.f;
	else mFacingDirection.y = 0.f;

	if (mVelocity.x != 0.f)	mFacingDirection.x = mVelocity.x > 0.f ? 1.f : -1.f;
	else mFacingDirection.x = 0.f;
}

void VelocityComponent::setFacingDirectionX(float x)
{
	mFacingDirection.x = x;
	mFacingDirection.y = 0.f;
}

void VelocityComponent::setFacingDirectionY(float y)
{
	mFacingDirection.y = y;
	mFacingDirection.x = 0.f;
}

sf::Vector2f VelocityComponent::getFacingDirection() const
{
	return mFacingDirection;
}

float VelocityComponent::getFacingDirectionX() const
{
	return mFacingDirection.x;
}

float VelocityComponent::getFacingDirectionY() const
{
	return mFacingDirection.y;
}

void VelocityComponent::setVelocity(const sf::Vector2f& vector, bool turnFaceDir)
{
	setVelocity(vector.x, vector.y, turnFaceDir);
}
void VelocityComponent::setVelocity(float x, float y, bool turnFaceDir)
{
	/*if ((x != 0.f || y != 0.f) && mOwnerEntity->comp<CategoryComponent>()->getCategory() == 8)
		int i = 0;*/

	setVelocityX(x, turnFaceDir);
	setVelocityY(y, turnFaceDir);
}

void VelocityComponent::setVelocityScript(float x, float y, bool turnFaceDir)
{
	setVelocity(x, y, turnFaceDir);
}

void VelocityComponent::setVelocityY(float y, bool turnFaceDir)
{
	mVelocity.y = y;
	if (turnFaceDir)
		updateDirBasedOnVelo();
}

void VelocityComponent::setVelocityX(float x, bool turnFaceDir)
{
	mVelocity.x = x;
	if (turnFaceDir)
		updateDirBasedOnVelo();
}

sf::Vector2f VelocityComponent::getVelocity() const
{
	return mVelocity;
}


float VelocityComponent::getVelocityX() const
{
	return mVelocity.x;
}

float VelocityComponent::getVelocityY() const
{
	return mVelocity.y;
}


void VelocityComponent::setSpeed(float speed)
{
	mSpeed = speed;
}

float VelocityComponent::getSpeed() const
{
	return mSpeed * mSpeedIdentifier;
}

void VelocityComponent::setSpeedIdentifier(float val)
{
	mSpeedIdentifier = val;
}

float VelocityComponent::getSpeedIdentifier() const
{
	return mSpeedIdentifier;
}

void VelocityComponent::setSprintSpeed(float sprintSpeed)
{
	mSprintSpeed = sprintSpeed;
}

float VelocityComponent::getSprintSpeed() const
{
	return mSprintSpeed;
}

void VelocityComponent::sprintOneFrame()
{
	mIsSprintingOneFrame = true;
}

bool VelocityComponent::isSprintingOneFrame()
{
	bool original = mIsSprintingOneFrame;
	mIsSprintingOneFrame = false;
	return original;
}

bool VelocityComponent::isDrawingFacingDirection() const
{
	return mDrawFacingDirection;
}