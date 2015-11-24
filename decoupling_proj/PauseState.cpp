#include "PauseState.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "Utility.h"

PauseState::PauseState(StateStack& stack, Context context)
:State(stack, context),
pausedText(),
backgroundSprite(),
instructionText()
{
	sf::Font& font = *context.font;
	sf::Vector2f viewSize = context.window->getDefaultView().getSize();

	pausedText.setFont(font);
	pausedText.setString("Game Paused");
	pausedText.setCharacterSize(70);
	Utility::centerOrigin(pausedText);
	pausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	instructionText.setFont(font);
	instructionText.setString("(Press Backspace to return to the main menu)");
	Utility::centerOrigin(instructionText);
	instructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *(getContext().window);
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(sf::Vector2f(window.getSize()));

	window.draw(backgroundShape);
	window.draw(pausedText);
	window.draw(instructionText);
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape){
		// Escape pressed, remove itself to return to the game
		requestStackPop();
	}

	if (event.key.code == sf::Keyboard::BackSpace){
		requestStackClear();
		requestStackPush(States::Menu);
		getContext().generalData->clearAllData();
	}
	return false;
}

bool PauseState::update(sf::Time)
{
	return false;
}
PauseState::~PauseState()
{
}