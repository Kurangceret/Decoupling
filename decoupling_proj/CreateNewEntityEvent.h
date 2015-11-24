#pragma once
#include "EventBase.h"
#include <queue>
#include <memory>
#include <functional>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class Entity;
//this data is for entity created from lua script
struct QueueEntityScriptData{
	typedef std::function<void(Entity*)> EngineInitializeFunc;

	QueueEntityScriptData(const std::string& layerName, const std::string& scriptDir, 
		const std::string& tableName,
		const luabridge::LuaRef* customInitializeData,
		const luabridge::LuaRef* customInitializeLuaFunc,
		const EngineInitializeFunc* customEngineInitializeFunc);

	std::string mLayer;
	std::string mScriptDir;
	std::string mTableName;

	std::shared_ptr<luabridge::LuaRef> mCustomInitializeData;
	std::shared_ptr<luabridge::LuaRef> mCustomInitializeLuaFunc;
	std::shared_ptr<EngineInitializeFunc> mEngineInitializeFunc;
};

class CreateNewEntityEvent : public EventBase{
public:
	typedef std::unique_ptr<CreateNewEntityEvent> Ptr;
public:
	CreateNewEntityEvent();
	virtual ~CreateNewEntityEvent();


	void queueEntityScript(const std::string& layerName, const std::string& scriptDir,
		const std::string& tableName, lua_State* luaState);

	void queueEntityFromEngine(const std::string& layerName, const std::string& scriptDir,
		const std::string& tableName, 
		const QueueEntityScriptData::EngineInitializeFunc* engineFunc);

	std::queue<QueueEntityScriptData> mEntitiesToBeCreated;
};

