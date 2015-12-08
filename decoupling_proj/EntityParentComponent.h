#pragma once
#include "Component.h"

class Entity;
class EntityChildrenComponent;
class EntityParentComponent :	public Component{
public:
	friend EntityChildrenComponent;
public:
	EntityParentComponent(Entity* entity);
	virtual ~EntityParentComponent();

	Entity* getParent();
private:
	Entity* mParent;
};

