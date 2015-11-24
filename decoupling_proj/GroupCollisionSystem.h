#pragma once
#include "GroupSystem.h"
class GroupCollisionSystem :	public GroupSystem{
public:
	GroupCollisionSystem();
	virtual ~GroupCollisionSystem();

	//virtual void update(Entity* entity, sf::Time dt);
	void handleEntityWithItsGroup(Entity* entity, sf::Time dt, const
		std::vector<Entity*>& entities);
	//void setAvailableEntities(const std::vector<Entity*>& entities);
private:
	//entities that can be checked for collision
	//std::vector<Entity*> mAvailableEntities;
};

