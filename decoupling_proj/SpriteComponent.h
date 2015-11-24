#pragma once
#include "DisplayComponent.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class RenderingSystem;

class SpriteComponent :	public DisplayComponent{
public:
	friend RenderingSystem;
public:
	SpriteComponent(Entity* entity);
	virtual ~SpriteComponent();

	virtual sf::Drawable* getDrawable();

	sf::Sprite mSprite;

	void beganBlink();
	void updateBlinkStatus(sf::Time dt);
private:
	sf::Time mBlinkDuration;
	sf::Time mPeriodBlink;

	sf::Time mCurBlinkDur;
	sf::Time mElapsedPeriod;
};

