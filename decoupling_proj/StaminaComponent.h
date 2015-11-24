#pragma once
#include "Component.h"
#include <SFML/System/Time.hpp>


class Entity;
class ComponentArranger;

class StaminaComponent :public Component{
public:
	friend ComponentArranger;
public:
	StaminaComponent(Entity* entity);
	virtual ~StaminaComponent();

	void updateStamina(sf::Time dt);

	//void decreaseStamina(float val);
	//void increaseStamina(float val);
	
	void setCurStamina(float val);

	void increaseCurStamina(float val);
	void decreaseCurStamina(float val, bool withStaminaIndexer = false);

	bool checkDecreaseStamina(float checkVal, bool withStaminaIndexer = false);

	void setMaxStamina(float val);

	float getCurStamina() const;
	float getMaxStamina() const;

	void setStaminaRegen(float val);
	void setMaxStaminaRegen(float val);

	float getStaminaRegen() const;
	float getMaxStaminaRegen() const;
	
	bool wasStaminaChanged();

	void setStaminaRecover(bool flag);
	bool isStaminaRecovering() const;


	float getStaminaIndexer() const;
private:
	float mCurStamina;
	float mMaxStamina;

	float mStaminaRegenPerSecond;
	float mMaxStaminaRegenPerSecond;


	float mStaminaIndexer;

	bool mStaminaWasChanged;
	bool mStaminaIsRecovering;
};

