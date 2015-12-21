#include "PlayerCastFireBallState.h"
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
#include <SFML/System/Vector2.hpp>

PlayerCastFireBallState::PlayerCastFireBallState(Entity* player, 
	const luabridge::LuaRef& playerTable,
	const sf::Vector2f& initialDir)
:PlayerState(player, playerTable),
mProjectileDir(initialDir),
mFireBallSpawned(false),
mCastingTime(sf::seconds(0.4f)),
mElapsedCastingTime(sf::Time::Zero),
mRecoveryDur(sf::seconds(0.5f)),
mElapsedRecoveryTime(sf::Time::Zero)
{
}


PlayerCastFireBallState::~PlayerCastFireBallState()
{
}


PlayerState* PlayerCastFireBallState::handleEvent(const sf::Event& event,
	const sf::RenderWindow& renderWindow)
{
	if (!mFireBallSpawned)
		return nullptr;
	
	if (event.type == sf::Event::MouseButtonPressed
		&& event.key.code == sf::Mouse::Right)
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerCastFireBallState(mPlayer, mPlayerStateTable, Utility::unitVector(mousePos - entityWorldPos));
	}
	return nullptr;
}

PlayerState* PlayerCastFireBallState::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	if (!mFireBallSpawned)
		return nullptr;

	return nullptr;
}

PlayerState* PlayerCastFireBallState::update(sf::Time dt)
{
	if (!mFireBallSpawned){
		mElapsedCastingTime += dt;
		if (mElapsedCastingTime > mCastingTime){
			mFireBallSpawned = true;
			CreateNewEntityEvent::Ptr createEntitiesEvent(new CreateNewEntityEvent());

			sf::Vector2f playerWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

			BoxCollisionComponent* boxComp = mPlayer->comp<BoxCollisionComponent>();
			sf::Vector2f playerBoxSize(boxComp->mBoundingRect.width, boxComp->mBoundingRect.height);


			QueueEntityScriptData::EngineInitializeFunc initializeFunc;
			

			playerWorldPos += sf::Vector2f((playerBoxSize.x + 10.f) * mProjectileDir.x, 
				(playerBoxSize.y + 10.f) * mProjectileDir.y);

			mPlayer->comp<SpiritCoreComponent>()->decreaseSpiritCore(1);


			sf::Vector2f projectileDir = mProjectileDir;
			initializeFunc = [projectileDir, playerWorldPos](Entity* newEntity){
				newEntity->comp<VelocityComponent>()->setVelocity(projectileDir);
				newEntity->comp<TransformableComponent>()->setPosition(playerWorldPos);

				TimerComponent* timerComp = newEntity->comp<TimerComponent>();
				sf::Time originalTime = timerComp->getTimeLimitTimer("FireBall_Time");
				newEntity->comp<TimerComponent>()->setNewTimeLimitToTimer("FireBall_Time", 1.0f);
			};
			createEntitiesEvent->queueEntityFromEngine("Sky", scriptDir + "FireBallScript.lua",
				"FireBall", &initializeFunc);
			EventManager::getInstance()->queueEvent(std::move(createEntitiesEvent));

			return new PlayerIdleState(mPlayer, mPlayerStateTable);
		}
		return nullptr;
	}
	
	//return new PlayerIdleState(mPlayer, mPlayerStateTable);

	return nullptr;
}

bool PlayerCastFireBallState::isStateAvailable()
{
	SpiritCoreComponent* spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	return (!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft()));
}