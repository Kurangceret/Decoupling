#pragma once
#include "Component.h"
#include <vector>

class EntityManager;

class EntityChildrenComponent :	public Component{
public:
	EntityChildrenComponent(Entity* entity);
	virtual ~EntityChildrenComponent();

	void removeChild(Entity* childEntity);
	void removeRemoveableChild(EntityManager* entityManager);

	void insertNewChild(Entity* childEntity);
	bool isCategoryInsideChildren(std::size_t category) const;
private:
	std::vector<Entity*> mChildren;
};

