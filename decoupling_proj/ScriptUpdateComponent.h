#pragma once
#include "Component.h"
#include <functional>
#include <SFML/System/Time.hpp>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class Entity;
class ScriptUpdateComponent :	public Component{
public:
	typedef std::function < void(Entity*, sf::Time) > ScriptUpdateFunc;
public:
	ScriptUpdateComponent(Entity* entity);
	virtual ~ScriptUpdateComponent();

	//void setScriptUpdateFunc(const ScriptUpdateFunc& scriptUpdateFunc);
	void setScriptUpdateFunc(const luabridge::LuaRef& scriptUpdateFunc, lua_State* luaState);
	void setScriptMemberVariables(const luabridge::LuaRef& scriptMemberVariables);
	void runScriptUpdateFunc(sf::Time dt);

	luabridge::LuaRef getScriptMemberVariables();
private:
	//ScriptUpdateFunc mScriptUpdateFunction;
	lua_State* mLuaState;
	std::shared_ptr<luabridge::LuaRef> mScriptUpdateFunction;
	std::shared_ptr<luabridge::LuaRef> mScriptMemberVariables;
};

