#include "SoulHookStruckEvent.h"


SoulHookStruckEvent::SoulHookStruckEvent()
:mHookLatestPos(),
mCollidedEntityCategory(0)
{
	mEventIdentifier = EventType::SoulHookStruck;
}


SoulHookStruckEvent::~SoulHookStruckEvent()
{
}
