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
		luabridge::LuaRef newBuff = luabridge::LuaRef::fromStack(luaState, 2);
		
		if (getFirstBuffByName(newBuff["name"].cast<std::string>()))
			return;

		luabridge::LuaRef buffInitializer = luabridge::LuaRef::fromStack(luaState, 3);
		if (!buffInitializer.isNil()){
			//std::unique_ptr<luabridge::LuaRef> test = std::move(std::make_unique<luabridge::LuaRef>(buffInitializer));
			insertNewBuff(new BuffScript(std::make_unique<luabridge::LuaRef>(
				newBuff), BuffScript::mEmptyNativeInitializer, std::make_unique<luabridge::LuaRef>(buffInitializer)));
		}
		else{
			insertNewBuff(new BuffScript(std::make_unique<luabridge::LuaRef>(newBuff)));
		}
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void BuffableComponent::insertBuffWithScriptName(const std::string& scriptName,
	const std::string& tableName,
	const BuffScript::NativeBuffInitializer& nativeBuffInitalizer)
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
		/*nativeBuffInitalizer(mOwnerEntity->comp<BuffableComponent>()->getFirstBuffByName
			(buffTable["name"].cast<std::string>())->getLuaReferenceToBuff());*/
		initializeBackQueue(nativeBuffInitalizer);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}

void BuffableComponent::initializeBackQueue(const BuffScript::NativeBuffInitializer& nativeBuffInitializer)
{
	if (!mQueueBuffList.empty())
		nativeBuffInitializer(*mQueueBuffList.back()->getDirectRefToBuff());
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

