#pragma once
#include <memory>
#include <SFML/System/Time.hpp>
#include "ComponentArranger.h"
#include "ScriptAITransition.h"
#include <vector>

class Entity;

class ScriptAIState{
public:
	typedef std::unique_ptr<luabridge::LuaRef> LuaRefPtr;
	typedef std::shared_ptr<luabridge::LuaRef> LuaRefSharedPtr;
	typedef std::unique_ptr< ScriptAIState> Ptr;
	typedef  std::vector<ScriptAITransition::Ptr> TransitionsList;
	friend ComponentArranger;
public:
	ScriptAIState(LuaRefPtr statePtr);
	//ScriptAIState(luabridge::LuaRef& state);
	~ScriptAIState();

	void enterState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity);
	void updateState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity);
	void quitState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity);

	const TransitionsList& getTransitions() const;

	luabridge::LuaRef& getLuaReferenceToState(lua_State* luaState = nullptr) const;

	lua_State* getLuaStateRef() const;
	std::string getAIStateName() const;
private:
	void readTransitionsList();
	//void setNewAIState();
private:
	/*LuaRefPtr mEnterFunction;
	LuaRefPtr mUpdateFunction;
	LuaRefPtr mQuitFunction;*/

	LuaRefPtr mLuaReferenceToState;
	TransitionsList mTransitions;
	std::string mAIStateName;
};

