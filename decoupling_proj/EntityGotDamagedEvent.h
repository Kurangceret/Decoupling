#pragma once
#include "EventBase.h"
class EntityGotDamagedEvent :public EventBase{
public:
	EntityGotDamagedEvent(Entity* entitySender);
	virtual ~EntityGotDamagedEvent();
		
};

