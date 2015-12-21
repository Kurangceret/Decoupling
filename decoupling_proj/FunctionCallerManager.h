#pragma once
#include <vector>
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
struct FunctionCallerData{
	typedef std::unique_ptr<FunctionCallerData> Ptr;
	typedef std::function<void(Entity*)> Function;
	FunctionCallerData(Function& function, std::size_t entityCategory);
	//first arg is indexed entity from EntityManager
	Function mFunction;
	//what category that is applicable to mFunction
	std::size_t mEntityCategory;
};

struct LuaFunctionCallerData{
	typedef std::unique_ptr<LuaFunctionCallerData> Ptr;

	LuaFunctionCallerData(Entity* ownerEntity, const luabridge::LuaRef selfLuaClass, 
		const luabridge::LuaRef luaFunction, std::size_t entityCategory);

	//first arg is thisEntity(the one who send this data)
	//second arg is selfLuaClass(the upper/owner of the mLuaFunction(typically holds data)
	//third arg is indexed entity from EntityManager
	const luabridge::LuaRef mLuaFunction;

	const luabridge::LuaRef mSelfLuaClass;
	//what category that is applicable to mLuaFunction
	std::size_t mEntityCategory;
	Entity* mOwnerEntity;
};

class FunctionCallerManager{
public:
	~FunctionCallerManager();

	static FunctionCallerManager* getInstance();
	void checkEntityForFunctionCaller(Entity* entityFromManager);
	void destroyAllFunctionCaller();
	void insertNewFunctionCaller(FunctionCallerData& functionCallerData);
	void insertNewLuaFunctionCaller(Entity* ownerEntity, std::size_t entityCategory, lua_State* luaState);
private:
	FunctionCallerManager();
		
private:
	static FunctionCallerManager* mFunctionCallerManager;
	
	std::vector<FunctionCallerData::Ptr> mFunctionCallerList;
	std::vector<LuaFunctionCallerData::Ptr> mLuaFunctionCallerList;
};


