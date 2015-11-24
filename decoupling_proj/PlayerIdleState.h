#pragma once
#include "PlayerState.h"
class PlayerIdleState :	public PlayerState{
public:
	PlayerIdleState(Entity* playerEntity);
	virtual ~PlayerIdleState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
};

