#include "PlayerLogicSystem.h"
#include "CategoryComponent.h"
#include "TransformableComponent.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "Entity.h"
#include "ScriptUpdateComponent.h"
#include "EventManager.h"
#include "HealthComponent.h"
#include "PlayerHPChangedEvent.h"
#include "PlayerStaminaChangedEvent.h"
#include "StaminaComponent.h"
#include "PlayerIdleState.h"
#include "SpiritCoreComponent.h"
#include "SpiritCoreChangedEvent.h"
#include "FloatableComponent.h"
#include "TextDisplayComponent.h"
#include "EntityExpertiseComponent.h"
#include <iostream>

PlayerLogicSystem::PlayerLogicSystem(Entity* entityPlayer, const luabridge::LuaRef& playerStateDataTable)
:mPlayerState(new PlayerIdleState(entityPlayer, playerStateDataTable)),
mPlayer(entityPlayer),
mPlayerStateDataTable(playerStateDataTable),
mPreviousSpiritCoreNum(0)
{
	pushRequiredComponent(ComponentIdentifier::CategoryComponent);
}


PlayerLogicSystem::~PlayerLogicSystem()
{
}


void PlayerLogicSystem::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	mPlayer->comp<VelocityComponent>()->setVelocity(0.f, 0.f, false);
	//mPlayer->comp<FloatableComponent>()->setIsFloating(false);
	mPlayer->comp<EntityExpertiseComponent>()->setAbleToFloat(false);
	if (!mPlayerState.get())
		return;



	PlayerState* newPlayerState = mPlayerState->handleEvent(event, renderWindow);
	checkStateValidity(newPlayerState, mPlayer);

	if (newPlayerState)
		mPlayerState.reset(newPlayerState);
	
}

void PlayerLogicSystem::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	
	if (!mPlayerState.get())
		return;

	PlayerState* newPlayerState =  mPlayerState->processRealTimeInput(dt, renderWindow);
	checkStateValidity(newPlayerState, mPlayer);
	if (newPlayerState)
		mPlayerState.reset(newPlayerState);
}



void PlayerLogicSystem::processEntity(sf::Time dt, Entity* entity)
{
	if (!(entity->comp<CategoryComponent>()->getCategory() & Category::Player))
		return;
	
	
	if (!mPlayerState.get())
		return;

	TextDisplayComponent* textDisplayComp = mPlayer->comp<TextDisplayComponent>();
	textDisplayComp->setString("");
	if (mPlayer->comp<HealthComponent>()->isImmune())
		textDisplayComp->setString("Immune");
	
	SpiritCoreComponent* spiritCoreComp = entity->nonCreateComp<SpiritCoreComponent>();

	/*if (spiritCoreComp && spiritCoreComp->isRestoring())
		mPlayer->comp<VelocityComponent>()->setSpeedIdentifier(0.7f);*/


	PlayerState* newPlayerState = mPlayerState->update(dt);
	checkStateValidity(newPlayerState, mPlayer);
	if (newPlayerState)
		mPlayerState.reset(newPlayerState);

	StaminaComponent* staminaComp = entity->nonCreateComp<StaminaComponent>();

	if (staminaComp){
		staminaComp->updateStamina(dt);
		PlayerStaminaChangedEvent::Ptr staminaChanged(new PlayerStaminaChangedEvent());
		staminaChanged->mCurrentStamina = staminaComp->getCurStamina();
		staminaChanged->mMaxStamina = staminaComp->getMaxStamina();
		staminaChanged->mNumIndexer = staminaComp->getStaminaIndexer();

		EventManager::getInstance()->queueEvent(std::move(staminaChanged));
	}

	if (entity->hasComp<HealthComponent>()){
		HealthComponent* healthComp = entity->comp<HealthComponent>();
		if (healthComp->wasHealthChanged()){
			PlayerHPChangedEvent::Ptr playerHpChanged(new PlayerHPChangedEvent());
			playerHpChanged->mCurrentHP = healthComp->getCurrentHealth();
			playerHpChanged->mMaxHP = healthComp->getMaxHealth();

			EventManager::getInstance()->queueEvent(std::move(playerHpChanged));
		}
	}

//	SpiritCoreComponent* spiritCoreComp = entity->nonCreateComp<SpiritCoreComponent>();
	if (spiritCoreComp){
		int previousCore = mPreviousSpiritCoreNum;
		mPreviousSpiritCoreNum = spiritCoreComp->getCurrentSpiritCore();

		if (spiritCoreComp->isRestoring() || previousCore != mPreviousSpiritCoreNum){
			SpiritCoreChangedEvent::Ptr coreChangedEvent(new SpiritCoreChangedEvent());

			coreChangedEvent->mIsRestoring = spiritCoreComp->isRestoring();
			coreChangedEvent->mCurrentSpiritCores = mPreviousSpiritCoreNum;
			coreChangedEvent->mCurRatioInRestoring = spiritCoreComp->getCurTimeRatioInRestoring();

			EventManager::getInstance()->queueEvent(std::move(coreChangedEvent));
		}
	}




	

	//std::cout << "OK!" << std::endl;
}


void PlayerLogicSystem::checkStateValidity(PlayerState*& playerStatePointer, Entity* player)
{
	if (!playerStatePointer)
		return;

	
	//StaminaComponent* staminaComp = player->comp<StaminaComponent>();

	if (!playerStatePointer->isStateAvailable()){
		delete playerStatePointer;
		playerStatePointer = nullptr;
	}
}