#pragma once
#include <string>
#include "ComponentArranger.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class ScriptAIState;

class ScriptAITransition{
public:
	typedef std::unique_ptr<ScriptAITransition> Ptr;
	typedef std::unique_ptr<luabridge::LuaRef> LuaRefPtr;
	typedef std::shared_ptr<luabridge::LuaRef> LuaRefSharedPtr;
public:
	ScriptAITransition(LuaRefPtr callTargetStateFunc,
		LuaRefPtr isTriggeredFunc);
	~ScriptAITransition();

	bool isTriggered(const luabridge::LuaRef& state, Entity* ownerEntity, Entity* playerEntity) const;
	void callTargetState(Entity* ownerEntity);

	//void setCalledTargetStateFunc(LuaRefSharedPtr state);
private:
	LuaRefPtr mCallTargetStateFunction;
	LuaRefPtr mIsTriggeredFunction;
};

