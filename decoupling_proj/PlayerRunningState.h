#pragma once
#include "PlayerState.h"
class PlayerRunningState :	public PlayerState{
public:
	PlayerRunningState(Entity* entity, const luabridge::LuaRef& playerStateTable);
	virtual ~PlayerRunningState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
};

