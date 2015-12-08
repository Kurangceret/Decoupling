#pragma once
#include "Component.h"

#include <functional>
#include <SFML/System/Time.hpp>

class Entity;

class HealthComponent :	public Component{
public:
	typedef std::function<void(float&, Entity*)> DamagedReactor;
public:
	HealthComponent(Entity* entity, DamagedReactor damagedReactor = DamagedReactor());
	virtual ~HealthComponent();

	void increaseHealth(float health);
	void setMaxHealth(float health);
	void setCurrentHealth(float health);
	

	float getMaxHealth() const;
	float getCurrentHealth() const;

	void damage(float damage, Entity* damager);

	//calling this function once will set the member variable to reset back
	bool wasHealthChanged();

	void setDamagedReactor(const DamagedReactor& damagedReactor);
	
	//void setImmuneTimer(const sf::Time& timer);
	void setIsImmune(bool flag);
	bool isImmune() const;
	void update(sf::Time dt);
private:
	float mMaxHealth;
	float mCurrentHealth;

	bool mWasHealthChanged;
	DamagedReactor mDamagedFunc;
	//sf::Time mImmuneTimer;
	bool mIsImmune;
};

