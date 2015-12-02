#include "PlayerRunningState.h"
#include "PlayerIdleState.h"
#include "VelocityComponent.h"
#include "Entity.h"
#include "AnimationComponent.h"
#include "PlayerIdleState.h"
#include "StaminaComponent.h"

PlayerRunningState::PlayerRunningState(Entity* entity, const luabridge::LuaRef& playerStateTable)
:PlayerState(entity, playerStateTable)
{
}


PlayerRunningState::~PlayerRunningState()
{
}


PlayerState* PlayerRunningState::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerRunningState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	VelocityComponent* velocityComp = mPlayer->nonCreateComp<VelocityComponent>();


	velocityComp->sprintOneFrame();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		velocityComp->setVelocityY(-1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		velocityComp->setVelocityX(-1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		velocityComp->setVelocityY(1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		velocityComp->setVelocityX(1.f);

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		return new PlayerIdleState(mPlayer, mPlayerStateTable);

	return nullptr;
}

bool PlayerRunningState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(0.f);
}

PlayerState* PlayerRunningState::update(sf::Time dt)
{
	std::string finalAnimName = "";

	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();

	if (veloComp->getFacingDirectionX() != 0.f)
		finalAnimName = veloComp->getFacingDirectionX() > 0.f ? "Standing_Right" : "Standing_Left";
	if (veloComp->getFacingDirectionY() != 0.f)
		finalAnimName = veloComp->getFacingDirectionY() > 0.f ? "Standing_Down" : "Standing_Up";

	if (veloComp->getVelocityX() != 0.f)
		finalAnimName = veloComp->getVelocityX() > 0.f ? "Moving_Right" : "Moving_Left";
	if (veloComp->getVelocityY() != 0.f)
		finalAnimName = veloComp->getVelocityY() > 0.f ? "Moving_Down" : "Moving_Up";


	if (finalAnimName != "")
		mPlayer->comp<AnimationComponent>()->setAnimation(finalAnimName);
	return nullptr;
}
