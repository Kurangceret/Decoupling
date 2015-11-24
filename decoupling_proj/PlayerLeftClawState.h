#pragma once
#include "PlayerState.h"
class PlayerLeftClawState :	public PlayerState{
public:
	PlayerLeftClawState(Entity* player);
	virtual ~PlayerLeftClawState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);

private:
	bool mLeftMouseIsStilllActivated;
	float mIncreasedAngle;
	float mMaxIncreasedAngle;

	sf::Time mElapsedTime;
	sf::Time mPeriodForEachWidening;

	bool mAttackInCommand;
	sf::Vector2f mMouseLastPos;
};

