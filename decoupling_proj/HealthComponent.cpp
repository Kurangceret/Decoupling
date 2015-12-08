#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "Entity.h"
#include "EntityGotDamagedEvent.h"
#include "EventManager.h"

HealthComponent::HealthComponent(Entity* entity, DamagedReactor damagedReactor)
:Component(entity),
mDamagedFunc(damagedReactor),
mMaxHealth(1),
mCurrentHealth(1),
mWasHealthChanged(false),
mIsImmune(false)
//mImmuneTimer(sf::Time::Zero)
{
	mIdentifier = ComponentIdentifier::HealthComponent;
	mDamagedFunc = [&](float&, Entity*){

	};
}


HealthComponent::~HealthComponent()
{
}


void HealthComponent::increaseHealth(float health)
{
	mCurrentHealth += std::abs(health);
	mWasHealthChanged = true;
}

void HealthComponent::setMaxHealth(float health)
{
	mMaxHealth = health;
	mWasHealthChanged = true;
}

void HealthComponent::setCurrentHealth(float health)
{
	mCurrentHealth = health;
	mWasHealthChanged = true;
}

float HealthComponent::getMaxHealth() const
{
	return mMaxHealth;
}

float HealthComponent::getCurrentHealth() const
{
	return mCurrentHealth;
}

void HealthComponent::damage(float damage, Entity* damager)
{
	/*if (isImmune()){
		mImmuneTimer = sf::Time::Zero;
		return;
	}*/

	if (isImmune())
		damage = 0.f;

	mDamagedFunc(damage, damager);
	mCurrentHealth -= std::abs(damage);
	mWasHealthChanged = true;

	if (damager != mOwnerEntity && damage > 0.f && 
		mOwnerEntity->hasComp<SpriteComponent>())
		mOwnerEntity->comp<SpriteComponent>()->beganBlink();

	EntityGotDamagedEvent::Ptr eventBase(new EntityGotDamagedEvent(mOwnerEntity));
	EventManager::getInstance()->queueEvent(std::move(eventBase));
	
}

bool HealthComponent::wasHealthChanged()
{
	bool original = mWasHealthChanged;
	mWasHealthChanged = false;
	return original;
}

void HealthComponent::setDamagedReactor(const DamagedReactor& damagedReactor)
{
	mDamagedFunc = damagedReactor;
}

/*void HealthComponent::setImmuneTimer(const sf::Time& timer)
{
	mImmuneTimer = timer;
}*/

void HealthComponent::setIsImmune(bool flag)
{
	mIsImmune = flag;
}

bool HealthComponent::isImmune() const
{
	//return mImmuneTimer > sf::Time::Zero;
	return mIsImmune;
}

void HealthComponent::update(sf::Time dt)
{
	
}