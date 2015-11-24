#pragma once
#include "System.h"

class GroupSystem{
public:
	GroupSystem();
	virtual ~GroupSystem();


	virtual void update(Entity* entity, sf::Time dt);

	void addSystem(System* system);
protected:
	std::vector<System*> mSystems;
};

