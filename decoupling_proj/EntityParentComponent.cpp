#include "EntityParentComponent.h"


EntityParentComponent::EntityParentComponent(Entity* entity)
:Component(entity),
mParent(nullptr)
{
	mIdentifier = ComponentIdentifier::EntityParentComponent;
}


EntityParentComponent::~EntityParentComponent()
{
}


Entity* EntityParentComponent::getParent()
{
	return mParent;
}