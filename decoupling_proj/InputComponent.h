#pragma once
#include <SFML/Window/Event.hpp>
#include "CommandQueue.h"
#include <memory>

class GameObject;

class InputComponent{
public:
	typedef std::unique_ptr<InputComponent> Ptr;
public:
	InputComponent();
	virtual ~InputComponent();

	virtual void handleEvent(const sf::Event&) = 0;

	virtual void update(GameObject* gameObject, sf::Time dt, CommandQueue& commandQueue) = 0;
};

