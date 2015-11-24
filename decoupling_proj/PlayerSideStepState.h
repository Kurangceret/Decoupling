#pragma once
#include "PlayerState.h"
class PlayerSideStepState :	public PlayerState{
public:
	PlayerSideStepState(Entity* player, const sf::Vector2f& initialDir);
	virtual ~PlayerSideStepState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
private:
	sf::Vector2f mCurrentSideStepDir;
	sf::Time mSideStepDur;
	sf::Time mVulnerableDur;
	sf::Time mRecoveryDur;

	sf::Vector2f mNextSideStepDir;

};

