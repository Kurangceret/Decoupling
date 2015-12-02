#pragma once
#include <unordered_map>
#include <SFML/System/Time.hpp>
#include "CollisionComponent.h"
#include "RotatedRect.h"
#include "MeleeRectAngleUpdater.h"
#include <functional>

class Entity;
class ComponentArranger;

//a single state of meleeRect
//will be used in std::vector mostly, and another std::vector for movement sets purposes
//no exact dir or position(additionalPos only used to set position exactly)
//the exact dir is based on outside factor
struct SlashData{
	SlashData(float ang, const sf::Vector2f& pos, float duration = 0.f, 
		float delayDuration = 0.f);
	SlashData(float ang, float x, float y, float duration = 0.f,
		float delayDuration = 0.f);
	float angle; //the addition is based on current angle relative to coordinate, so it will adjust based on direction
	sf::Vector2f movingPos; // the same applies

	sf::Time mDuration;
	sf::Time mDelayDuration;
};

//struct that contains data for entity moving when meleeRectIsUpdating
struct EntityMovingData{
	EntityMovingData();
	float speedIdentifier;
};

struct MovementSetData{
	std::vector<SlashData> slashes;
	EntityMovingData entityMovingData;

	float staminaUsage;
	float damage;
	float range;
	float thickness;
	sf::Time wholeDelayDur; //the time before the meleeRect became collideable, while meleeRect is updating
	sf::Time wholeDuration; //the entire duration of the updating meleeRect, including the delayDur

	sf::Time vulnerableDur;
	sf::Time recoveryDur;
};

//entity who has this class has the ability
//to perform melee attack
class MeleeRectComponent :	public CollisionComponent{
public:
	typedef std::function<void(Entity*)> EntityHandler;
	friend ComponentArranger;
public:
	MeleeRectComponent(Entity* entity);
	virtual ~MeleeRectComponent();
	
	void update(sf::Time dt);

	void setAttackCategory(std::size_t newAttackCategory);
	std::size_t getAttackCategory() const;
	bool isNowCollidedable() const;

	const RotatedRect& getRotatedRect() const;

	float getMovementSetStaminaUsage(const 
		std::string& movementSetName);

	bool startMeleeRect(const sf::Vector2f& direction, 
		const std::string& movementSetName, float additionalRelativeAngle = 0.f,
		float additionalRange = 0.f);

	bool startMeleeRectScript(const sf::Vector2f& direction,
		const std::string& movementSetName);

	void insertNewMovementSetData(const std::string& movementName,
		const MovementSetData& movementSetData);

	void stopMeleeRectUpdating();

	float getCurrentDamage() const;

	void insertTouchedEntity(const Entity*, float d);
	bool entityHasBeenTouched(const Entity*) const;

	bool meleeRectIsUpdating() const;
	bool meleeRectIsDelaying() const;

	bool isVulnerable() const;
	bool isRecovering() const;

	sf::Vector2f getTransformedMeleeRectPos() const;
private:
	void calculateMeleeRect();

	void resetMeleeRect();
private:
	MeleeRectAngleUpdater mAngleUpdater;

	RotatedRect mRotatedRect;
	std::size_t mAttackCategory;
	bool mNowIsCollideable;
		
	std::unordered_map<std::string, MovementSetData> mMovementSetList;
		
	std::unordered_map<const Entity*, float> mTouchedEntities;

	sf::Vector2f mCurrentAttackDir;

	std::string mCurrentMoveSetName;
	sf::Time mCurrentVulnerableDur;
	sf::Time mCurrentRecoveryDur;
	float mCurrentAdditionalRange;
	//will be called the mRotatedRect collided with certain entity collision
	EntityHandler mEntityHandler;

	bool mStartEntityMovingData;
};

