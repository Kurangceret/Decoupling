#include "SpriteComponent.h"


SpriteComponent::SpriteComponent(Entity* entity)
:DisplayComponent(entity),
mBlinkDuration(sf::seconds(0.90f)),
mPeriodBlink(sf::seconds(0.15f)),
mCurBlinkDur(sf::Time::Zero),
mElapsedPeriod(sf::Time::Zero)
{
	mIdentifier = ComponentIdentifier::SpriteComponent;
	//std::size_t i = ComponentIdentifier::SpriteComponent | ComponentIdentifier::PositionComponent;
}


SpriteComponent::~SpriteComponent()
{
}


sf::Drawable* SpriteComponent::getDrawable()
{
	return &mSprite;
}

void SpriteComponent::beganBlink()
{
	mElapsedPeriod = sf::Time::Zero;
	//mBlinkDuration = sf::seconds(1.2f);
	mCurBlinkDur = mBlinkDuration;
	//mPeriodBlink = sf::seconds(0.1f);
}

void SpriteComponent::updateBlinkStatus(sf::Time dt)
{
	if (mCurBlinkDur.asSeconds() <= 0.0f)
		return;
	mCurBlinkDur -= dt;
	mElapsedPeriod += dt;
}