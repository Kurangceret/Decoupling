#include "CategoryComponent.h"


CategoryComponent::CategoryComponent(Entity* entity)
:Component(entity),
mCategory(1)
{
	mIdentifier = ComponentIdentifier::CategoryComponent;
}


CategoryComponent::~CategoryComponent()
{
}

std::size_t CategoryComponent::getCategory() const
{
	return mCategory;
}

void CategoryComponent::setCategory(std::size_t category)
{
	mCategory = category;
}
