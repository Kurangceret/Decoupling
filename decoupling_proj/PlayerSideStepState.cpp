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
#include "SpiritCoreComponent.h"
#include "CreateNewEntityEvent.h"
#include "BoxCollisionComponent.h"
#include "HealthComponent.h"
#include "EventManager.h"
#include "Constant.h"

/*const float sideStepDur = 0.15f;
const float vulnerableDur = 0.2f;
const float recoveryDur = 0.0f;
const float staminaUsage = 20.f;*/

float sideStepDur = 0.0f;
float vulnerableDur = 0.0f;
float recoveryDur = 0.0f;

PlayerSideStepState::PlayerSideStepState(Entity* player, const sf::Vector2f& initialDir, 
	const luabridge::LuaRef& playerStateTable)
:PlayerState(player, playerStateTable),
mCurrentSideStepDir(0.f, 0.f),
mSideStepDur(/*sf::seconds(sideStepDur)*/),
mRecoveryDur(/*sf::seconds(recoveryDur)*/),
mVulnerableDur(/*sf::seconds(vulnerableDur)*/),
mNextSideStepDir(initialDir),
mStaminaUsage(0.f)
{
	luabridge::LuaRef curT = playerStateTable[getLuaTableName()];
	mSideStepDur = sf::seconds(curT["sideStepDur"].cast<float>());
	sideStepDur = mSideStepDur.asSeconds();

	mVulnerableDur = sf::seconds(curT["vulnerableDur"].cast<float>());
	vulnerableDur = mVulnerableDur.asSeconds();

	mRecoveryDur = sf::seconds(curT["recoveryDur"].cast<float>());
	recoveryDur = mRecoveryDur.asSeconds();

	mStaminaUsage = curT["staminaUsage"].cast<float>();
}


PlayerSideStepState::~PlayerSideStepState()
{
}

std::string PlayerSideStepState::getLuaTableName() const
{
	return "PlayerSideStepState";
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

	SpiritCoreComponent* spiritCoreComp = mPlayer->nonCreateComp<SpiritCoreComponent>();
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left && (!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft())))
	{
		VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
		veloComp->setSpeedIdentifier(1.f);

		AttackModeComponent* attackModeComp = mPlayer->comp<AttackModeComponent>();

		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerAttackState(mPlayer, Utility::unitVector(mousePos - entityWorldPos), mPlayerStateTable,
			attackModeComp->getCurrentStringIndex());
	}
	
	/*if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2f mousePos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		sf::Vector2f entityWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);

		return new PlayerSpiritState(mPlayer, Utility::unitVector(mousePos - entityWorldPos), mPlayerStateTable);
	}*/

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
	return staminaComp->checkDecreaseStamina(mStaminaUsage);
}

PlayerState* PlayerSideStepState::update(sf::Time dt)
{
	VelocityComponent* veloComp = mPlayer->comp<VelocityComponent>();
	mSideStepDur -= dt;

	if (mCurrentSideStepDir != sf::Vector2f() && mSideStepDur.asSeconds() > 0.f)
	{
		veloComp->setVelocity(mCurrentSideStepDir, false);
		veloComp->setSpeedIdentifier(3.5f);
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

		if (staminaComp && !staminaComp->checkDecreaseStamina(mStaminaUsage)){
			mNextSideStepDir = sf::Vector2f();
			return nullptr;
		}
		if (staminaComp)
			staminaComp->decreaseCurStamina(mStaminaUsage, true);
		SpiritCoreComponent* spiritCoreComp = mPlayer->nonCreateComp<SpiritCoreComponent>();

		mSideStepDur = sf::seconds(sideStepDur);
		mRecoveryDur = sf::seconds(recoveryDur);
		mVulnerableDur = sf::seconds(vulnerableDur);

		mCurrentSideStepDir = mNextSideStepDir;
		mNextSideStepDir = sf::Vector2f();

		if (spiritCoreComp && spiritCoreComp->noSpiritCoreLeft()){
			CreateNewEntityEvent::Ptr createEntitiesEvent(new CreateNewEntityEvent());

			QueueEntityScriptData::EngineInitializeFunc initializeFunc;
			sf::Vector2f mainReverseDir = Utility::unitVector(mCurrentSideStepDir * -1.f);

			sf::Vector2f playerWorldPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
			BoxCollisionComponent* boxComp = mPlayer->comp<BoxCollisionComponent>();
			sf::Vector2f playerBoxSize(boxComp->mBoundingRect.width, boxComp->mBoundingRect.height);

			playerWorldPos += sf::Vector2f((playerBoxSize.x + 5.f) * mainReverseDir.x,
				(playerBoxSize.y + 5.f) * mainReverseDir.y);

			initializeFunc = [mainReverseDir, playerWorldPos](Entity* newEntity){
				float finalAngle = Utility::vectorToDegree(mainReverseDir, false) +
					(static_cast<float>(Utility::randomRange(0, 181)) - 90.f);
				sf::Vector2f finalDir = Utility::degreeToVector(finalAngle);
				newEntity->comp<VelocityComponent>()->setVelocity(finalDir);

				newEntity->comp<TransformableComponent>()->setPosition(playerWorldPos);
			};

			int selfDamage = 2;
			for (int i = 0; i < selfDamage; i++)
				createEntitiesEvent->queueEntityFromEngine(
				"Foreground", scriptDir + "HealthSpiritScript.lua",
				"HealthSpirit", &initializeFunc);

			mPlayer->comp<HealthComponent>()->damage(selfDamage, mPlayer);
			EventManager::getInstance()->queueEvent(std::move(createEntitiesEvent));
		}
		else if (spiritCoreComp){
			spiritCoreComp->decreaseSpiritCore(1);
		}
		//return new PlayerSideStepState(mPlayer, mNextSideStepDir);
		return nullptr;
	}

	if (mRecoveryDur.asSeconds() > 0.f){
		mRecoveryDur -= dt;
		return nullptr;
	}


	veloComp->setSpeedIdentifier(1.f);
	return new PlayerIdleState(mPlayer, mPlayerStateTable);
}
