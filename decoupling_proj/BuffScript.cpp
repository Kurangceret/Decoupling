#include "BuffScript.h"
#include <iostream>

BuffScript::LuaBuffInitializer BuffScript::mEmptyLuaInitializer = nullptr;

BuffScript::NativeBuffInitializer BuffScript::mEmptyNativeInitializer = 
[](const luabridge::LuaRef& table)
{

};

BuffScript::BuffScript(LuaRefPtr luaRef, const NativeBuffInitializer& nativeBuffInitalizer, LuaBuffInitializer luaBuffInitializer)
:mLuaReferenceToBuff(std::move(luaRef)),
mBuffName(""),
mUseBuffTimer(false),
mBuffTimer(sf::seconds(0.1f)),
mAbsoluteDestroy(false)
{
	luabridge::LuaRef luaTable = *mLuaReferenceToBuff;
	if (!luaTable["name"].isNil())
		mBuffName = luaTable["name"].cast<std::string>();
	if (!luaTable["mUseBuffTimer"].isNil())
		mUseBuffTimer = luaTable["mUseBuffTimer"].cast<bool>();
	if (!luaTable["mBuffTimer"].isNil())
		mBuffTimer = sf::seconds(luaTable["mBuffTimer"].cast<float>());

	nativeBuffInitalizer(*mLuaReferenceToBuff);
	if (luaBuffInitializer.get())
		(*luaBuffInitializer)(*mLuaReferenceToBuff);
}


BuffScript::~BuffScript()
{
}


void BuffScript::enterBuff(Entity* ownerEntity, sf::Time dt)
{
	try{
		(*mLuaReferenceToBuff)["enter"](*mLuaReferenceToBuff, ownerEntity, dt.asSeconds());
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void BuffScript::updateBuff(Entity* ownerEntity, sf::Time dt)
{
	if (mUseBuffTimer)
		mBuffTimer -= dt;

	try{
		(*mLuaReferenceToBuff)["update"](*mLuaReferenceToBuff, ownerEntity, dt.asSeconds());
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void BuffScript::exitBuff(Entity* ownerEntity, sf::Time dt)
{
	try{
		(*mLuaReferenceToBuff)["exit"](*mLuaReferenceToBuff, ownerEntity, dt.asSeconds());
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void BuffScript::absoluteDestroy()
{
	mAbsoluteDestroy = true;
}

bool BuffScript::isBuffOver(Entity* ownerEntity) const
{
	if (mAbsoluteDestroy)
		return true;

	bool finalFlag = true;
	try{
		bool luaFlag = (*mLuaReferenceToBuff)["isBuffOver"](*mLuaReferenceToBuff, ownerEntity);

		if (mUseBuffTimer)	(finalFlag = isBuffTimerOver() || luaFlag);
		else finalFlag = luaFlag;
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}

	return finalFlag;
}


std::string BuffScript::getBuffName() const
{
	return mBuffName;
}

bool BuffScript::isBuffTimerOver() const
{
	return mBuffTimer <= sf::Time::Zero;
}

luabridge::LuaRef* BuffScript::getDirectRefToBuff()
{
	return mLuaReferenceToBuff.get();
}

luabridge::LuaRef BuffScript::getLuaReferenceToBuff()
{
	lua_State* luaState = mLuaReferenceToBuff.get()->state();

	luabridge::Stack<luabridge::LuaRef>::push(luaState, *mLuaReferenceToBuff);

	return luabridge::LuaRef(luaState, luabridge::LuaRef::fromStack(luaState, 2));
}