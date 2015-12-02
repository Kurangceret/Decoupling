#include "PlayerHookState.h"
#include "StaminaComponent.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "PlayerIdleState.h"
#include "Utility.h"
#include "VelocityComponent.h"
#include "EventManager.h"
#include "SoulHookStruckEvent.h"
#include "FloatableComponent.h"
#include "CreateNewEntityEvent.h"
#include "Constant.h"
#include "BoxCollisionComponent.h"
#include "TimerComponent.h"

PlayerHookState::PlayerHookState(Entity* player, const luabridge::LuaRef& playerStateTable)
:PlayerState(player, playerStateTable),
mEndThisState(false),
mIsHookingPlayer(false),
mSpawnedHook(false),
mHoldRightMouseTime(sf::Time::Zero),
mElapsedTime(sf::Time::Zero),
mPeriodDecreaseStamina(sf::seconds(0.05f)),
mHookSpeed(500.f)
{
	StaminaComponent* staminaComp = mPlayer->comp<StaminaComponent>();
	staminaComp->decreaseCurStamina(20.f);

	mOriginalSpeed = mPlayer->comp<VelocityComponent>()->getSpeed();

	EventManager::getInstance()->addListener(EventType::SoulHookStruck,
		EventCallBackFunc(Utility::convertPointerToAddress(this),
		std::bind(&PlayerHookState::reactToSoulHookEvent, this, std::placeholders::_1)));
}


PlayerHookState::~PlayerHookState()
{
	EventManager::getInstance()->deleteListener(this);
}

void PlayerHookState::reactToSoulHookEvent(EventBase* eventBase)
{
	if (eventBase->getEventType() != EventType::SoulHookStruck)
		return;

	SoulHookStruckEvent* soulHookEvent = dynamic_cast<SoulHookStruckEvent*>(eventBase);

	if (soulHookEvent->mCollidedEntityCategory == 0){
		mEndThisState = true;
		return;
	}
	mHookLatestPos = soulHookEvent->mHookLatestPos;
	mIsHookingPlayer = true;
	mHookingDir = Utility::unitVector(mHookLatestPos - mPlayer->comp<TransformableComponent>()->getWorldPosition(true));
	mPlayer->comp<FloatableComponent>()->setIsFloating(true);
}

PlayerState* PlayerHookState::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerHookState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	StaminaComponent* staminaComp = mPlayer->comp<StaminaComponent>();
	if (!mSpawnedHook && (!sf::Mouse::isButtonPressed(
		sf::Mouse::Right) || staminaComp->getCurStamina() <= 0.f))
	{
		CreateNewEntityEvent::Ptr createEntitiesEvent(new CreateNewEntityEvent());

		sf::Vector2f playerWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		BoxCollisionComponent* boxComp = mPlayer->comp<BoxCollisionComponent>();
		sf::Vector2f playerBoxSize(boxComp->mBoundingRect.width, boxComp->mBoundingRect.height);
		

		QueueEntityScriptData::EngineInitializeFunc initializeFunc;
		sf::Vector2f dirToMouse = Utility::unitVector(sf::Vector2f(renderWindow.mapPixelToCoords(
				sf::Mouse::getPosition(renderWindow))) - playerWorldPos);

		playerWorldPos += sf::Vector2f((playerBoxSize.x + 10.f) * dirToMouse.x, (playerBoxSize.y + 10.f) * dirToMouse.y);

		sf::Time finalTime = mHoldRightMouseTime / 1.5f;
		if (finalTime < sf::seconds(0.5f))
			finalTime = sf::seconds(0.5f);

		initializeFunc = [dirToMouse, playerWorldPos, finalTime](Entity* newEntity){
			newEntity->comp<VelocityComponent>()->setVelocity(dirToMouse);
			newEntity->comp<TransformableComponent>()->setPosition(playerWorldPos);
			newEntity->comp<TransformableComponent>()->setRotation(Utility::vectorToDegree(dirToMouse, false));
			newEntity->comp<TimerComponent>()->setNewTimeLimitToTimer("Hooking_Time", finalTime.asSeconds());
		};
		createEntitiesEvent->queueEntityFromEngine("Sky", scriptDir + "SoulHookScript.lua", "SoulHook", &initializeFunc);

		mSpawnedHook = true;
		EventManager::getInstance()->queueEvent(std::move(createEntitiesEvent));
	}

	return nullptr;
}

PlayerState* PlayerHookState::update(sf::Time dt)
{
	StaminaComponent* staminaComp = mPlayer->nonCreateComp<StaminaComponent>();
	staminaComp->setStaminaRecover(false);

	if (!mSpawnedHook){
		mHoldRightMouseTime += dt;
		if (mHoldRightMouseTime >= sf::seconds(1.5f))
			mHoldRightMouseTime = sf::seconds(1.5f);

		mElapsedTime += dt;
		if (mElapsedTime >= mPeriodDecreaseStamina){
			staminaComp->decreaseCurStamina(1.f);
			mElapsedTime -= mPeriodDecreaseStamina;
		}
	}

	if (mEndThisState)
		return new PlayerIdleState(mPlayer, mPlayerStateTable);

	if (!mIsHookingPlayer)
		return nullptr;
	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		
	if (mIsHookingPlayer && !mPlayer->comp<FloatableComponent>()->isFloating()){
		veloComp->setSpeed(mOriginalSpeed);
		return new PlayerIdleState(mPlayer, mPlayerStateTable);
	}
	veloComp->setVelocity(mHookingDir);
	veloComp->setSpeed(mHookSpeed);

	return nullptr;
}

bool PlayerHookState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return staminaComp->checkDecreaseStamina(20.f);
}