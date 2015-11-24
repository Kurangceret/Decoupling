#pragma once
#include "System.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>


class RenderingSystem :	public System{
public:
	RenderingSystem(sf::RenderWindow& renderWindow);
	virtual ~RenderingSystem();

protected:
	virtual void processEntity(sf::Time dt, Entity* entity);

private:
	sf::RenderWindow& mWindow;
	sf::Font mFont;
};

