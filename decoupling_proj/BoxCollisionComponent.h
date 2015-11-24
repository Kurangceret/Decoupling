#pragma once
#include "CollisionComponent.h"
#include <SFML/Graphics/Rect.hpp>
class BoxCollisionComponent :	public CollisionComponent{
public:
	BoxCollisionComponent(Entity* entity);
	virtual ~BoxCollisionComponent();

	sf::FloatRect getTransfromedRect() const;

	sf::FloatRect mBoundingRect;

};

