#include "ScriptUpdateComponent.h"
#include "Entity.h"

ScriptUpdateComponent::ScriptUpdateComponent(Entity* entity)
:Component(entity),
mScriptMemberVariables(nullptr),
mScriptUpdateFunction(nullptr),
mLuaState(nullptr)
{
	//mScriptUpdateFunction = [&](Entity* entity, sf::Time dt){};
	mIdentifier = ComponentIdentifier::ScriptUpdateComponent;
}


ScriptUpdateComponent::~ScriptUpdateComponent()
{
}


/*void ScriptUpdateComponent::setScriptUpdateFunc(const ScriptUpdateFunc& scriptUpdateFunc)
{
	mScriptUpdateFunction = scriptUpdateFunc;
}*/

void ScriptUpdateComponent::setScriptUpdateFunc(const luabridge::LuaRef& scriptUpdateFunc, lua_State* luaState)
{
	mLuaState = luaState;
	mScriptUpdateFunction = std::make_shared<luabridge::LuaRef>(scriptUpdateFunc);
}

void ScriptUpdateComponent::setScriptMemberVariables(const luabridge::LuaRef& scriptMemberVariables)
{
	mScriptMemberVariables = std::make_shared<luabridge::LuaRef>(scriptMemberVariables);
}

luabridge::LuaRef ScriptUpdateComponent::getScriptMemberVariables()
{
	luabridge::Stack<luabridge::LuaRef>::push(mLuaState, *mScriptMemberVariables);
	
	return luabridge::LuaRef(mLuaState, luabridge::LuaRef::fromStack(mLuaState, 2));
	//return mScriptMemberVariables.get();
}


void ScriptUpdateComponent::runScriptUpdateFunc(sf::Time dt)
{
	//mScriptUpdateFunction(mOwnerEntity, dt);
	//luabridge::re
	try{
		if (mScriptUpdateFunction.get() && mScriptMemberVariables.get())
			(*mScriptUpdateFunction)(mOwnerEntity, dt.asSeconds(), *mScriptMemberVariables);
		else if (mScriptUpdateFunction.get() && !mScriptMemberVariables.get())
			(*mScriptUpdateFunction)(mOwnerEntity, dt.asSeconds());
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}