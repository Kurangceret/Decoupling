#include "Application.h"
#include "Constant.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "LoadingState.h"
#include "GuiGameState.h"
#include "GeneralData.h"
#include "EventManager.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

#include <iostream>
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
:mWindow(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Close),
mFont(),
mStacks(State::Context(mWindow, mFont, mGeneralData)),
mGeneralData()
{
	EventManager::getInstance();
	mGeneralData.setLuaState(luaL_newstate());
	lua_State* luaState = mGeneralData.getLuaState();
	luaL_openlibs(luaState);

	std::string constantScript = scriptDir + "ConstantScript.lua";
	
	luaL_dofile(luaState, constantScript.c_str());
	//std::cout << luaL_dofile(luaState, "Scripts/PlayerScript.lua") << std::endl;
	//std::cout << lua_tostring(luaState, -1) << std::endl;
	int windowWidth = luabridge::getGlobal(luaState, "WindowWidth").cast<int>();
	int windowHeight = luabridge::getGlobal(luaState, "WindowHeight").cast<int>();

	mWindow.setSize(sf::Vector2u(windowWidth, windowHeight));


	mFont.loadFromFile(fontDir + "Sansation.ttf");
	registerStates(); 
	mWindow.setFramerateLimit(60);
	mStacks.pushState(States::Menu);

	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	
	
	
}


Application::~Application()
{
}


void Application::run()
{
	sf::Time limitTime = sf::Time::Zero;
	sf::Clock clock;
	sf::Time sleepTime = sf::Time::Zero;
	while (mWindow.isOpen()){
		sf::Time dt = clock.restart();
		limitTime += dt;

		/*if (TimePerFrame >= limitTime){
			sleepTime = TimePerFrame - limitTime;
			sf::sleep(sleepTime);
		}*/

		while (TimePerFrame <= limitTime){
			limitTime -= TimePerFrame;

			processInputs();
			update(TimePerFrame);
			if (mStacks.isEmpty())
				mWindow.close();
		}
		updateStatistics(dt);
		render();
	}
}

void Application::update(sf::Time dt)
{
	mStacks.update(dt);
}

void Application::processInputs()
{
	sf::Event event;
	while (mWindow.pollEvent(event)){
		mStacks.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::render()
{
	mWindow.clear();

	//std::cout << mStatisticsText.getString().toAnsiString() << std::endl;
	
	mStacks.draw();

	sf::View view = mWindow.getView();
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f, 
		view.getCenter().y - view.getSize().y / 2.f, 0.f, 0.f);

	mStatisticsText.setPosition(viewPort.left + 5.f, viewPort.top + 5.f);

	mWindow.draw(mStatisticsText);
	//mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}

void Application::registerStates()
{
	mStacks.registerState<MenuState>(States::Menu);
	mStacks.registerState<GameState>(States::Game);
	mStacks.registerState<PauseState>(States::Pause);
	mStacks.registerState<LoadingState>(States::Loading);
	mStacks.registerState<GuiGameState>(States::GuiGame);
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" //+
			/*"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us"*/);
		//mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsUpdateTime = sf::Time::Zero;
		mStatisticsNumFrames = 0;
	}
}