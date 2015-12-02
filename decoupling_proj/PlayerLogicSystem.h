#pragma once
#include "System.h"
#include "PlayerState.h"
#include <vector>
#include <string>


class PlayerLogicSystem :	public System{
public:
	typedef std::unique_ptr<PlayerLogicSystem> Ptr;
public:
	PlayerLogicSystem(Entity* entityPlayer, const luabridge::LuaRef& playerStateDataTable);
	virtual ~PlayerLogicSystem();

	void handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	void processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
	void checkStateValidity(PlayerState*& playerStatePointer, Entity* player);
private:
	std::unique_ptr<PlayerState> mPlayerState;
	Entity* mPlayer;
	
	luabridge::LuaRef mPlayerStateDataTable;

	int mPreviousSpiritCoreNum;
};

