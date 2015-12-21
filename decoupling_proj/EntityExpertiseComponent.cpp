#include "EntityExpertiseComponent.h"


EntityExpertiseComponent::EntityExpertiseComponent(Entity* entity)
:Component(entity),
mAbleToFloat(false),
mAbleToWalk(true)
{
	mIdentifier = ComponentIdentifier::EntityExpertiseComponent;
}


EntityExpertiseComponent::~EntityExpertiseComponent()
{
}


void EntityExpertiseComponent::setAbleToFloat(bool flag)
{
	mAbleToFloat = flag;
}

bool EntityExpertiseComponent::isAbleToFloat() const
{
	return mAbleToFloat;
}

void EntityExpertiseComponent::setAbleToWalk(bool flag)
{
	mAbleToWalk = flag;
}

bool EntityExpertiseComponent::isAbleToWalk() const
{
	return mAbleToWalk;
}