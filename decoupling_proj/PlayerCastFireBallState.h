#pragma once
#include "PlayerState.h"
class PlayerCastFireBallState :	public PlayerState{
public:
	PlayerCastFireBallState(Entity* player, const luabridge::LuaRef& playerTable, 
		const sf::Vector2f& initialDir);
	virtual ~PlayerCastFireBallState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);
	virtual bool isStateAvailable();

private:
	sf::Time mCastingTime;
	sf::Time mElapsedCastingTime;

	sf::Vector2f mProjectileDir;
	bool mFireBallSpawned;

	sf::Time mRecoveryDur;
	sf::Time mElapsedRecoveryTime;
};

