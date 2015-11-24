#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Entity;
class StaminaComponent;

class PlayerState{
public:
	typedef std::unique_ptr<PlayerState> Ptr;
public:
	PlayerState(Entity* entityPlayer);
	~PlayerState();

	virtual PlayerState* handleEvent(const sf::Event&, 
		const sf::RenderWindow& renderWindow) = 0;
	virtual PlayerState* processRealTimeInput(sf::Time dt, 
		const sf::RenderWindow& renderWindow) = 0;

	virtual PlayerState* update(sf::Time dt) = 0;

	virtual bool isStaminaCompEnough(StaminaComponent* staminaComp) = 0;

protected:
	Entity* mPlayer;
};

