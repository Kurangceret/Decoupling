#pragma once
#include "System.h"
class FallingSystem :	public System{
public:
	typedef std::unique_ptr<FallingSystem> Ptr;
public:
	FallingSystem();
	virtual ~FallingSystem();

protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
};

