#include "MovingComponent.h"
#include "GameObject.h"

MovingComponent::MovingComponent(float speed)
:mVelocity(0.f, 0.f),
mSpeed(speed)
{
}


MovingComponent::~MovingComponent()
{
}


void MovingComponent::setVelocity(float x, float y)
{
	mVelocity.x = x;
	mVelocity.y = y;
}

void MovingComponent::setVelocity(const sf::Vector2f& velocity)
{
	mVelocity = velocity;
}

sf::Vector2f MovingComponent::getVelocity() const
{
	return mVelocity;
}

float MovingComponent::getXVelocity() const
{
	return mVelocity.x;
}

float MovingComponent::getYVelocity() const
{
	return mVelocity.y;
}

void MovingComponent::setSpeed(float speed)
{
	mSpeed = speed;
}

float MovingComponent::getSpeed() const
{
	return mSpeed;
}

void MovingComponent::update(GameObject* gameObject, sf::Time dt, CommandQueue& commandQueue)
{
	
	gameObject->move(mSpeed * dt.asSeconds * mVelocity);
}

