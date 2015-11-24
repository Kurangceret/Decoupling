#include "PlayerStaminaChangedEvent.h"


PlayerStaminaChangedEvent::PlayerStaminaChangedEvent()
:mCurrentStamina(0.f),
mMaxStamina(0.f)
{
	mEventIdentifier = EventType::PlayerStaminaChanged;
}


PlayerStaminaChangedEvent::~PlayerStaminaChangedEvent()
{
}
