#pragma once
#include "PhysicsComponent.h"
class BoundingBoxPhysicsComponent :	public PhysicsComponent{
public:
	BoundingBoxPhysicsComponent(sf::FloatRect boundingSize = sf::FloatRect());
	virtual ~BoundingBoxPhysicsComponent();

	sf::FloatRect getTransformedBoundingBox() const;

	virtual void update(GameObject* gameObject,
		sf::Time dt, CommandQueue& commandQueue);

	virtual CollisionType::ID getCollisionType() const;
private:
	sf::FloatRect mBoundingRect;
	
};

