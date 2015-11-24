#pragma once
#include "System.h"
#include "AutomaticPathComponent.h"
class AutomaticMovementSystem :	public System{
public:
	AutomaticMovementSystem();
	virtual ~AutomaticMovementSystem();


	
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);

private:
	void smoothAutomaticPath(const sf::Vector2f& agentPos,
		const sf::FloatRect& boundingRect,
		AutomaticPathComponent::AutomaticPathList& pathList);

};

