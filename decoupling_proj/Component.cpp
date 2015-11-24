#include "Component.h"


Component::Component(Entity* entity)
:mOwnerEntity(entity),
mIdentifier(ComponentIdentifier::EmptyComponent)
{
	//mComponentIdentifier = ComponentIdentifier::EmptyComponent;
}


Component::~Component()
{
}
