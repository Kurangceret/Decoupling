#include "PlayerIdleState.h"
#include "VelocityComponent.h"
#include "Entity.h"
#include "AnimationComponent.h"
#include <string>
#include "PlayerRunningState.h"
#include "PlayerAttackState.h"
#include "PlayerSideStepState.h"
#include "PlayerSpiritState.h"
#include "StaminaComponent.h"
#include "AttackModeComponent.h"
#include "Utility.h"
#include "TransformableComponent.h"
#include "PlayerLeftClawState.h"
#include "PlayerHookState.h"
#include "SpiritCoreComponent.h"
#include "PlayerAimRangeState.h"
#include "HealthComponent.h"
#include "BuffableComponent.h"
#include "Constant.h"

PlayerIdleState::PlayerIdleState(Entity* playerEntity, const luabridge::LuaRef& playerStateTable)
:PlayerState(playerEntity, playerStateTable)
{
}


PlayerIdleState::~PlayerIdleState()
{
}


PlayerState* PlayerIdleState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	/*if (event.type != sf::Event::KeyPressed)
		return nullptr;*/

	AttackModeComponent* attackModeComp =  mPlayer->comp<AttackModeComponent>();

	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Space)
	{
		VelocityComponent* velocityComp = mPlayer->nonCreateComp<VelocityComponent>();

		sf::Vector2f nextSideStepDir;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			nextSideStepDir.x = -1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			nextSideStepDir.y = -1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			nextSideStepDir.y = 1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			nextSideStepDir.x = 1.f;

		if (nextSideStepDir == sf::Vector2f())
			nextSideStepDir = velocityComp->getFacingDirection() * -1.f;

		return new PlayerSideStepState(mPlayer, nextSideStepDir, mPlayerStateTable);
	}

	if (event.type == sf::Event::KeyPressed && 
		event.key.code == sf::Keyboard::Z)
	{
		attackModeComp->incrementIndex();
	}
	SpiritCoreComponent* spiritCoreComp = mPlayer->nonCreateComp<SpiritCoreComponent>();

	HealthComponent* healthComp = mPlayer->comp<HealthComponent>();

	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::R)
	{
		spiritCoreComp->startRestoring();
		//healthComp->setImmuneTimer(sf::Time::Zero);
	}

	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Q 
		&& (!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft())))
	{
		mPlayer->comp<BuffableComponent>()->insertBuffWithScriptName(buffScriptDir + 
			"SpiritShieldBuffScript.lua", "SpiritShieldBuff");

		//healthComp->setImmuneTimer(sf::seconds(5.0f));
		spiritCoreComp->decreaseSpiritCore(1);
	}
	
	
	if (event.type == sf::Event::MouseButtonPressed && 
		event.mouseButton.button == sf::Mouse::Left 
		&& (!spiritCoreComp || (!spiritCoreComp->isRestoring() && 
		!spiritCoreComp->noSpiritCoreLeft())))
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
		
		return new PlayerAttackState(mPlayer, Utility::unitVector(mousePos - entityWorldPos), mPlayerStateTable,
			attackModeComp->getCurrentStringIndex());
		//return new PlayerLeftClawState(mPlayer);
	}
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Right && 
		(!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft())))
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
		

		return new PlayerAimRangeState(mPlayer, mPlayerStateTable);
	}
	/*if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Right)
	{
		/*sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerSpiritState(mPlayer, Utility::unitVector(mousePos - entityWorldPos));
		return new PlayerHookState(mPlayer, mPlayerStateTable);
	}*/

	/*if (event.key.code == sf::Keyboard::Up){
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		return new PlayerAttackState(mPlayer, veloComp->getFacingDirection(), 
			attackModeComp->getCurrentStringIndex());
	}*/
	return nullptr;
}

PlayerState* PlayerIdleState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	VelocityComponent* velocityComp = mPlayer->nonCreateComp<VelocityComponent>();

	sf::Vector2f finalVelo;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		finalVelo.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		finalVelo.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		finalVelo.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		finalVelo.x = 1.f;

	bool turnFaceDir = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && finalVelo != sf::Vector2f())
		turnFaceDir = false;

	velocityComp->setVelocity(finalVelo, turnFaceDir);

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		if (finalVelo == sf::Vector2f())
			finalVelo = velocityComp->getFacingDirection() * -1.f;

		return new PlayerSideStepState(mPlayer, finalVelo);
	}*/

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		return new PlayerRunningState(mPlayer);*/

	return nullptr;
}

bool PlayerIdleState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(0.f);
}

PlayerState* PlayerIdleState::update(sf::Time dt)
{
	SpiritCoreComponent* spiritCoreComp = mPlayer->nonCreateComp<SpiritCoreComponent>();
	if (spiritCoreComp->noSpiritCoreLeft())
		spiritCoreComp->startRestoring();
	std::string finalAnimName = "";

	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();

	if (veloComp->getFacingDirectionX() != 0.f)
		finalAnimName = veloComp->getFacingDirectionX() > 0.f ? "Standing_Right" : "Standing_Left";
	if (veloComp->getFacingDirectionY() != 0.f)
		finalAnimName = veloComp->getFacingDirectionY() > 0.f ? "Standing_Down" : "Standing_Up";

	/*if (veloComp->getVelocityX() != 0.f && veloComp->getFacingDirectionX() != 0.f){
		finalAnimName = veloComp->getFacingDirectionX() > 0.f ? "Moving_Right" : "Moving_Left";
	}
	if (veloComp->getVelocityY() != 0.f && veloComp->getFacingDirectionY() != 0.f)
		finalAnimName = veloComp->getFacingDirectionY() > 0.f ? "Moving_Down" : "Moving_Up";*/
	if (veloComp->getVelocity() != sf::Vector2f()){
		if (veloComp->getFacingDirectionX() != 0.f)
			finalAnimName = veloComp->getFacingDirectionX() > 0.f ? "Moving_Right" : "Moving_Left";
		if (veloComp->getFacingDirectionY() != 0.f)
			finalAnimName = veloComp->getFacingDirectionY() > 0.f ? "Moving_Down" : "Moving_Up";
	}
	
	
	if (finalAnimName != "")
		mPlayer->comp<AnimationComponent>()->setAnimation(finalAnimName);

	return nullptr;
}