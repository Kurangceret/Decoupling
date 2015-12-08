#include "ScriptAIComponent.h"
#include "TransformableComponent.h"
#include "RayCast.h"
#include "Entity.h"
#include "PathFinder.h"
#include "Utility.h"
#include <iostream>

ScriptAIComponent::ScriptAIComponent(Entity* entity)
:Component(entity),
mCurrentState(nullptr),
mUsePlayerFoundSystem(true),
mPlayerIsFound(false),
mRangeCheckingToTarget(200.f),
mLuaState(nullptr)
{
	mIdentifier = ComponentIdentifier::ScriptAIComponent;
}


ScriptAIComponent::~ScriptAIComponent()
{
}


void ScriptAIComponent::update(sf::Time dt, Entity* playerEntity)
{
	if (!mCurrentState.get() && !mQueueAIState.get())
		return;
	
	if (mUsePlayerFoundSystem && !mPlayerIsFound && 
		mOwnerEntity->hasComp<TransformableComponent>())
	{
		TransformableComponent* ownerTransformComp = mOwnerEntity->comp<TransformableComponent>();

		sf::Vector2f ownerWorldPos = ownerTransformComp->getWorldPosition(true);
		sf::Vector2f playerWorldPos = playerEntity->comp<TransformableComponent>()->getWorldPosition(true);

		if (Utility::vectorLength(playerWorldPos - ownerWorldPos) > mRangeCheckingToTarget)
			return;

		mPlayerIsFound = RayCast::castRayLine(ownerTransformComp->getWorldPosition(true), 
			playerEntity->comp<TransformableComponent>()->getWorldPosition(true),
			PathFinder::getInstance());
		return;
	}

	if (!mCurrentState.get() && mQueueAIState.get()){
		mCurrentState.reset(mQueueAIState.release());
		mCurrentState->enterState(mOwnerEntity, dt, playerEntity);
		return;
	}

	//std::cout << mCurrentState->getAIStateName() << std::endl;

	ScriptAITransition* transition = nullptr;

	for (const auto& transitionPtr : mCurrentState->getTransitions()){
		if (transitionPtr->isTriggered(
			mCurrentState->getLuaReferenceToState(), mOwnerEntity, playerEntity))
			transition = transitionPtr.get();
	}
	
	if (transition){
		mCurrentState->quitState(mOwnerEntity, dt, playerEntity);
		transition->callTargetState(mOwnerEntity);
		transition = nullptr;
		mCurrentState.reset(mQueueAIState.release());
		mQueueAIState = nullptr;
		mCurrentState->enterState(mOwnerEntity, dt, playerEntity);
		return;
	}

	mCurrentState->updateState(mOwnerEntity, dt, playerEntity);
	
}

void ScriptAIComponent::insertNewStateToManage(const std::string& stateName,
	ScriptAIState::Ptr state)
{
	mStates.insert(std::make_pair(stateName, std::move(state)));
}

void ScriptAIComponent::setCurState(lua_State* luaState)
{
	try{
		luabridge::LuaRef param = luabridge::LuaRef::fromStack(luaState, 2);


		//std::cout << param.tostring() << std::endl;
		if (param.isTable())
			setCurrentState(new ScriptAIState(std::make_unique<luabridge::LuaRef>(param)));
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void ScriptAIComponent::setCurrentState(ScriptAIState* state)
{
	/*if (!mCurrentState.get()){
		mCurrentState.reset(state);
		return;
	}*/
	// we queue the generated state
	//and call it later, since the script is still going on this call
	mQueueAIState.reset(state); 
	
}

void ScriptAIComponent::setCurrentState(ScriptAIState::Ptr state)
{
	mCurrentState = std::move(state);
}

std::string ScriptAIComponent::getCurAIStateName() const
{
	if (mCurrentState.get())
		return mCurrentState->getAIStateName();

	return "";
}

/*const luabridge::LuaRef* ScriptAIComponent::getCurLuaState() const
{
	if (mCurrentState.get())
		return &mCurrentState->getLuaReferenceToState();

	return nullptr;
}*/

/*luabridge::LuaRef& ScriptAIComponent::getCurAIState() const
{
	if (mCurrentState.get())
		return mCurrentState.get();
	return nullptr;
	//return mCurrentState->getLuaReferenceToState(mLuaState);
}*/

ScriptAIState* ScriptAIComponent::getCurAIState() const
{
	if (mCurrentState.get())
		return mCurrentState.get();
	return nullptr;
	//return mCurrentState->getLuaReferenceToState(mLuaState);
}