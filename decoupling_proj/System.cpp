#include "System.h"
#include "Entity.h"
#include "ComponentIdentifier.h"

System::System()
{
}


System::~System()
{
}

void System::update(sf::Time dt, Entity* entity)
{
	//for (auto entity : mEntities)
	if (!checkForEntity(entity))
		return;
		
	processEntity(dt, entity);
}

bool System::checkForEntity(Entity* entity) const
{
	if (mRequiredComponents.empty())
		return true;

	/*Entity::EntityBitSet compBit = entity->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponents){
		if (!compBit.test(identifier))
			return false;
	}*/

	std::size_t compBit = entity->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponents){
		if (!(identifier & compBit))
			return false;
	}

	return true;
}

void System::addEntity(Entity* entity)
{
	if (checkForEntity(entity))
		mEntities.push_back(entity);
}

void System::setRequiredComponents(const
	std::vector<std::size_t>& requiredComponents)
{
	mRequiredComponents = requiredComponents;
}

void System::pushRequiredComponent(std::size_t componentIdentifier)
{
	mRequiredComponents.push_back(componentIdentifier);
}