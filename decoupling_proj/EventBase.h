#pragma once
#include <memory>
#include "EventType.h"

class EventBase{
public:
	typedef std::unique_ptr<EventBase> Ptr;
public:
	EventBase();
	virtual ~EventBase();

	EventType::ID getEventType() const;
protected:
	EventType::ID mEventIdentifier;
};

