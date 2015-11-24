#include "DestroyableComponent.h"


DestroyableComponent::DestroyableComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::DestroyableComponent;
}


DestroyableComponent::~DestroyableComponent()
{
}
