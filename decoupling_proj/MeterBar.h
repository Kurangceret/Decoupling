#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class EventBase;

class MeterBar{
public:
	MeterBar(const sf::Color& barColor);
	~MeterBar();


	void reactToChangingValue(EventBase* eventBase);

	void setSize(const sf::Vector2f& size);
	void setSize(float width, float height);

	void drawMeter(sf::RenderTarget& target, sf::RenderStates states);

private:
	sf::RectangleShape mOuterRect;
	sf::RectangleShape mFillRect;

	
};

