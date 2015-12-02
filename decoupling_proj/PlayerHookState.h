#pragma once
#include "PlayerState.h"

class EventBase;
class PlayerHookState :	public PlayerState{
public:
	PlayerHookState(Entity* player, const luabridge::LuaRef& playerStateTable);
	virtual ~PlayerHookState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);

	virtual PlayerState* update(sf::Time dt);

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
private:
	void reactToSoulHookEvent(EventBase* eventBase);

private:
	bool mEndThisState;
	bool mSpawnedHook;
	bool mIsHookingPlayer;
	sf::Vector2f mHookLatestPos;
	sf::Vector2f mHookingDir;

	sf::Time mHoldRightMouseTime;
	sf::Time mElapsedTime;
	sf::Time mPeriodDecreaseStamina;

	float mOriginalSpeed;
	float mHookSpeed;
};

