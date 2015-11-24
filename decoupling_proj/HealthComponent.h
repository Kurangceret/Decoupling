#pragma once
#include "Component.h"

#include <functional>


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
private:
	float mMaxHealth;
	float mCurrentHealth;

	bool mWasHealthChanged;
	DamagedReactor mDamagedFunc;
};

