#pragma once

#include "ComponentIdentifier.h"
#include <memory>

class Entity;

class Component{
public:
	typedef std::unique_ptr<Component> Ptr;
public:
	Component(Entity* entity);
	virtual ~Component();

	std::size_t mIdentifier;

	Entity* mOwnerEntity;

};

