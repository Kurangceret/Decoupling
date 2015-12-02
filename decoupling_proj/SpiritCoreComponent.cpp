#include "SpiritCoreComponent.h"
#include "VelocityComponent.h"
#include "Entity.h"

SpiritCoreComponent::SpiritCoreComponent(Entity* entity)
:Component(entity),
mCurrentSpiritCore(1),
mMaxSpiritCore(1),
mRestoreTimePerCore(sf::seconds(0.3f)),
mElapsedTime(sf::seconds(-1.f))
{
	mIdentifier = ComponentIdentifier::SpiritCoreComponent;
}


SpiritCoreComponent::~SpiritCoreComponent()
{
}


void SpiritCoreComponent::updateRestoreStatus(sf::Time dt)
{
	

	if (mElapsedTime.asSeconds() < 0.0f)
		return;

	VelocityComponent* veloCom = mOwnerEntity->nonCreateComp<VelocityComponent>();

	veloCom->setSpeedIdentifier(0.7f);

	mElapsedTime += dt;
	if (mElapsedTime.asSeconds() >= mRestoreTimePerCore.
		asSeconds() * mMaxSpiritCore)
	{
		mElapsedTime = sf::seconds(-1.f);
		restoreSpiritCoreFully();
		veloCom->setSpeedIdentifier(1.f);
	}
}

void SpiritCoreComponent::startRestoring()
{
	if (mCurrentSpiritCore >= mMaxSpiritCore || 
		mElapsedTime.asSeconds() >= 0.f)
		return;
	mCurrentSpiritCore = 0;
	mElapsedTime = sf::Time::Zero;
}

bool SpiritCoreComponent::isRestoring() const
{
	return mElapsedTime.asSeconds() >= 0.0f;
}

void SpiritCoreComponent::restoreSpiritCoreFully()
{
	mCurrentSpiritCore = mMaxSpiritCore;
}

void SpiritCoreComponent::increaseSpiritCore(int val)
{
	mCurrentSpiritCore += std::abs(val);
	if (mCurrentSpiritCore > mMaxSpiritCore)
		mCurrentSpiritCore = mMaxSpiritCore;
}

void SpiritCoreComponent::decreaseSpiritCore(int val)
{
	mCurrentSpiritCore -= std::abs(val);
	if (mCurrentSpiritCore < 0)
		mCurrentSpiritCore = 0;
}

int SpiritCoreComponent::getCurrentSpiritCore() const
{
	return mCurrentSpiritCore;
}

int SpiritCoreComponent::getMaxSpiritCore() const
{
	return mMaxSpiritCore;
}

bool SpiritCoreComponent::noSpiritCoreLeft() const
{
	return mCurrentSpiritCore <= 0;
}

float SpiritCoreComponent::getCurTimeRatioInRestoring() const
{
	if (mElapsedTime.asSeconds() < 0.f)
		return 0.f;

	return mElapsedTime.asSeconds() / (mRestoreTimePerCore.asSeconds() 
		* mMaxSpiritCore);
}