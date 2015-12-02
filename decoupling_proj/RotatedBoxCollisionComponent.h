#pragma once
#include "CollisionComponent.h"
#include "RotatedRect.h"

class ComponentArranger;

class RotatedBoxCollisionComponent :	public CollisionComponent{
public:
	friend ComponentArranger;
public:
	RotatedBoxCollisionComponent(Entity* entity);
	virtual ~RotatedBoxCollisionComponent();

	
	RotatedRect getTransformedRotatedRect() const;
private:
	RotatedRect mRotatedRect;
};

