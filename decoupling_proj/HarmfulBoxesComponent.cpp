#include "HarmfulBoxesComponent.h"
#include "ScriptAIComponent.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "Utility.h"
#include "TransformableComponent.h"
#include "CategoryComponent.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <iostream>


HarmfulBoxData::HarmfulBoxData()
:mAttackCategory(0),
mBoundingBox(),
mLuaCollisionReactor(nullptr),
mBoxName(""),
mIsActivated(true)
{}

HarmfulData::HarmfulData()
:mDamage(0.f),
mPeriodForRest(sf::seconds(1.5f))
{}

HarmedEntityData::HarmedEntityData(std::uintptr_t ownerAddress, const sf::Time& timeLeft)
: mOwnerAddress(ownerAddress),
mTimeLeft(timeLeft)
{}



HarmfulBoxesComponent::HarmfulBoxesComponent(Entity* entity)
:CollisionComponent(entity)
{
	mIdentifier = ComponentIdentifier::HarmfulBoxesComponent;
}


HarmfulBoxesComponent::~HarmfulBoxesComponent()
{
}

std::vector<HarmfulBoxData>& HarmfulBoxesComponent::getHarmfulBoxesData()
{
	return mHarmfulBoxesData;
}

void HarmfulBoxesComponent::callCollisionReactor(Entity* thisEntity,
	Entity* collidedEntity, CollisionHandlerSystem* system)
{
	auto collidedAddress = Utility::convertPointerToAddress(collidedEntity);
	for (auto i : mIndexForCollidedData){
		HarmfulBoxData& harmfulBoxData = mHarmfulBoxesData[i];
		
		if (std::find_if(harmfulBoxData.mHarmedEntities.begin(),
			harmfulBoxData.mHarmedEntities.end(),
			[&](HarmedEntityData& entityData)
		{
			return entityData.mOwnerAddress == collidedAddress;
		})
			!= harmfulBoxData.mHarmedEntities.end())
		{
			continue;
		}
		
		HealthComponent* collidedHealthComp = collidedEntity->nonCreateComp<HealthComponent>();
		CategoryComponent* collidedCategoryComp = collidedEntity->nonCreateComp<CategoryComponent>();

		if (collidedHealthComp && collidedCategoryComp){
			if (collidedCategoryComp->getCategory() & harmfulBoxData.mAttackCategory)
				collidedHealthComp->damage(harmfulBoxData.mHarmfulData.mDamage, mOwnerEntity);
		}
		
		harmfulBoxData.mHarmedEntities.push_back(
			HarmedEntityData(collidedAddress, 
			harmfulBoxData.mHarmfulData.mPeriodForRest));

		luabridge::LuaRef* luaCollisionReactor = harmfulBoxData.mLuaCollisionReactor.get();
		if (luaCollisionReactor){
			try{
				if (mOwnerEntity->hasComp<ScriptAIComponent>()){
					(*luaCollisionReactor)(thisEntity, collidedEntity, system, harmfulBoxData,
						mOwnerEntity->comp<ScriptAIComponent>()->getCurAIState()->getLuaReferenceToState());
				}
				else{
					(*luaCollisionReactor)(thisEntity, collidedEntity, system, harmfulBoxData);
				}
			}
			catch (luabridge::LuaException const& e) {
				std::cerr << e.what() << std::endl;
			}
		}
	}

	mIndexForCollidedData.clear();
}

void HarmfulBoxesComponent::insertHarmfulBoxIndexForReactor(int index)
{
	if (index < 0 || index > mIndexForCollidedData.size())
		return;
	mIndexForCollidedData.push_back(index);
}

void HarmfulBoxesComponent::updateHarmedEntityData(sf::Time dt)
{
	for (auto& harmfulBox : mHarmfulBoxesData){
		for (auto iter = harmfulBox.mHarmedEntities.begin(); iter != harmfulBox.mHarmedEntities.end();)
		{
			iter->mTimeLeft -= dt;
			if (iter->mTimeLeft.asSeconds() <= 0.f){
				iter = harmfulBox.mHarmedEntities.erase(iter);
				continue;
			}
			iter++;
		}
	}
}


void HarmfulBoxesComponent::deactivateBox(const std::string& name)
{
	auto iter = std::find_if(mHarmfulBoxesData.begin(), mHarmfulBoxesData.end(), 
		[&](HarmfulBoxData& harmfulBox)
	{
		return harmfulBox.mBoxName == name;
	});

	if (iter == mHarmfulBoxesData.end())
		return;
	iter->mIsActivated = false;
}

void HarmfulBoxesComponent::activateBox(const std::string& name)
{
	auto iter = std::find_if(mHarmfulBoxesData.begin(), mHarmfulBoxesData.end(),
		[&](HarmfulBoxData& harmfulBox)
	{
		return harmfulBox.mBoxName == name;
	});

	if (iter == mHarmfulBoxesData.end())
		return;
	iter->mIsActivated = true;
}

void HarmfulBoxesComponent::drawHarmfulBoxes(sf::RenderWindow& target)
{

	sf::Transform worldTransform = mOwnerEntity->comp<TransformableComponent>()->getWorldTransform();
	for (auto& harmfulBoxData : mHarmfulBoxesData){
		sf::FloatRect rect(worldTransform.transformRect(
			harmfulBoxData.mBoundingBox));
		sf::RectangleShape shape;
		//sf::FloatRect rect = getBoundingRect();

		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Magenta);
		if (harmfulBoxData.mIsActivated)
			shape.setOutlineColor(sf::Color::Red);
		shape.setOutlineThickness(1.f);

		target.draw(shape);
	}
}
