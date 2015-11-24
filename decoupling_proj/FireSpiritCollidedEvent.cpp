#include "FireSpiritCollidedEvent.h"


FireSpiritCollidedEvent::FireSpiritCollidedEvent()
:mIsClockwise(true),
mOwnerEntity(nullptr)
{
	mEventIdentifier = EventType::FireSpiritCollided;
}


FireSpiritCollidedEvent::~FireSpiritCollidedEvent()
{
}
