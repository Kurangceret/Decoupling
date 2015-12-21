#include "PlayerCastingBuff.h"
#include "SpiritCoreComponent.h"
#include "Entity.h"
#include "VelocityComponent.h"
#include "BuffableComponent.h"
#include "BoxCollisionComponent.h"
#include "TransformableComponent.h"
#include "Constant.h"
#include "PlayerIdleState.h"

PlayerCastingBuff::PlayerCastingBuff(Entity* player, luabridge::LuaRef& playerTable,
	const std::string& buffScriptDir, const std::string& buffTableName)
:PlayerState(player, playerTable),
mBuffScriptDir(buffScriptDir),
mBuffTableName(buffTableName),
mElapsedTime(sf::Time::Zero),
mTimeBeforeApplyingBuff(sf::seconds(0.5f)),
mHasReducedSpiritCore(false)
{
}


PlayerCastingBuff::~PlayerCastingBuff()
{
}


PlayerState* PlayerCastingBuff::handleEvent(const sf::Event&,
	const sf::RenderWindow& renderWindow)
{
	return nullptr;
}

PlayerState* PlayerCastingBuff::processRealTimeInput(sf::Time dt,
	const sf::RenderWindow& renderWindow)
{
	sf::Vector2f finalVelo;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		finalVelo.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		finalVelo.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		finalVelo.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		finalVelo.x = 1.f;

	mPlayer->comp<VelocityComponent>()->setVelocity(finalVelo);
	return nullptr;
}

PlayerState* PlayerCastingBuff::update(sf::Time dt)
{
	SpiritCoreComponent* spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	if (!mHasReducedSpiritCore){
		mPlayer->comp<VelocityComponent>()->setSpeedIdentifier(0.7f);
		spiritCoreComp->decreaseSpiritCore(1);
		mHasReducedSpiritCore = true;
	}
	mElapsedTime += dt;
	if (mElapsedTime < mTimeBeforeApplyingBuff)
		return nullptr;

	BuffableComponent* buffableComp = mPlayer->comp<BuffableComponent>();
	buffableComp->insertBuffWithScriptName(mBuffScriptDir, mBuffTableName);
	mPlayer->comp<VelocityComponent>()->setSpeedIdentifier(1.f);
	return new PlayerIdleState(mPlayer, mPlayerStateTable);
}

bool PlayerCastingBuff::isStateAvailable()
{
	SpiritCoreComponent* spiritCoreComp = mPlayer->comp<SpiritCoreComponent>();

	return (!spiritCoreComp || (!spiritCoreComp->isRestoring() &&
		!spiritCoreComp->noSpiritCoreLeft()));
}