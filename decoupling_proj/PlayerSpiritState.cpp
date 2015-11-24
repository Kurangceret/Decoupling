#include "PlayerSpiritState.h"
#include "Entity.h"
#include "SpiritFormComponent.h"
#include "PlayerIdleState.h"
#include "StaminaComponent.h"
#include "VelocityComponent.h"

PlayerSpiritState::PlayerSpiritState(Entity* player, const sf::Vector2f& direction)
:PlayerState(player),
mSpiritDirection(direction),
mSpiritLifeTime(player->comp<SpiritFormComponent>()->getNormalSpiritLifeTime()),
mDelayTime(sf::seconds(player->comp<SpiritFormComponent>()->mDelayBeforeSpirit))
{
}


PlayerSpiritState::~PlayerSpiritState()
{
}


PlayerState* PlayerSpiritState::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerSpiritState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerSpiritState::update(sf::Time dt)
{
	if (mDelayTime.asSeconds() > 0.f){
		mDelayTime -= dt;
		return nullptr;
	}

	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
	SpiritFormComponent* spiritComp = mPlayer->comp<SpiritFormComponent>();

	if (!spiritComp->isOnSpiritForm())
		spiritComp->enterSpiritForm();

	mSpiritLifeTime -= dt;

	if (mSpiritLifeTime.asSeconds() <= 0.0f){
		veloComp->setVelocity(0.f, 0.f);
		spiritComp->quitSpiritForm();
		return new PlayerIdleState(mPlayer);
	}
	veloComp->setVelocity(mSpiritDirection);

	return nullptr;
}

bool PlayerSpiritState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(0);
}