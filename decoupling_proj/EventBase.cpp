#include "EventBase.h"


EventBase::EventBase(Entity* entitySender)
:mEventIdentifier(EventType::None),
mEntityEventSender(entitySender)
{
}


EventBase::~EventBase()
{
}


EventType::ID EventBase::getEventType() const
{
	return mEventIdentifier;
}

Entity* EventBase::getEntityEventSender()
{
	return mEntityEventSender;
}