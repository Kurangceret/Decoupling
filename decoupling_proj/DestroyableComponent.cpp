#include "DestroyableComponent.h"


DestroyableComponent::DestroyableComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::DestroyableComponent;
}


DestroyableComponent::~DestroyableComponent()
{
}


bool DestroyableComponent::isDestroyed() const
{
	if (mLuaDestroyedFunc)
		return (*mLuaDestroyedFunc)(mOwnerEntity);
	return false;
}

bool DestroyableComponent::isRemoveable() const
{
	if (mLuaRemoveableFunc)
		return (*mLuaRemoveableFunc)(mOwnerEntity);
	return false;
}