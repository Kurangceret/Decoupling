#include "GuiGameState.h"
#include "EventManager.h"
#include "Utility.h"

#include <SFML/Graphics/Rect.hpp>
using namespace std::placeholders;

GuiGameState::GuiGameState(StateStack& stack, Context context)
:State(stack, context),
mHealthBar(sf::Color::Red),
mStaminaContainersBar(sf::Color::Yellow),
mWindow(*context.window),
mSpiritCoresCircle(sf::Color::White),
mRestoringSpiritBar(sf::Color::Yellow)
{

	mRestoringSpiritBar.setSize(sf::Vector2f(70.f, 10.f));

	EventManager* eventManager = EventManager::getInstance();
		

	eventManager->addListener(EventType::PlayerHPChanged, 
		EventCallBackFunc(Utility::convertPointerToAddress(&mHealthBar),
		std::bind(&MeterBar::reactToChangingValue, &mHealthBar, _1)));

	eventManager->addListener(EventType::SpiritCoreChanged,
		EventCallBackFunc(Utility::convertPointerToAddress(&mRestoringSpiritBar),
		std::bind(&MeterBar::reactToChangingValue, &mRestoringSpiritBar, _1)));

	eventManager->addListener(EventType::SpiritCoreChanged,
		EventCallBackFunc(Utility::convertPointerToAddress(&mSpiritCoresCircle),
		std::bind(&CircleBar::reactToChangingValue, &mSpiritCoresCircle, _1)));
	
	/*eventManager->addListener(EventType::PlayerStaminaChanged,
		EventCallBackFunc(Utility::convertPointerToAddress(&mStaminaContainersBar),
		std::bind(&ContainerBar::reactToChangingValue, &mStaminaContainersBar, _1)));*/
	
}


GuiGameState::~GuiGameState()
{
}


void GuiGameState::draw()
{
	sf::View view = mWindow.getView();
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f, 
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::RenderStates healthBarStates;
	healthBarStates.transform.translate(viewPort.left + 10.f, viewPort.top + 20.f);
	mHealthBar.drawMeter(mWindow, healthBarStates);

	sf::RenderStates circleCoreStates;
	circleCoreStates.transform.translate(viewPort.left + (viewPort.width / 2.f) - 25.f, 
		viewPort.top + (viewPort.height - 40.f));

	mSpiritCoresCircle.drawCircles(mWindow, circleCoreStates);

	sf::RenderStates restoringBarStates = circleCoreStates;
	//restoringBarStates.transform.translate(viewPort.left + 10.f, viewPort.top + 55.f);
	restoringBarStates.transform.translate(0.f, -15.f);
	mRestoringSpiritBar.drawMeter(mWindow, restoringBarStates);

	/*sf::RenderStates staminaBarStates;
	staminaBarStates.transform.translate(viewPort.left + 10.f, viewPort.top + 40.f);
	mStaminaContainersBar.drawContainers(mWindow, staminaBarStates);*/
}

bool GuiGameState::update(sf::Time dt)
{
	return true;
}

bool GuiGameState::handleEvent(const sf::Event&)
{
	return true;
}