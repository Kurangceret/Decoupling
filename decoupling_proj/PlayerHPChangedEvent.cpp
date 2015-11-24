#include "PlayerHPChangedEvent.h"


PlayerHPChangedEvent::PlayerHPChangedEvent()
:mCurrentHP(0.f),
mMaxHP(0.f),
mPreviousHP(0.f)
{
	mEventIdentifier = EventType::PlayerHPChanged;
}


PlayerHPChangedEvent::~PlayerHPChangedEvent()
{
}
