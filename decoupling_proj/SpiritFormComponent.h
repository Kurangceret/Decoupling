#pragma once
#include "Component.h"
#include <vector>
#include <cstdint>
#include <SFML/System/Time.hpp>

class ComponentArranger;
class PlayerSpiritState;

class SpiritFormComponent :	public Component{
public:
	friend ComponentArranger;
	friend PlayerSpiritState;
public:
	SpiritFormComponent(Entity* entity);
	virtual ~SpiritFormComponent();

	/*void enterSpiritForm(const sf::Time& spiritLifeTime, 
		const sf::Vector2f& direction);*/

	void enterSpiritForm();
	void quitSpiritForm();

	bool isOnSpiritForm() const;

	std::size_t getPassedThroughCategory() const;
	float getDamageToPassedThroughCategory() const;
	float getSpiritSpeed() const;

	//means insert entity so it will not get checked
	//in any other way...
	void insertExcemptionEntity(Entity* entity);
	bool isExcemptionListEmpty() const;
	bool isEntityInsideExcemptionList(Entity* entity) const;

	sf::Time getNormalSpiritLifeTime() const;

	float getStaminaIncrease() const;
private:
	//sf::Vector2f mCurDirection;
	float mSpiritSpeed;

	float mDamageToPassedThroughCategory;
	
	//sf::Time mCurLifeTime;

	std::size_t mPassedThroughCategory;

	bool mIsOnSpiritForm;

	std::vector<std::uintptr_t> mExcemptionList;

	float mNormalLifeTime;
	float mDelayBeforeSpirit;

	float mStaminaIncrease;
	//sf::Time mCooldownBeforeNextSpirit;
};

