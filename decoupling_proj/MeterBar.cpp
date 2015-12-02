#include "MeterBar.h"
#include "PlayerHPChangedEvent.h"
#include "PlayerStaminaChangedEvent.h"
#include "SpiritCoreChangedEvent.h"

MeterBar::MeterBar(const sf::Color& barColor)
{
	mOuterRect.setFillColor(sf::Color::Transparent);
	mOuterRect.setOutlineThickness(1.f);
	mOuterRect.setOutlineColor(sf::Color::Blue);
	mOuterRect.setSize(sf::Vector2f(70.f, 15.f));

	mFillRect.setFillColor(barColor);
	mFillRect.setSize(sf::Vector2f(70.f, 15.f));
}


MeterBar::~MeterBar()
{
}

void MeterBar::setSize(const sf::Vector2f& size)
{
	mOuterRect.setSize(size);
	mFillRect.setSize(size);
}

void MeterBar::setSize(float width, float height)
{
	setSize(sf::Vector2f(width, height));
}


void MeterBar::drawMeter(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mOuterRect, states);
	target.draw(mFillRect, states);
}


void MeterBar::reactToChangingValue(EventBase* eventBase)
{
	if (eventBase->getEventType() == EventType::PlayerHPChanged){
		PlayerHPChangedEvent* playerHPChangedEvent = dynamic_cast<PlayerHPChangedEvent*>(eventBase);

		float curHealth = playerHPChangedEvent->mCurrentHP;
		curHealth = curHealth > 0.f ? curHealth : 0.f;


		float maxHealth = playerHPChangedEvent->mMaxHP;
		mFillRect.setSize(sf::Vector2f(mOuterRect.getSize().x *
			(curHealth / maxHealth), mOuterRect.getSize().y));
	}
	else if (eventBase->getEventType() == EventType::PlayerStaminaChanged){
		PlayerStaminaChangedEvent* playerStaminaChangedEvent = dynamic_cast<PlayerStaminaChangedEvent*>(eventBase);
		float curStamina = playerStaminaChangedEvent->mCurrentStamina;
		curStamina = curStamina > 0.f ? curStamina : 0.f;


		float maxStamina = playerStaminaChangedEvent->mMaxStamina;
		mFillRect.setSize(sf::Vector2f(mOuterRect.getSize().x *
			(curStamina / maxStamina), mOuterRect.getSize().y));
	}
	else if (eventBase->getEventType() == EventType::SpiritCoreChanged){
		SpiritCoreChangedEvent* spiritCoreChangedEvent = dynamic_cast<SpiritCoreChangedEvent*>(eventBase);

		//if (spiritCoreChangedEvent->mIsRestoring){
			mFillRect.setSize(sf::Vector2f(mOuterRect.getSize().x *
				spiritCoreChangedEvent->mCurRatioInRestoring, mOuterRect.getSize().y));
		//}
	}
}