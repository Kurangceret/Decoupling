#pragma once
#include "Component.h"
#include <memory>
#include <unordered_map>
#include "ScriptAIState.h"
#include <SFML/System/Time.hpp>
#include "ComponentArranger.h"

class CollisionHandlerSystem;

class ScriptAIComponent : public Component{
public:
	friend ComponentArranger;
	friend CollisionHandlerSystem;
public:
	ScriptAIComponent(Entity* entity);
	virtual ~ScriptAIComponent();

	void update(sf::Time dt, Entity* playerEntity);
	void insertNewStateToManage(const std::string& stateName,
		ScriptAIState::Ptr state);

	void setCurrentState(ScriptAIState* state);
	void setCurrentState(ScriptAIState::Ptr state);

	/*exclusive to script usage*/
	void setCurState(lua_State* luaState);
	/**/
	std::string getCurAIStateName() const;
	ScriptAIState* getCurAIState() const;
	//luabridge::LuaRef& getCurAIState() const;

	//float getRangeCheckingToPlayer() const;
private:
	std::unordered_map<std::string, ScriptAIState::Ptr> mStates;
	ScriptAIState::Ptr mCurrentState;
	ScriptAIState::Ptr mQueueAIState;
	lua_State* mLuaState;

	bool mUsePlayerFoundSystem;
	bool mPlayerIsFound;
	float mRangeCheckingToTarget;
	//uses in conjunction with mUsePlayerFoundSystem
	std::unique_ptr<luabridge::LuaRef> mLuaTileChecker;
};

