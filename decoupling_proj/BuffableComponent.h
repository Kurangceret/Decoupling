#pragma once
#include "Component.h"
#include "BuffScript.h"
#include <queue>
#include <string>

class ComponentArranger;

class BuffableComponent :	public Component{
public:
	friend ComponentArranger;
public:
	BuffableComponent(Entity* entity);
	virtual ~BuffableComponent();

	void update(sf::Time dt);
	
	void insertNewBuff(BuffScript* buffPtr);
	void insertNewBuffFromScript(lua_State* luaState);

	void insertBuffWithScriptName(const std::string& scriptName, 
		const std::string& tableName, 
		const BuffScript::NativeBuffInitializer& nativeBuffInitalizer = BuffScript::mEmptyNativeInitializer);

	void destroyAllBuff();

	BuffScript* getFirstBuffByName(const std::string& buffName);
private:
	//call this function before update(sf::Time dt) func is called
	void initializeBackQueue(const BuffScript::NativeBuffInitializer& nativeBuffInitalizer);
private:
	std::vector<BuffScript::Ptr> mBuffScriptList;
	std::queue<BuffScript::Ptr> mQueueBuffList;
	lua_State* mLuaState;
};

