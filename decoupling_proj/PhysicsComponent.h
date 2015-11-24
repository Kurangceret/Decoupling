#pragma once
#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "CollisionType.h"
#include "CommandQueue.h"

class GameObject;

class PhysicsComponent /*: public sf::Transformable*/{
public:
	typedef std::unique_ptr<PhysicsComponent> Ptr;
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	virtual void update(GameObject* gameObject, 
		sf::Time dt, CommandQueue& commandQueue) = 0;

	virtual CollisionType::ID getCollisionType() const = 0;
protected:
	GameObject* mGameObject;
};

