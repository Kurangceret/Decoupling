#pragma once
#include "Component.h"

class Entity;
class EntityParentComponent :	public Component{
public:
	EntityParentComponent(Entity* entity);
	virtual ~EntityParentComponent();

	Entity* mParent;
};

