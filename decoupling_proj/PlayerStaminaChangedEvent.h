#pragma once
#include "EventBase.h"
class PlayerStaminaChangedEvent :	public EventBase{
public:
	typedef std::unique_ptr<PlayerStaminaChangedEvent> Ptr;
public:
	PlayerStaminaChangedEvent();
	virtual ~PlayerStaminaChangedEvent();

	float mCurrentStamina;
	float mMaxStamina;
	float mNumIndexer;
};

