#include "Entity.h"


Entity::Entity(std::size_t id)
:mEntityId(id),
mComponentsIdentifiers(0)
{
}


Entity::~Entity()
{
}

std::size_t Entity::getId() const
{
	return mEntityId;
}

bool Entity::hasComp(ComponentIdentifier::ID componentIden)
{
	if (mComponentsIdentifiers & componentIden)
		return true;
	return false;
}

/*Entity::EntityBitSet Entity::getComponentsIdentifiers()
{
	return mComponentsIdentifiers;
}

const Entity::EntityBitSet& Entity::getComponentsIdentifiers() const
{
	return mComponentsIdentifiers;
}*/

std::size_t Entity::getComponentsIdentifiers() const
{
	return mComponentsIdentifiers;
}