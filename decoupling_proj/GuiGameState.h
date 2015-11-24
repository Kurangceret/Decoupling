#pragma once
#include "StateStack.h"
#include "MeterBar.h"
#include "ContainerBar.h"

class GuiGameState :public State{
public:
	GuiGameState(StateStack& stack, Context context);
	virtual ~GuiGameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

private:
	MeterBar mHealthBar;
	//MeterBar mStaminaBar;
	ContainerBar mStaminaContainersBar;
	sf::RenderWindow& mWindow;
};

