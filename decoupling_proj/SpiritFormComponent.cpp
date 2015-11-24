#include "SpiritFormComponent.h"
#include "Utility.h"
#include <algorithm>

SpiritFormComponent::SpiritFormComponent(Entity* entity)
:Component(entity),
/*mCurLifeTime(sf::Time::Zero),
mCurDirection(),*/
mPassedThroughCategory(0),
mSpiritSpeed(100),
mDamageToPassedThroughCategory(0.f),
mIsOnSpiritForm(false),
mNormalLifeTime(0.f),
mDelayBeforeSpirit(0.f),
mStaminaIncrease(0.f)
{
	mIdentifier = ComponentIdentifier::SpritFormComponent;
}


SpiritFormComponent::~SpiritFormComponent()
{
}


/*void SpiritFormComponent::enterSpiritForm(const sf::Time& spiritLifeTime, 
	const sf::Vector2f& direction)
{
	mCurLifeTime = spiritLifeTime;
	mCurDirection = direction;
}*/

void SpiritFormComponent::enterSpiritForm()
{
	mIsOnSpiritForm = true;
}

void  SpiritFormComponent::quitSpiritForm()
{
	mIsOnSpiritForm = false;
	mExcemptionList.clear();
}

bool SpiritFormComponent::isOnSpiritForm() const
{
	//return mCurLifeTime.asSeconds() > 0.f;
	return mIsOnSpiritForm;
}

std::size_t SpiritFormComponent::getPassedThroughCategory() const
{
	return mPassedThroughCategory;
}

float SpiritFormComponent::getDamageToPassedThroughCategory() const
{
	return mDamageToPassedThroughCategory;
}

float SpiritFormComponent::getSpiritSpeed() const
{
	return mSpiritSpeed;
}

void SpiritFormComponent::insertExcemptionEntity(Entity* entity)
{
	if (!entity) return;

	mExcemptionList.push_back(Utility::convertPointerToAddress(entity));
}

sf::Time SpiritFormComponent::getNormalSpiritLifeTime() const
{
	return sf::seconds(mNormalLifeTime);
}

bool SpiritFormComponent::isEntityInsideExcemptionList(Entity* entity) const
{
	if (!entity) return false;

	std::uintptr_t ptrAddress = Utility::convertPointerToAddress(entity);

	return std::find(mExcemptionList.begin(), mExcemptionList.end(), ptrAddress) != mExcemptionList.end();
}

float SpiritFormComponent::getStaminaIncrease() const
{
	return mStaminaIncrease;
}

bool SpiritFormComponent::isExcemptionListEmpty() const
{
	return mExcemptionList.empty();
}