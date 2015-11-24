#include "MenuState.h"
#include "Utility.h"

MenuState::MenuState(StateStack& stack, Context context)
:State(stack, context), mOptionIndex(0)
{
	sf::Text playText;
	
	playText.setString("Play");
	playText.setFont(*context.font);
	Utility::centerOrigin(playText);
	playText.setPosition(context.window->getView().getSize() / 2.f);
	mTexts.push_back(playText);

	sf::Text exitText;
	exitText.setString("Exit");
	exitText.setFont(*context.font);
	Utility::centerOrigin(exitText);
	exitText.setPosition(playText.getPosition().x, playText.getPosition().y + 30.f);
	mTexts.push_back(exitText);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	for (sf::Text& text : mTexts)
		window.draw(text);
		
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Return){
		if (mOptionIndex == Play){
			requestStackPop();
			requestStackPush(States::Loading);
		}
		else if (mOptionIndex == Exit){
			// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up){
		// Decrement and wrap-around
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mTexts.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down){
		// Increment and wrap-around
		if (mOptionIndex < mTexts.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		updateOptionText();
	}

	return true;
}
void MenuState::updateOptionText()
{
	if (mTexts.empty())
		return;

	// White all texts
	for (sf::Text& text : mTexts)
		text.setColor(sf::Color::White);

	// Red the selected text
	mTexts[mOptionIndex].setColor(sf::Color::Red);
}
MenuState::~MenuState()
{
}
