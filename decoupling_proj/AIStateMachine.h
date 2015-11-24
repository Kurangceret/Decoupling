#pragma once
#include <memory>
#include <unordered_map>
#include "ScriptAIState.h"
#include <SFML/System/Time.hpp>


class AIStateMachine{
public:
	AIStateMachine();
	~AIStateMachine();

	//void update(sf::Time dt);

private:
	//std::unordered_map<std::string, AIState::Ptr> mStates;
};

