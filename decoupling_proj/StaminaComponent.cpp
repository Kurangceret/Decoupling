#include "StaminaComponent.h"


StaminaComponent::StaminaComponent(Entity* entity)
:Component(entity),
mStaminaWasChanged(true),
mStaminaIsRecovering(true),
mStaminaIndexer(20.f)
{
	mIdentifier = ComponentIdentifier::StaminaComponent;
}


StaminaComponent::~StaminaComponent()
{
}



void StaminaComponent::updateStamina(sf::Time dt)
{
	if (!mStaminaIsRecovering){
		mStaminaIsRecovering = true;
		return;
	}

	float newStamina = mCurStamina + (mStaminaRegenPerSecond * dt.asSeconds());
	setCurStamina(newStamina);
}


void StaminaComponent::setCurStamina(float val)
{
	mCurStamina = val;
	if (mCurStamina > mMaxStamina){
		mCurStamina = mMaxStamina;
		return;
	}
	if (mCurStamina < 0.f)
		mCurStamina = 0.f;
	mStaminaWasChanged = true;
}

void StaminaComponent::setMaxStamina(float val)
{
	mMaxStamina = val;
	if (mMaxStamina <= 0.f)
		mMaxStamina = 1.f;

	mCurStamina = mMaxStamina;

	mStaminaWasChanged = true;
}

float StaminaComponent::getCurStamina() const
{
	return mCurStamina;
}

float StaminaComponent::getMaxStamina() const
{
	return mMaxStamina;
}

void StaminaComponent::setStaminaRegen(float val)
{
	mStaminaRegenPerSecond = val;
	if (mStaminaRegenPerSecond > mMaxStaminaRegenPerSecond)
		mStaminaRegenPerSecond = mMaxStaminaRegenPerSecond;
}

void StaminaComponent::setMaxStaminaRegen(float val)
{
	mMaxStaminaRegenPerSecond = val;
}

float StaminaComponent::getStaminaRegen() const
{
	return mStaminaRegenPerSecond;
}

float StaminaComponent::getMaxStaminaRegen() const
{
	return mMaxStaminaRegenPerSecond;
}

bool StaminaComponent::wasStaminaChanged()
{
	bool original = mStaminaWasChanged;
	mStaminaWasChanged = false;
	return original;
}

void StaminaComponent::increaseCurStamina(float val)
{
	setCurStamina(getCurStamina() + std::abs(val));
}

void StaminaComponent::decreaseCurStamina(float val, bool withStaminaIndexer)
{
	if (!withStaminaIndexer){
		setCurStamina(getCurStamina() - val);
		return;
	}
	int curStamina = static_cast<int>(mCurStamina);
	int staminaRemainder = curStamina % static_cast<int>(mStaminaIndexer);
	
	mCurStamina = curStamina - (staminaRemainder + static_cast<int>(std::abs(val)));
}

bool StaminaComponent::checkDecreaseStamina(float checkVal, bool withStaminaIndexer)
{
	if (!withStaminaIndexer)
		return (mCurStamina - std::abs(checkVal)) >= 0.f;
	
	int curStamina = static_cast<int>(mCurStamina);
	int staminaRemainder = curStamina % static_cast<int>(mStaminaIndexer);

	return curStamina - (staminaRemainder + static_cast<int>(std::abs(checkVal))) >= 0;
}

void StaminaComponent::setStaminaRecover(bool flag)
{
	mStaminaIsRecovering = flag;
}

bool StaminaComponent::isStaminaRecovering() const
{
	return mStaminaIsRecovering;
}

float StaminaComponent::getStaminaIndexer() const
{
	return mStaminaIndexer;
}