#pragma once
#include "EventBase.h"
class PlayerHPChangedEvent :	public EventBase{
public:
	typedef std::unique_ptr<PlayerHPChangedEvent> Ptr;
public:
	PlayerHPChangedEvent();
	virtual ~PlayerHPChangedEvent();
	
	float mCurrentHP;
	float mPreviousHP;
	float mMaxHP;
};

