#pragma once
#include "StateStack.h"
#include "Game.h"

class GameState :	public State{
public:
	GameState(StateStack& stack, Context context);
	virtual ~GameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

private:
	Game mGame;
};

