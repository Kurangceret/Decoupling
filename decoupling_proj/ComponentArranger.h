#pragma once

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
class Entity;
#include "TextureStringManager.h"


class GeneralData;

//this class usually works with LUA script
//in order to create entity with different components
//and data
class ComponentArranger{
public:
	ComponentArranger(GeneralData* generalData);
	~ComponentArranger();
	
	void readFromLuaScript(Entity* entity, 	const std::string& scriptFilename, 
		const std::string& tableName, lua_State* luaState);

private:
	void readTransformableComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readSpriteComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readBoxCollisionComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readHealthComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readVelocityComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readAnimationComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readScriptUpdateComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readAutomaticPathComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readStaminaComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readMeleeRectComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readScriptAIComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readLightPointComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readCategoryComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readCircularPathComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readAvoidanceBoxComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readAttackModeComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readTextDisplayComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readHarmfulBoxesComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readSpiritFormComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readSpiritGrabberComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readRotatedBoxCollisionComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readTimerComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readFloatableComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readSpiritCoreComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readDestroyableComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
	void readBuffableComponent(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);

	void arrangeChildEntityList(Entity* entity, lua_State* luaState,
		luabridge::LuaRef& table);
private:
	TextureStringManager& mTexturesStringManager;
	GeneralData* mGeneralData;
};

