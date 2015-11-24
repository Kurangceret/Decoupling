#include "GameState.h"


GameState::GameState(StateStack& stack, Context context)
:State(stack, context),
mGame(context.generalData, *context.window)
{
}


GameState::~GameState()
{
}


void GameState::draw()
{
	mGame.draw();
}

bool GameState::update(sf::Time dt)
{
	mGame.update(dt);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	mGame.handleEvent(event);
	if (event.type != sf::Event::KeyPressed)
		return true;

	if (event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return true;
}