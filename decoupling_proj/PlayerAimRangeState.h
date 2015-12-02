#pragma once
#include "PlayerState.h"
class PlayerAimRangeState :	public PlayerState{
public:
	PlayerAimRangeState(Entity* player, 
		const luabridge::LuaRef& playerStateTable);
	virtual ~PlayerAimRangeState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);

private:
	sf::Time mTimeForEachRangeBuff;
	sf::Time mElapsedTime;
	float mEachRangeBuff;
	float mCurrentRangeBuff;
	float mMaximumRangeBuff;

	bool mSpawnRangeMelee;
	bool mAttackInCommand;
	sf::Vector2f mMouseLastPos;

};

