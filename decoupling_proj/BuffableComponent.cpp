#include "BuffableComponent.h"
#include <iostream>
#include "Entity.h"

BuffableComponent::BuffableComponent(Entity* entity)
:Component(entity),
mLuaState(nullptr)
{
	mIdentifier = ComponentIdentifier::BuffableComponent;
}


BuffableComponent::~BuffableComponent()
{
}

void BuffableComponent::update(sf::Time dt)
{
	while (!mQueueBuffList.empty()){
		mQueueBuffList.front()->enterBuff(mOwnerEntity, dt);
		mBuffScriptList.push_back(std::move(mQueueBuffList.front()));
		mQueueBuffList.pop();
	}
	
	for (auto ptrIter = mBuffScriptList.begin(); ptrIter != mBuffScriptList.end();){
		auto& ptr = (*ptrIter);
		if (ptr->isBuffOver(mOwnerEntity)){
			ptr->exitBuff(mOwnerEntity, dt);
			ptrIter = mBuffScriptList.erase(ptrIter);
			continue;
		}
		ptr->updateBuff(mOwnerEntity, dt);
		ptrIter++;
	}

}

void BuffableComponent::insertNewBuff(BuffScript* buffPtr)
{
	//mBuffScriptList.push_back(std::move(buffPtr));
	BuffScript::Ptr newBuffPtr(buffPtr);

	mQueueBuffList.push(std::move(newBuffPtr));
}

void BuffableComponent::insertNewBuffFromScript(lua_State* luaState)
{
	try{
		luabridge::LuaRef param = luabridge::LuaRef::fromStack(luaState, 2);
		
		if (getFirstBuffByName(param["name"].cast<std::string>()))
			return;

		if (param.isTable())
			insertNewBuff(new BuffScript(std::make_unique<luabridge::LuaRef>(param)));
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void BuffableComponent::insertBuffWithScriptName(const std::string& scriptName,
	const std::string& tableName)
{
	if (luaL_dofile(mLuaState, scriptName.c_str()) != 0){
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		return;
	}
	luaL_openlibs(mLuaState);

	lua_pcall(mLuaState, 0, 0, 0);

	luabridge::LuaRef buffTable = luabridge::getGlobal(
		mLuaState, tableName.c_str());

	try{
		buffTable["newBuffFunc"](mOwnerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}

BuffScript* BuffableComponent::getFirstBuffByName(const std::string& buffName)
{
	auto findIter = std::find_if(mBuffScriptList.begin(), mBuffScriptList.end(), 
		[&](BuffScript::Ptr& ptr)
	{
		return ptr->getBuffName() == buffName;
	});

	if (findIter == mBuffScriptList.end())
		return nullptr;

	return findIter->get();
}

void BuffableComponent::destroyAllBuff()
{
	for (auto& buffPtr : mBuffScriptList)
		buffPtr->absoluteDestroy();
}

