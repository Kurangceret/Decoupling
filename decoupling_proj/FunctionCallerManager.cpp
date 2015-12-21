#include "FunctionCallerManager.h"
#include "CategoryComponent.h"
#include "Entity.h"

FunctionCallerManager* FunctionCallerManager::mFunctionCallerManager = nullptr;

FunctionCallerData::FunctionCallerData(Function& function, std::size_t entityCategory)
:mFunction(function),
mEntityCategory(entityCategory)
{

}

LuaFunctionCallerData::LuaFunctionCallerData(Entity* ownerEntity, const luabridge::LuaRef selfLuaClass,
	const luabridge::LuaRef luaFunction, std::size_t entityCategory)
:mOwnerEntity(ownerEntity),
mLuaFunction(luaFunction),
mSelfLuaClass(selfLuaClass),
mEntityCategory(entityCategory)
{

}

FunctionCallerManager::FunctionCallerManager()
{
}


FunctionCallerManager::~FunctionCallerManager()
{
}


FunctionCallerManager* FunctionCallerManager::getInstance()
{
	if (!mFunctionCallerManager)
		mFunctionCallerManager = new FunctionCallerManager();
	
	return mFunctionCallerManager;
}

void FunctionCallerManager::checkEntityForFunctionCaller(Entity* entityFromManager)
{
	if (!entityFromManager->hasComp<CategoryComponent>())
		return;

	std::size_t entityCategory = entityFromManager->comp<CategoryComponent>()->getCategory();
	
	for (auto& functionCaller : mFunctionCallerList){
		if (functionCaller->mEntityCategory & entityCategory)
			functionCaller->mFunction(entityFromManager);
	}
	for (auto& luaCaller : mLuaFunctionCallerList){
		if (luaCaller->mEntityCategory & entityCategory)
			luaCaller->mLuaFunction(luaCaller->mOwnerEntity, 
				luaCaller->mSelfLuaClass, entityFromManager);
	}
}

void FunctionCallerManager::destroyAllFunctionCaller()
{
	mFunctionCallerList.clear();
	mLuaFunctionCallerList.clear();
}

void FunctionCallerManager::insertNewFunctionCaller(FunctionCallerData& functionCallerData)
{
	mFunctionCallerList.push_back(std::make_unique<FunctionCallerData>(functionCallerData));
}

void FunctionCallerManager::insertNewLuaFunctionCaller(Entity* ownerEntity, 
	std::size_t entityCategory, lua_State* luaState)
{
	luabridge::LuaRef selfLuaClass = luabridge::LuaRef::fromStack(luaState, 4);
	luabridge::LuaRef luaFunction = luabridge::LuaRef::fromStack(luaState, 5);

	LuaFunctionCallerData::Ptr luaFunctionCaller(new 
		LuaFunctionCallerData(ownerEntity, selfLuaClass, luaFunction, entityCategory));

	/*mLuaFunctionCallerList.push_back(std::make_unique<LuaFunctionCallerData>(
		new LuaFunctionCallerData(ownerEntity, 	selfLuaClass, luaFunction, entityCategory)));*/
	mLuaFunctionCallerList.push_back(std::move(luaFunctionCaller));
}