#pragma once
#include "PlayerState.h"
class PlayerSideStepState :	public PlayerState{
public:
	PlayerSideStepState(Entity* player, const sf::Vector2f& initialDir, 
		const luabridge::LuaRef& playerStateTable);
	virtual ~PlayerSideStepState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);


	virtual bool isStateAvailable();
	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);

	virtual std::string getLuaTableName() const;
private:
	sf::Vector2f mCurrentSideStepDir;
	sf::Time mSideStepDur;
	sf::Time mVulnerableDur;
	sf::Time mRecoveryDur;
	float mStaminaUsage;
	sf::Vector2f mNextSideStepDir;

};

