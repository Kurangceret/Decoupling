#include "GuiGameState.h"
#include "EventManager.h"
#include "Utility.h"

#include <SFML/Graphics/Rect.hpp>
using namespace std::placeholders;

GuiGameState::GuiGameState(StateStack& stack, Context context)
:State(stack, context),
mHealthBar(sf::Color::Red),
mStaminaContainersBar(sf::Color::Yellow),
mWindow(*context.window)
{
	EventManager* eventManager = EventManager::getInstance();
		

	eventManager->addListener(EventType::PlayerHPChanged, 
		EventCallBackFunc(Utility::convertPointerToAddress(&mHealthBar),
		std::bind(&MeterBar::reactToChangingValue, &mHealthBar, _1)));

	eventManager->addListener(EventType::PlayerStaminaChanged,
		EventCallBackFunc(Utility::convertPointerToAddress(&mStaminaContainersBar),
		std::bind(&ContainerBar::reactToChangingValue, &mStaminaContainersBar, _1)));
	
}


GuiGameState::~GuiGameState()
{
}


void GuiGameState::draw()
{
	sf::View view = mWindow.getView();
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f, 
		view.getCenter().y - view.getSize().y / 2.f, 0.f, 0.f);

	sf::RenderStates healthBarStates;
	healthBarStates.transform.translate(viewPort.left + 10.f, viewPort.top + 20.f);
	mHealthBar.drawMeter(mWindow, healthBarStates);

	sf::RenderStates staminaBarStates;
	staminaBarStates.transform.translate(viewPort.left + 10.f, viewPort.top + 40.f);
	mStaminaContainersBar.drawContainers(mWindow, staminaBarStates);
}

bool GuiGameState::update(sf::Time dt)
{
	return true;
}

bool GuiGameState::handleEvent(const sf::Event&)
{
	return true;
}