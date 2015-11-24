#include "PlayerHookState.h"
#include "StaminaComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "PlayerIdleState.h"
#include "Utility.h"
#include "VelocityComponent.h"

PlayerHookState::PlayerHookState(Entity* player)
:PlayerState(player)
{
}


PlayerHookState::~PlayerHookState()
{
}


PlayerState* PlayerHookState::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerHookState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerHookState::update(sf::Time dt)
{
	return nullptr;
}

bool PlayerHookState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(0.f);
}