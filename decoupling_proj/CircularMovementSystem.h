#pragma once
#include "System.h"
class CircularMovementSystem :	public System{
public:
	CircularMovementSystem();
	virtual ~CircularMovementSystem();
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
};

