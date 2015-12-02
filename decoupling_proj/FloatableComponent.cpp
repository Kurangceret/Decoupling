#include "FloatableComponent.h"


FloatableComponent::FloatableComponent(Entity* entity)
:Component(entity),
mIsFloating(false)
{
	mIdentifier = ComponentIdentifier::FloatableComponent;
}


FloatableComponent::~FloatableComponent()
{
}


void FloatableComponent::setIsFloating(bool flag)
{
	mIsFloating = flag;
}

bool FloatableComponent::isFloating() const
{
	return mIsFloating;
}