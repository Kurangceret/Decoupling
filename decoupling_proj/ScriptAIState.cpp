#include "ScriptAIState.h"
#include <iostream>

ScriptAIState::ScriptAIState(LuaRefPtr statePtr)
:mLuaReferenceToState(std::move(statePtr)),
mAIStateName("")
{
	readTransitionsList();
	if (!(*mLuaReferenceToState)["name"].isNil())
		mAIStateName = (*mLuaReferenceToState)["name"].cast<std::string>();
}


ScriptAIState::~ScriptAIState()
{
}


void ScriptAIState::enterState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity)
{
	try{
		(*mLuaReferenceToState)["enter"](*mLuaReferenceToState, ownerEntity, dt.asSeconds(), playerEntity);
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void ScriptAIState::updateState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity)
{
	try{
		(*mLuaReferenceToState)["update"](*mLuaReferenceToState, ownerEntity, dt.asSeconds(), playerEntity);
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

void ScriptAIState::quitState(Entity* ownerEntity, sf::Time dt, Entity* playerEntity)
{
	try{
		(*mLuaReferenceToState)["quit"](*mLuaReferenceToState, ownerEntity, dt.asSeconds(), playerEntity);
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}
}

std::string ScriptAIState::getAIStateName() const
{
	return mAIStateName;
}

void ScriptAIState::readTransitionsList()
{
	luabridge::LuaRef& state = *mLuaReferenceToState.get();

	luabridge::LuaRef transitions = state["Transitions"];
	int i = 1;
	while (!transitions[i].isNil()){

		luabridge::LuaRef curArray = transitions[i].cast<luabridge::LuaRef>();

		/*ScriptAITransition::LuaRefSharedPtr callTarget =
			std::make_shared<luabridge::LuaRef>(
			curArray["callTargetState"].cast<luabridge::LuaRef>());

		ScriptAITransition::LuaRefSharedPtr isTriggered =
			std::make_shared<luabridge::LuaRef>(
			curArray["isTriggered"].cast<luabridge::LuaRef>());*/

		ScriptAITransition::LuaRefPtr callTarget =
			std::make_unique<luabridge::LuaRef>(
			curArray["callTargetState"].cast<luabridge::LuaRef>());

		ScriptAITransition::LuaRefPtr isTriggered =
			std::make_unique<luabridge::LuaRef>(
			curArray["isTriggered"].cast<luabridge::LuaRef>());

		ScriptAITransition::Ptr transition(new ScriptAITransition(std::move(callTarget), std::move(isTriggered)));
		mTransitions.push_back(std::move(transition));
		i++;
	}
}

const std::vector<ScriptAITransition::Ptr>& ScriptAIState::getTransitions() const
{
	return mTransitions;
}

/*ScriptAIState::LuaRefSharedPtr*/
luabridge::LuaRef& ScriptAIState::getLuaReferenceToState(lua_State* luaState) const
{
	/*luabridge::LuaRef::push(luaState);
	luabridge::Stack<luabridge::LuaRef>::push(luaState, *mLuaReferenceToState);*/
	/*if (luaState){
		//mLuaReferenceToState->push(luaState);
		
		luabridge::LuaRef topFuncRef =  luabridge::LuaRef::fromStack(luaState, 2);
		topFuncRef.push(topFuncRef.state());

		luabridge::Stack<luabridge::LuaRef>::push(topFuncRef.state(), *mLuaReferenceToState);
		try{
			luabridge::LuaException::pcall(topFuncRef.state(), 2, 1);
		}
		catch (luabridge::LuaException e)
		{
			std::cout << e.what() << std::endl;
		}
	}*/
	return *mLuaReferenceToState;
}

lua_State* ScriptAIState::getLuaStateRef() const
{
	//mLuaReferenceToState->push(mLuaReferenceToState->state());
	return mLuaReferenceToState->state();
}