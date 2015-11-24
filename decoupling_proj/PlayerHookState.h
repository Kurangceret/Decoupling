#pragma once
#include "PlayerState.h"
class PlayerHookState :	public PlayerState{
public:
	PlayerHookState(Entity* player);
	virtual ~PlayerHookState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
private:

};

