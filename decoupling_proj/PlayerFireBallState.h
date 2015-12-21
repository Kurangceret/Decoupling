#pragma once
#include "PlayerState.h"
class PlayerFireBallState :	public PlayerState{
public:
	PlayerFireBallState(Entity* player, const luabridge::LuaRef& playerData);
	virtual ~PlayerFireBallState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);
	virtual bool isStateAvailable();
private:
	sf::Time mTimeForEachTimeBuff;
	sf::Time mElapsedTime;

	float mEachTimeBuff;
	float mCurrentTimeBuff;
	float mMaximumTimeBuff;

	sf::Vector2f mMouseLastPos;
	bool mRightMouseReleased;
	bool mSpawnedFireBall;

	sf::Time mRecoveryDur;
	sf::Time mElapsedRecoveryTime;

	bool mHasReducedSpiritCore;
};

