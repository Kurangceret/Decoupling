#include "EntityGotDamagedEvent.h"


EntityGotDamagedEvent::EntityGotDamagedEvent(Entity* entitySender)
:EventBase(entitySender)
{
	mEventIdentifier = EventType::EntityGotDamaged;
}


EntityGotDamagedEvent::~EntityGotDamagedEvent()
{
}
