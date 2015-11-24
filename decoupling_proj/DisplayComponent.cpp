#include "DisplayComponent.h"


DisplayComponent::DisplayComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::DisplayComponent;
}


DisplayComponent::~DisplayComponent()
{
}

sf::Drawable* DisplayComponent::getDrawable()
{
	return nullptr;
}
