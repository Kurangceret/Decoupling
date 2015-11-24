#pragma once
#include "System.h"
#include <SFML/Graphics/RenderTarget.hpp>

class MovementSystem :	public System{
public:
	typedef std::unique_ptr<MovementSystem> Ptr;
public:
	MovementSystem();
	virtual ~MovementSystem();


	void drawFacingDir(sf::RenderTarget& target, Entity* entity);
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
};

