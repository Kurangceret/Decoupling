#pragma once
#include "Component.h"
#include <bitset>
#include <vector>
#include <memory>
#include <iostream>

class Entity{
public:
	typedef std::unique_ptr<Entity> Ptr;
	typedef std::bitset<100000> EntityBitSet;
public:
	Entity(std::size_t id);
	~Entity();

	std::size_t getId() const;


	//EntityBitSet getComponentsIdentifiers();
	//const EntityBitSet& getComponentsIdentifiers() const;

	std::size_t getComponentsIdentifiers() const;
	template <class T> bool hasComp();
	bool hasComp(ComponentIdentifier::ID componentIden);

	// Variadic template will allow users and systems to check if an entity has any number of specific entities
	//template <class T1, class T2, class... Other> bool hasComps();

	template <class T> bool removeComp();

	template <class T> T* comp();
	template <class T> T* nonCreateComp();

private:
	std::size_t mEntityId;
	std::vector<Component::Ptr> mComponents;
	//EntityBitSet mComponentsIdentifiers;
	std::size_t mComponentsIdentifiers;

	// Disambiguate Recursive Variadic Template
	template <class T> bool hasComps()
	{
		return this->hasComp<T>();
	}
};


template <class T>
bool Entity::hasComp()
{
	T comp(this);

	//if (mComponentsIdentifiers.test(comp.mIdentifier))
	if (mComponentsIdentifiers & comp.mIdentifier)
	{
		return true;
	}

	return false;
}




/*template <class T1, class T2, class... Other> bool Entity::hasComps()
{
	T1 comp(this);

	if (mComponentsIdentifiers.test(comp.mIdentifier))
	{
		return (true && this->hasComps<T2, Other...>());
	}

	return false;
}*/

template <class T> bool Entity::removeComp()
{
	T comp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier == comp.mIdentifier)
		{
			//unregister component from flags
			//mComponentsIdentifiers.set(comp.mIdentifier, false);
			mComponentsIdentifiers = mComponentsIdentifiers ^ comp.mIdentifier;

			mComponents.erase(mComponents.begin() + i);
			return true;
		}
	}

	return false;
}

template <class T> T* Entity::nonCreateComp()
{
	T comp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier & comp.mIdentifier)
		{
			return dynamic_cast<T*>(mComponents[i].get());
		}
	}
	return nullptr;
}

template <class T> T* Entity::comp()
{
	T comp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier & comp.mIdentifier)
		{
			return dynamic_cast<T*>(mComponents[i].get());
		}
	}

	//mComponentsIdentifiers.set(comp.mIdentifier);
	mComponentsIdentifiers = mComponentsIdentifiers | comp.mIdentifier;
	std::unique_ptr<T> newPtr(new T(this));

	mComponents.push_back(std::move(newPtr));

	return	dynamic_cast<T*>(mComponents.back().get());
}