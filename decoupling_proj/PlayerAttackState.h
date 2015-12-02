#pragma once
#include "PlayerState.h"
#include <SFML/Window/Event.hpp>
#include <string>

class PlayerAttackState :	public PlayerState{
public:
	PlayerAttackState(Entity* player, const sf::Vector2f& initialDir, 
		const luabridge::LuaRef& playerStateTable,
		const std::string& defaultAttackName = "Heavy_Attack_");
	virtual ~PlayerAttackState();

	virtual PlayerState* handleEvent(const sf::Event&,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* processRealTimeInput(sf::Time dt,
		const sf::RenderWindow& renderWindow);
	virtual PlayerState* update(sf::Time dt);


	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp);
private:
	std::string getAttackAnimFacingDir(const sf::Vector2f& facingDir) const;
private:
	int mCurIndex;

	bool mIsAttackCommandQueue;
	sf::Vector2f mLatestAttackDir;
	std::string mDefaultAttackName;
};

