#pragma once
#include "CollisionComponent.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include <cstdint>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class ComponentArranger;
struct HarmedEntityData;
struct HarmfulData;

struct HarmfulData{
	HarmfulData();
	float mDamage;
	//this variable will indicate for how long until the entity
	//is suspected to the next collision damage
	sf::Time mPeriodForRest; 
};

struct HarmfulBoxData{
	HarmfulBoxData();

	std::string mBoxName;
	bool mIsActivated; // if this var set to true, this box will be checked for collision
	sf::FloatRect mBoundingBox;
	std::shared_ptr<luabridge::LuaRef> mLuaCollisionReactor;
	HarmfulData mHarmfulData;
	std::size_t mAttackCategory;
	std::vector<HarmedEntityData> mHarmedEntities;
};



struct HarmedEntityData{
	HarmedEntityData(std::uintptr_t ownerAddress, const sf::Time& timeLeft);
	std::uintptr_t mOwnerAddress; // the address of the entity pointer
	sf::Time mTimeLeft; // if timer reaches zero this struct instance will be deleted
};

class HarmfulBoxesComponent :	public CollisionComponent{
public:
	friend ComponentArranger;
public:
	HarmfulBoxesComponent(Entity* entity);
	virtual ~HarmfulBoxesComponent();

	void insertHarmfulBoxIndexForReactor(int index);

	virtual void callCollisionReactor(Entity* thisEntity,
		Entity* collidedEntity, CollisionHandlerSystem* system);

	std::vector<HarmfulBoxData>& getHarmfulBoxesData();

	void updateHarmedEntityData(sf::Time dt);
	void drawHarmfulBoxes(sf::RenderWindow& target);

	void deactivateBox(const std::string& name);
	void activateBox(const std::string& name);
		
private:
	std::vector<HarmfulBoxData> mHarmfulBoxesData;
	std::vector<int> mIndexForCollidedData;

};

