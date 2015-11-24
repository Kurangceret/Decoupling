#include "GroupSystem.h"


GroupSystem::GroupSystem()
{
}


GroupSystem::~GroupSystem()
{
}


void GroupSystem::update(Entity* entity, sf::Time dt)
{
	for (auto& system : mSystems)
		system->update(dt, entity);
}

void GroupSystem::addSystem(System* system)
{
	mSystems.push_back(system);
}