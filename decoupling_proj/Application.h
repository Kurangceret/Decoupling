#pragma once

#include <SFML/Graphics.hpp>
#include "StateStack.h"

#include "GeneralData.h"

class Application{
public:
	Application();
	~Application();


	void run();
	void render();
	void processInputs();
	void update(sf::Time dt);
	void registerStates();
	void updateStatistics(sf::Time elapsedTime);
private:
	StateStack mStacks;
	sf::RenderWindow mWindow;
	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t	mStatisticsNumFrames;
	GeneralData mGeneralData;
};

