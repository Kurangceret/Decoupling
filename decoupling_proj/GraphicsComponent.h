#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include "CommandQueue.h"
class GameObject;

class GraphicsComponent{
public:
	typedef std::unique_ptr<GraphicsComponent> Ptr;
public:
	GraphicsComponent();
	virtual ~GraphicsComponent();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) = 0;
	virtual void update(GameObject* gameObject, sf::Time dt, CommandQueue& commandQueue) = 0;

private:

};

