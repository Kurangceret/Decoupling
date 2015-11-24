#include "PlayerSideStepState.h"
#include "Entity.h"
#include "VelocityComponent.h"
#include "AnimationComponent.h"
#include "PlayerIdleState.h"
#include "PlayerAttackState.h"
#include "StaminaComponent.h"
#include "AttackModeComponent.h"
#include "TransformableComponent.h"
#include "Utility.h"
#include "PlayerSpiritState.h"

const float sideStepDur = 0.1f;
const float vulnerableDur = 0.2f;
const float recoveryDur = 0.0f;
const float staminaUsage = 20.f;

PlayerSideStepState::PlayerSideStepState(Entity* player, const sf::Vector2f& initialDir)
:PlayerState(player),
mCurrentSideStepDir(0.f, 0.f),
mSideStepDur(sf::seconds(sideStepDur)),
mRecoveryDur(sf::seconds(recoveryDur)),
mVulnerableDur(sf::seconds(vulnerableDur)),
mNextSideStepDir(initialDir)
{
}


PlayerSideStepState::~PlayerSideStepState()
{
}


PlayerState* PlayerSideStepState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	if (mSideStepDur.asSeconds() <= 0.f && event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Space)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			mNextSideStepDir.x = -1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			mNextSideStepDir.y = -1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			mNextSideStepDir.y = 1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			mNextSideStepDir.x = 1.f;
	}
	
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left)
	{
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		veloComp->setSpeedIdentifier(1.f);

		AttackModeComponent* attackModeComp = mPlayer->comp<AttackModeComponent>();

		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerAttackState(mPlayer, Utility::unitVector(mousePos - entityWorldPos),
			attackModeComp->getCurrentStringIndex());
	}
	
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerSpiritState(mPlayer, Utility::unitVector(mousePos - entityWorldPos));
	}

	/*if (event.key.code == sf::Keyboard::Up){
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		AttackModeComponent* attackModeComp = mPlayer->comp<AttackModeComponent>();

		veloComp->setSpeedIdentifier(1.f);
		return new PlayerAttackState(mPlayer, veloComp->getFacingDirection(), attackModeComp->getCurrentStringIndex());
	}*/

	return nullptr;
}

PlayerState* PlayerSideStepState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	/*if (mSideStepDur.asSeconds() > 0.f)
		return nullptr;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		return nullptr;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		mNextSideStepDir.x = -1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		mNextSideStepDir.y = -1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		mNextSideStepDir.y = 1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		mNextSideStepDir.x = 1.f;*/

	return nullptr;
}

bool PlayerSideStepState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(staminaUsage);
}

PlayerState* PlayerSideStepState::update(sf::Time dt)
{
	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
	mSideStepDur -= dt;

	if (mCurrentSideStepDir != sf::Vector2f() && mSideStepDur.asSeconds() > 0.f)
	{
		veloComp->setVelocity(mCurrentSideStepDir, false);
		veloComp->setSpeedIdentifier(4.f);
		return nullptr;
	}
	mCurrentSideStepDir = sf::Vector2f();
	if (mCurrentSideStepDir != sf::Vector2f() && mVulnerableDur.asSeconds() > 0.f)
	{
		mVulnerableDur -= dt;
		return nullptr;
	}

	mCurrentSideStepDir = sf::Vector2f();

	if (mCurrentSideStepDir == sf::Vector2f() && mNextSideStepDir != sf::Vector2f()){
		StaminaComponent* staminaComp = mPlayer->nonCreateComp<StaminaComponent>();

		if (staminaComp && !staminaComp->checkDecreaseStamina(staminaUsage)){
			mNextSideStepDir = sf::Vector2f();
			return nullptr;
		}

		staminaComp->decreaseCurStamina(staminaUsage, true);

		mSideStepDur = sf::seconds(sideStepDur);
		mRecoveryDur = sf::seconds(recoveryDur);
		mVulnerableDur = sf::seconds(vulnerableDur);

		mCurrentSideStepDir = mNextSideStepDir;
		mNextSideStepDir = sf::Vector2f();

		//return new PlayerSideStepState(mPlayer, mNextSideStepDir);
		return nullptr;
	}

	if (mRecoveryDur.asSeconds() > 0.f){
		mRecoveryDur -= dt;
		return nullptr;
	}


	veloComp->setSpeedIdentifier(1.f);
	return new PlayerIdleState(mPlayer);
}
