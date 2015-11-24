#pragma once
#include "Component.h"
class DestroyableComponent :	public Component{
public:
	DestroyableComponent(Entity* entity);
	virtual ~DestroyableComponent();
};

