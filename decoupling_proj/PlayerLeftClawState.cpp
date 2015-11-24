#include "PlayerLeftClawState.h"
#include "StaminaComponent.h"
#include "MeleeRectComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "PlayerIdleState.h"
#include "Utility.h"
#include "VelocityComponent.h"
#include "PlayerSideStepState.h"

PlayerLeftClawState::PlayerLeftClawState(Entity* player)
:PlayerState(player),
mLeftMouseIsStilllActivated(true),
mElapsedTime(sf::Time::Zero),
mPeriodForEachWidening(sf::seconds(0.1f)),
mIncreasedAngle(0.f),
mMaxIncreasedAngle(45.f),
mAttackInCommand(true)
{
}


PlayerLeftClawState::~PlayerLeftClawState()
{
}



PlayerState* PlayerLeftClawState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{

	return nullptr;
}

PlayerState* PlayerLeftClawState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	if (mLeftMouseIsStilllActivated && !sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		mLeftMouseIsStilllActivated = false;
		mMouseLastPos = sf::Vector2f(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
	}
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();

	if (mAttackInCommand || meleeRectComp->meleeRectIsUpdating() 
		|| meleeRectComp->isVulnerable())
		return nullptr;

	sf::Vector2f finalVelo;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		finalVelo.y = -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		finalVelo.x = -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		finalVelo.y = 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		finalVelo.x = 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		if (finalVelo == sf::Vector2f())
			finalVelo = veloComp->getFacingDirection() * -1.f;

		return new PlayerSideStepState(mPlayer, finalVelo);
	}
	if (finalVelo != sf::Vector2f())
		return new PlayerIdleState(mPlayer);

	return nullptr;
}

PlayerState* PlayerLeftClawState::update(sf::Time dt)
{
	StaminaComponent* staminaComp = mPlayer->comp<StaminaComponent>();
	if (mLeftMouseIsStilllActivated){
		mElapsedTime += dt;
		if (mElapsedTime >= mPeriodForEachWidening){
			mElapsedTime -= mPeriodForEachWidening;
			mIncreasedAngle += 2;
			if (mIncreasedAngle > mMaxIncreasedAngle) 
				mIncreasedAngle = mMaxIncreasedAngle;

			
			staminaComp->decreaseCurStamina(2.f);
			
		}
		staminaComp->setStaminaRecover(false);
		return nullptr;
	}
	
	
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();
	
	if (mAttackInCommand && !meleeRectComp->meleeRectIsUpdating()){
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
		meleeRectComp->startMeleeRect(Utility::unitVector(
			mMouseLastPos - entityWorldPos), "Left_Claw_Attack", mIncreasedAngle);
		mAttackInCommand = false;
	}

	if (!mAttackInCommand && !meleeRectComp->meleeRectIsUpdating()
		&& !meleeRectComp->isVulnerable() && !meleeRectComp->isRecovering())
		return new PlayerIdleState(mPlayer);

	return nullptr;
}

bool PlayerLeftClawState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(20);
}
