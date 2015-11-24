#include "EventBase.h"


EventBase::EventBase()
:mEventIdentifier(EventType::None)
{

}


EventBase::~EventBase()
{
}


EventType::ID EventBase::getEventType() const
{
	return mEventIdentifier;
}