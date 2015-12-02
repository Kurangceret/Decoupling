#pragma once
#include "PlayerState.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class PlayerSpiritState :	public PlayerState{
public:
	PlayerSpiritState(Entity* player, const sf::Vector2f& direction, const luabridge::LuaRef& playerStateTable);
	virtual ~PlayerSpiritState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);

private:
	sf::Time mDelayTime;
	sf::Vector2f mSpiritDirection;
	sf::Time mSpiritLifeTime;
};

