#pragma once
#include "EventBase.h"
#include <SFML/System/Vector2.hpp>

class SpiritCoreChangedEvent :	public EventBase{
public:
	typedef std::unique_ptr<SpiritCoreChangedEvent> Ptr;
public:
	SpiritCoreChangedEvent();
	virtual ~SpiritCoreChangedEvent();

	bool mIsRestoring;
	int mCurrentSpiritCores;
	float mCurRatioInRestoring;
};

