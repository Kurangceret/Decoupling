#pragma once
#include <SFML/System/Vector2.hpp>
#include "CommandQueue.h"
#include <memory>

class GameObject;

class MovingComponent{
public:
	typedef std::unique_ptr<MovingComponent> Ptr;
public:
	MovingComponent( float speed = 0.f);
	virtual ~MovingComponent();


	void setVelocity(float x, float y);
	void setVelocity(const sf::Vector2f& velocity);

	sf::Vector2f getVelocity() const;
	float getXVelocity() const;
	float getYVelocity() const;

	void setSpeed(float speed);
	float getSpeed() const;

	virtual void update(GameObject* gameObject, sf::Time dt, CommandQueue& commandQueue);

private:
	sf::Vector2f mVelocity;
	float mSpeed;
};

