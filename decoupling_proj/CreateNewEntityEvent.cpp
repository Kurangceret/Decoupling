#include "CreateNewEntityEvent.h"


QueueEntityScriptData::QueueEntityScriptData(const std::string& layerName,
	const std::string& scriptDir, const std::string& tableName,
	const luabridge::LuaRef* customInitializeData,
	const luabridge::LuaRef* customInitializeLuaFunc,
	const EngineInitializeFunc* customEngineInitializeFunc)
	:mLayer(layerName),
	mScriptDir(scriptDir),
	mTableName(tableName),
	mCustomInitializeData(nullptr),
	mCustomInitializeLuaFunc(nullptr),
	mEngineInitializeFunc(nullptr)
{
	if (customInitializeData)
		mCustomInitializeData = std::make_shared<luabridge::LuaRef>(*customInitializeData);
	if (customInitializeLuaFunc)
		mCustomInitializeLuaFunc = std::make_shared<luabridge::LuaRef>(*customInitializeLuaFunc);
	if (customEngineInitializeFunc)
		mEngineInitializeFunc = std::make_shared<EngineInitializeFunc>(*customEngineInitializeFunc);
}

CreateNewEntityEvent::CreateNewEntityEvent()
{
	mEventIdentifier = EventType::CreateNewEntity;
}


CreateNewEntityEvent::~CreateNewEntityEvent()
{
}


void CreateNewEntityEvent::queueEntityScript(const std::string& layerName, 
	const std::string& scriptDir,
	const std::string& tableName, lua_State* luaState)
{
	luabridge::LuaRef luaData = luabridge::LuaRef::fromStack(luaState, 5);
	luabridge::LuaRef luaFunc = luabridge::LuaRef::fromStack(luaState, 6);

	mEntitiesToBeCreated.push(QueueEntityScriptData(layerName, scriptDir, tableName, &luaData, &luaFunc, nullptr));
}

void CreateNewEntityEvent::queueEntityFromEngine(const std::string& layerName, const std::string& scriptDir,
	const std::string& tableName, 
	const QueueEntityScriptData::EngineInitializeFunc* engineFunc)
{
	
	mEntitiesToBeCreated.push(QueueEntityScriptData(layerName, scriptDir, tableName, nullptr, nullptr, 
		engineFunc));
}

