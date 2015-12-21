#include "DestroyableComponent.h"
#include <iostream>

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
	try{
		if (mLuaDestroyedFunc)
			return (*mLuaDestroyedFunc)(mOwnerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
	return false;
}

bool DestroyableComponent::isRemoveable() const
{
	try{
		if (mLuaRemoveableFunc)
			return (*mLuaRemoveableFunc)(mOwnerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
	return false;
}