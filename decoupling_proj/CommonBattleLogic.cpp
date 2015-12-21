#include "CommonBattleLogic.h"
#include "MeleeRectComponent.h"
#include "CategoryComponent.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "BoxCollisionComponent.h"
#include "PathFinder.h"
#include "TransformableComponent.h"
#include "SpiritGrabberComponent.h"
#include "Utility.h"
#include "BuffableComponent.h"
#include "Constant.h"

CommonBattleLogic::CommonBattleLogic()
{
}


CommonBattleLogic::~CommonBattleLogic()
{
}


void CommonBattleLogic::insertNewPair(MeleeRectComponent* meleeRectComp, Entity* collidedEntity)
{
	mToBeCheckedList.push_back(std::make_pair(meleeRectComp, collidedEntity));
}

void CommonBattleLogic::runThroughCheckedList(sf::Time dt)
{
	for (auto& iter : mToBeCheckedList){
		MeleeRectComponent* meleeRectComp = iter.first;
		Entity* collidedEntity = iter.second;
				

		if (!collidedEntity->hasComp<CategoryComponent>())
			continue;

		//Entity* thisEntity = meleeRectComp->mOwnerEntity;

		CategoryComponent* collidedCategoryComp = collidedEntity->comp<CategoryComponent>();
		HealthComponent* collidedHealthComp = nullptr;

		if (collidedEntity->hasComp<HealthComponent>())
			collidedHealthComp = collidedEntity->comp<HealthComponent>();

		if (collidedCategoryComp->getCategory() & Category::Tile){
			float tileSizeX = PathFinder::getInstance()->getTileSize().x;
			sf::Vector2f collidedWorldPos = collidedEntity->comp<TransformableComponent>()
				->getWorldPosition(true);
			sf::Vector2f meleeOwnerWorldPos = PathFinder::getInstance()->sceneToGraph(
				meleeRectComp->mOwnerEntity->comp<TransformableComponent>
				()->getWorldPosition(true))->pos;

			if ((Utility::vectorLength(collidedWorldPos - 
				meleeRectComp->getTransformedMeleeRectPos()) <= tileSizeX / 2.f)
				|| 
				(Utility::vectorLength(meleeOwnerWorldPos - collidedWorldPos) <= tileSizeX))
			{
				meleeRectComp->stopMeleeRectUpdating();
			}
		}
		
		if (collidedHealthComp && meleeRectComp->getAttackCategory() 
			& collidedCategoryComp->getCategory())
		{
			//float meleeRectCurDamage = meleeRectComp->getCurrentDamage();
			collidedHealthComp->damage(meleeRectComp->getCurrentDamage(), 
				meleeRectComp->mOwnerEntity);

			if (collidedEntity->hasComp<BuffableComponent>()){
				BuffableComponent* buffableComp = collidedEntity->comp<BuffableComponent>();

				sf::Vector2f collidedWorldPos = collidedEntity->comp<TransformableComponent>()
					->getWorldPosition(true);
				sf::Vector2f meleeOwnerWorldPos = PathFinder::getInstance()->sceneToGraph(
					meleeRectComp->mOwnerEntity->comp<TransformableComponent>
					()->getWorldPosition(true))->pos;

				sf::Vector2f finalDir = Utility::unitVector(collidedWorldPos - meleeOwnerWorldPos);
				buffableComp->insertBuffWithScriptName(
					debuffScriptDir + "KnockBackDebuffScript.lua", 
					"KnockBackDebuff",
					[finalDir](const luabridge::LuaRef& table)
					{
					try{
						table["mKnockBackDirX"] = finalDir.x;
						table["mKnockBackDirY"] = finalDir.y;
					}
					catch (luabridge::LuaException& e){
						std::cout << e.what() << std::endl;
					}
					});
			}
			
			/*SpiritGrabberComponent* spiritComp = meleeRectComp->mOwnerEntity->
				nonCreateComp<SpiritGrabberComponent>();
			if (spiritComp){
				spiritComp->increaseHealthSpiritGrabbed(m)
			}*/
		}
		meleeRectComp->insertTouchedEntity(collidedEntity, 0);
				
	}

	mToBeCheckedList.clear();
}