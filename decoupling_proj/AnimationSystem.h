#pragma once
#include "System.h"
class AnimationSystem :	public System{
public:
	AnimationSystem();
	virtual ~AnimationSystem();

protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
	
};

