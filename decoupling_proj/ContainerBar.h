#pragma once


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class EventBase;

class ContainerBar{
public:
	ContainerBar(const sf::Color& barColor);
	~ContainerBar();

	void reactToChangingValue(EventBase* eventBase);

	void setSize(const sf::Vector2f& size);
	void setSize(float width, float height);

	void drawContainers(sf::RenderTarget& target, sf::RenderStates states);
private:
	struct ContainerData{
		sf::RectangleShape mOuterRect;
		sf::RectangleShape mFillRect;
	};
private:
	void addNewContainer();
private:
	/*sf::RectangleShape mOuterRect;
	sf::RectangleShape mFillRect;
	*/
	sf::Color mBarColor;
	std::vector<ContainerData> mContainerList;
};

