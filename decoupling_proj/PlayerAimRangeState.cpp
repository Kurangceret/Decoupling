#include "PlayerAimRangeState.h"
#include "MeleeRectComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "PlayerIdleState.h"
#include "Utility.h"
#include "VelocityComponent.h"
#include "PlayerSideStepState.h"
#include "SpiritCoreComponent.h"

PlayerAimRangeState::PlayerAimRangeState(Entity* player, 
	const luabridge::LuaRef& playerStateTable)
:PlayerState(player, playerStateTable),
mTimeForEachRangeBuff(sf::seconds(0.5f)),
mEachRangeBuff(48.f),
mMaximumRangeBuff(96.f),
mSpawnRangeMelee(false),
mCurrentRangeBuff(0.f),
mAttackInCommand(true),
mElapsedTime(sf::Time::Zero)
{
	mPlayer->comp<SpiritCoreComponent>()->decreaseSpiritCore(1);
}


PlayerAimRangeState::~PlayerAimRangeState()
{
}



PlayerState* PlayerAimRangeState::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerAimRangeState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	SpiritCoreComponent* spiritCoreComp = nullptr;
	if (mPlayer->hasComp<SpiritCoreComponent>())
		spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	if (!mSpawnRangeMelee && !sf::Mouse::isButtonPressed(sf::Mouse::Right) || 
		(spiritCoreComp && spiritCoreComp->noSpiritCoreLeft()))
	{
		mSpawnRangeMelee = true;
		mMouseLastPos = sf::Vector2f(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		return nullptr;
	}
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();

	if (mAttackInCommand || meleeRectComp->meleeRectIsUpdating()
		|| meleeRectComp->isVulnerable())
		return nullptr;

	sf::Vector2f finalVelo;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		finalVelo.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		finalVelo.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		finalVelo.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		finalVelo.x = 1.f;
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		if (finalVelo == sf::Vector2f())
			finalVelo = veloComp->getFacingDirection() * -1.f;

		return new PlayerSideStepState(mPlayer, finalVelo, mPlayerStateTable);
	}
	if (finalVelo != sf::Vector2f())
		return new PlayerIdleState(mPlayer, mPlayerStateTable);

	return nullptr;
}

PlayerState* PlayerAimRangeState::update(sf::Time dt)
{
	SpiritCoreComponent* spiritCoreComp = nullptr;
	if (mPlayer->hasComp<SpiritCoreComponent>())
		spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	/*if (!mSpawnRangeMelee && spiritCoreComp && spiritCoreComp->noSpiritCoreLeft())
		mSpawnRangeMelee = true;*/
	
	if (!mSpawnRangeMelee){
		mElapsedTime += dt;
		if (mElapsedTime >= mTimeForEachRangeBuff){
			mCurrentRangeBuff += mEachRangeBuff;
			if (mCurrentRangeBuff > mMaximumRangeBuff)
				mCurrentRangeBuff = mMaximumRangeBuff;

			mElapsedTime -= mTimeForEachRangeBuff;
			if (spiritCoreComp)
				spiritCoreComp->decreaseSpiritCore(1);
		}
		return nullptr;
	}

	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();

	if (mAttackInCommand && !meleeRectComp->meleeRectIsUpdating()){
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
		meleeRectComp->startMeleeRect(Utility::unitVector(
			mMouseLastPos - entityWorldPos), "Ranged_Melee_Attack", 0.f, mCurrentRangeBuff);
		mAttackInCommand = false;
	}

	if (!mAttackInCommand && !meleeRectComp->meleeRectIsUpdating()
		&& !meleeRectComp->isVulnerable() && !meleeRectComp->isRecovering())
		return new PlayerIdleState(mPlayer, mPlayerStateTable);
	
	return nullptr;
}

bool PlayerAimRangeState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return false;
}