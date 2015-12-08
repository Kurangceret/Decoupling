#pragma once
#include <memory>
#include "EventType.h"

class Entity;
class EventBase{
public:
	typedef std::unique_ptr<EventBase> Ptr;
public:
	EventBase(Entity* entitySender = nullptr);
	virtual ~EventBase();

	EventType::ID getEventType() const;
	Entity* getEntityEventSender();
protected:
	EventType::ID mEventIdentifier;

private:
	Entity* mEntityEventSender;
};

