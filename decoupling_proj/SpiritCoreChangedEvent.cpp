#include "SpiritCoreChangedEvent.h"


SpiritCoreChangedEvent::SpiritCoreChangedEvent()
:mIsRestoring(false),
mCurrentSpiritCores(0),
mCurRatioInRestoring(0.f)
{
	mEventIdentifier = EventType::SpiritCoreChanged;
}


SpiritCoreChangedEvent::~SpiritCoreChangedEvent()
{
}
