#include "PlayerAttackState.h"
#include "VelocityComponent.h"
#include "AnimationComponent.h"
#include "Entity.h"
#include "MeleeRectComponent.h"
#include "PlayerIdleState.h"
#include "PlayerSideStepState.h"
#include "StaminaComponent.h"
#include "TransformableComponent.h"
#include "Utility.h"
#include "PlayerSpiritState.h"

PlayerAttackState::PlayerAttackState(Entity* player, const sf::Vector2f& initialDir,
	const std::string& defaultAttackName)
:PlayerState(player),
mCurIndex(0),
mIsAttackCommandQueue(true),
mLatestAttackDir(initialDir),
mDefaultAttackName(defaultAttackName)
{
	/*MeleeRectComponent* meleeRectComp =  player->comp<MeleeRectComponent>();
	VelocityComponent* veloComp = player->comp<VelocityComponent>();
	meleeRectComp->startMeleeRect(veloComp->getFacingDirection(), "Light_Attack_1");
	mCurIndex++;*/
}


PlayerAttackState::~PlayerAttackState()
{
}


PlayerState* PlayerAttackState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();

	if (meleeRectComp->meleeRectIsDelaying() || 
		mIsAttackCommandQueue)
		return nullptr;

	/*if (event.key.code == sf::Keyboard::Up)
		mIsAttackCommandQueue = true;*/
	
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left)
	{
		mIsAttackCommandQueue = true;
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		mLatestAttackDir = Utility::unitVector(mousePos - entityWorldPos);
		return nullptr;
	}

	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerSpiritState(mPlayer, Utility::unitVector(mousePos - entityWorldPos));
	}


	return nullptr;
}

PlayerState* PlayerAttackState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();
	if (meleeRectComp->meleeRectIsDelaying() ||	mIsAttackCommandQueue)
		return nullptr;

	sf::Vector2f finalVelo;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		//mLatestAttackDir = sf::Vector2f(0.f, -1.f);
		//mLatestAttackDir.y = -1.f;
		finalVelo.y = -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		//mLatestAttackDir = sf::Vector2f(-1.f, 0.f);
		//mLatestAttackDir.x = -1.f;
		finalVelo.x = -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		//mLatestAttackDir = sf::Vector2f(0.f, 1.f);
		//mLatestAttackDir.y = 1.f;
		finalVelo.y = 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		//mLatestAttackDir = sf::Vector2f(1.f, 0.f);
		//mLatestAttackDir.x = 1.f;
		finalVelo.x = 1.f;
	}

	if (meleeRectComp->meleeRectIsUpdating() || meleeRectComp->isVulnerable())
		return nullptr;
	
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

bool PlayerAttackState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(20.f);
}

PlayerState* PlayerAttackState::update(sf::Time dt)
{
	MeleeRectComponent* meleeRectComp = mPlayer->comp<MeleeRectComponent>();
	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
	sf::Vector2f facingDir = veloComp->getFacingDirection();

	std::string nextAnimName = getAttackAnimFacingDir(facingDir);

	if (mIsAttackCommandQueue && !meleeRectComp->meleeRectIsUpdating() 
		&& !meleeRectComp->isVulnerable())
	{
		
		if (mLatestAttackDir != sf::Vector2f()){
			facingDir = mLatestAttackDir;
			mLatestAttackDir = sf::Vector2f(0.f, 0.f);

			/*if (facingDir.x != 0.f)
				veloComp->setFacingDirectionX(facingDir.x);
			if (facingDir.y != 0.f)
				veloComp->setFacingDirectionY(facingDir.y);*/

			nextAnimName = getAttackAnimFacingDir(facingDir);
		}

		std::string nextAttackName = "";
		switch (mCurIndex){
		case 0:
			nextAttackName = mDefaultAttackName + "1";
			break;
		case 1:
			nextAttackName = mDefaultAttackName + "2";
			break;
		case 2:
			nextAttackName = mDefaultAttackName + "3";
			break;
		default:
			break;
		}
		
		if (nextAttackName == "")
			return new PlayerIdleState(mPlayer);
		mIsAttackCommandQueue = false;

		if(!meleeRectComp->startMeleeRect(facingDir, nextAttackName))
			return nullptr;

		std::string attackAnim = nextAnimName +  "_" + std::to_string(mCurIndex + 1);

		mPlayer->comp<AnimationComponent>()->setAnimation(attackAnim);

		mCurIndex++;
		
		
		return nullptr;
	}


	if (!meleeRectComp->meleeRectIsUpdating() && !meleeRectComp->isVulnerable() 
		&& meleeRectComp->isRecovering())
	{
		std::string recoveryAnim = "Recovery_" +  nextAnimName;
		mPlayer->comp<AnimationComponent>()->setAnimation(recoveryAnim);
	}
	
	if (!mIsAttackCommandQueue && !meleeRectComp->meleeRectIsUpdating()
		&& !meleeRectComp->isVulnerable() && !meleeRectComp->isRecovering())
		return new PlayerIdleState(mPlayer);
	
	return nullptr;
}


std::string PlayerAttackState::getAttackAnimFacingDir(const sf::Vector2f& facingDir) const
{
	std::string nextAnimName = "";
	if (facingDir == sf::Vector2f(0.f, 1.f))
		nextAnimName = "Heavy_Attack_Down";
	else if (facingDir == sf::Vector2f(0.f, -1.f))
		nextAnimName = "Heavy_Attack_Up";
	else if (facingDir == sf::Vector2f(1.f, 0.f))
		nextAnimName = "Heavy_Attack_Right";
	else if (facingDir == sf::Vector2f(-1.f, 0.f))
		nextAnimName = "Heavy_Attack_Left";
	
	return nextAnimName;
}