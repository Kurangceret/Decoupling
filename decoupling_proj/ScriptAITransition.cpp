#include "ScriptAITransition.h"
#include <iostream>

ScriptAITransition::ScriptAITransition(LuaRefPtr callTargetStateFunc,
	LuaRefPtr isTriggeredFunc)
:mCallTargetStateFunction(std::move(callTargetStateFunc)),
mIsTriggeredFunction(std::move(isTriggeredFunc))
{
}


ScriptAITransition::~ScriptAITransition()
{
}


bool ScriptAITransition::isTriggered(const luabridge::LuaRef& state, 
	Entity* ownerEntity, Entity* playerEntity) const
{
	bool isTrigger = false; 
	try{
		isTrigger = (*mIsTriggeredFunction)(state, ownerEntity, playerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
	return isTrigger;
}

void ScriptAITransition::callTargetState(Entity* ownerEntity)
{
	try{
		(*mCallTargetStateFunction)(ownerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}