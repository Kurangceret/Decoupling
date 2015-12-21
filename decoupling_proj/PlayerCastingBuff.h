#pragma once
#include "PlayerState.h"
class PlayerCastingBuff :	public PlayerState{
public:
	PlayerCastingBuff(Entity* player, luabridge::LuaRef& playerTable,
		const std::string& buffScriptDir, const std::string& buffTableName);
	virtual ~PlayerCastingBuff();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);
	virtual bool isStateAvailable();

private:
	std::string mBuffScriptDir;
	std::string mBuffTableName;

	sf::Time mElapsedTime;
	sf::Time mTimeBeforeApplyingBuff;

	bool mHasReducedSpiritCore;
};

