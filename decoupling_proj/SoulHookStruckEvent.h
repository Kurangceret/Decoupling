#pragma once
#include "EventBase.h"
#include <SFML/System/Vector2.hpp>

class SoulHookStruckEvent :	public EventBase{
public:
	SoulHookStruckEvent();
	virtual ~SoulHookStruckEvent();
		
	sf::Vector2f mHookLatestPos;
	std::size_t mCollidedEntityCategory;
};

