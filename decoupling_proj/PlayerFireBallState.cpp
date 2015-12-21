#include "PlayerFireBallState.h"
#include "SpiritCoreComponent.h"
#include "Entity.h"
#include "VelocityComponent.h"
#include "PlayerIdleState.h"
#include "PlayerSideStepState.h"
#include "TimerComponent.h"
#include "EventManager.h"
#include "CreateNewEntityEvent.h"
#include "BoxCollisionComponent.h"
#include "TransformableComponent.h"
#include "Constant.h"

PlayerFireBallState::PlayerFireBallState(Entity* player, 
	const luabridge::LuaRef& playerData)
:PlayerState(player, playerData),
mRightMouseReleased(false),
mSpawnedFireBall(false),
mTimeForEachTimeBuff(sf::seconds(0.5f)),
mMaximumTimeBuff(1.0f),
mCurrentTimeBuff(0.f),
mEachTimeBuff(0.5f),
mElapsedTime(sf::Time::Zero),
mRecoveryDur(sf::seconds(0.5f)),
mElapsedRecoveryTime(sf::Time::Zero),
mHasReducedSpiritCore(false)
{
	
}


PlayerFireBallState::~PlayerFireBallState()
{
}


PlayerState* PlayerFireBallState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	if (!mRightMouseReleased)
		return nullptr;
	SpiritCoreComponent* spiritCoreComp = nullptr;
	if (mPlayer->hasComp<SpiritCoreComponent>())
		spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	
	if (event.type == sf::Event::MouseButtonPressed 
		&& event.key.code == sf::Mouse::Right)
	{
		

		return new PlayerFireBallState(mPlayer, mPlayerStateTable);
	}
	
	return nullptr;
}
	
PlayerState* PlayerFireBallState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	SpiritCoreComponent* spiritCoreComp = nullptr;
	if (mPlayer->hasComp<SpiritCoreComponent>())
		spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	if (!mRightMouseReleased && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		mRightMouseReleased = true;
		mMouseLastPos = sf::Vector2f(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));

		CreateNewEntityEvent::Ptr createEntitiesEvent(new CreateNewEntityEvent());

		sf::Vector2f playerWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		BoxCollisionComponent* boxComp = mPlayer->comp<BoxCollisionComponent>();
		sf::Vector2f playerBoxSize(boxComp->mBoundingRect.width, boxComp->mBoundingRect.height);


		QueueEntityScriptData::EngineInitializeFunc initializeFunc;
		sf::Vector2f dirToMouse = Utility::unitVector(sf::Vector2f(renderWindow.mapPixelToCoords(
			sf::Mouse::getPosition(renderWindow))) - playerWorldPos);

		playerWorldPos += sf::Vector2f((playerBoxSize.x + 10.f) * dirToMouse.x, (playerBoxSize.y + 10.f) * dirToMouse.y);

		//sf::Time additionalTime(sf::seconds(mCurrentTimeBuff));
		float currentTimeBuff = mCurrentTimeBuff;

		initializeFunc = [dirToMouse, playerWorldPos, currentTimeBuff](Entity* newEntity){
			newEntity->comp<VelocityComponent>()->setVelocity(dirToMouse);
			newEntity->comp<TransformableComponent>()->setPosition(playerWorldPos);
			
			TimerComponent* timerComp = newEntity->comp<TimerComponent>();
			sf::Time originalTime = timerComp->getTimeLimitTimer("FireBall_Time");
			newEntity->comp<TimerComponent>()->setNewTimeLimitToTimer("FireBall_Time", 
				originalTime.asSeconds() + currentTimeBuff);
		};
		createEntitiesEvent->queueEntityFromEngine("Sky", scriptDir + "FireBallScript.lua", 
			"FireBall", &initializeFunc);
		EventManager::getInstance()->queueEvent(std::move(createEntitiesEvent));
		return nullptr;
	}

	if (!mRightMouseReleased)
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

PlayerState* PlayerFireBallState::update(sf::Time dt)
{
	SpiritCoreComponent* spiritCoreComp = nullptr;
	if (mPlayer->hasComp<SpiritCoreComponent>())
		spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	if (!mHasReducedSpiritCore){
		mHasReducedSpiritCore = true;
		spiritCoreComp->decreaseSpiritCore(1);
	}

	if (!mRightMouseReleased){
		mElapsedTime += dt;
		if (mElapsedTime >= mTimeForEachTimeBuff){
			mCurrentTimeBuff += mEachTimeBuff;
			if (mCurrentTimeBuff > mMaximumTimeBuff)
				mCurrentTimeBuff = mMaximumTimeBuff;

			mElapsedTime -= mTimeForEachTimeBuff;
			//if (spiritCoreComp)
				//spiritCoreComp->decreaseSpiritCore(1);
		}
		return nullptr;
	}
		
	mElapsedRecoveryTime += dt;
	if (mElapsedRecoveryTime > mRecoveryDur)
		return new PlayerIdleState(mPlayer, mPlayerStateTable);
	

	return nullptr;
}

bool PlayerFireBallState::isStateAvailable()
{
	SpiritCoreComponent* spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	return (!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft()));
}