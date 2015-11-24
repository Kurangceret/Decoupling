#pragma once
#include "EventBase.h"
class Entity;
class FireSpiritCollidedEvent :	public EventBase{
public:
	FireSpiritCollidedEvent();
	virtual ~FireSpiritCollidedEvent();

	Entity* mOwnerEntity;
	bool mIsClockwise;
};

