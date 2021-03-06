#include "EntityManager.h"
#include "CategoryComponent.h"
#include "HealthComponent.h"
#include "BoxCollisionComponent.h"
#include "TransformableComponent.h"
#include "DestroyableComponent.h"
#include "EntityParentComponent.h"
#include "EntityChildrenComponent.h"
#include "FunctionCallerManager.h"
#include <algorithm>

std::size_t EntityManager::mCurrentEntityIncremental = 0;

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}


Entity* EntityManager::createEntity(const std::string& layer)
{
	auto& iter = findIterByLayer(layer);
	if (iter == mEntities.end())
		return nullptr;

	Entity::Ptr entity(new Entity(mCurrentEntityIncremental++));
	//Entity* returnEnt = entity.get();
		
	iter->second.push_back(std::move(entity));
	//iter->second.push_back(std::move(Entity::Ptr(new Entity(mCurrentEntityIncremental++))));
	return iter->second.back().get();
}

EntityManager::EntitiesLayers::iterator 
EntityManager::findIterByLayer(const std::string& layer)
{
	return std::find_if(mEntities.begin(), mEntities.end(), [&](EntitiesPair& pair){
		if (pair.first == layer)
			return true;
		return false;
	});
}

void EntityManager::addNewLayer(const std::string& layer)
{
	mEntities.push_back(std::make_pair(layer, std::vector<Entity::Ptr>()));
	//mEntities.insert(mEntities.end(), std::make_pair(layer, std::vector<Entity::Ptr>()));
	//mEntities[layer] = std::vector<Entity::Ptr>();
}

Entity* EntityManager::getEntityById(std::size_t id) const
{
	for (auto& iter : mEntities){
		for (auto& ptr : iter.second){
			if (ptr.get()->getId() == id)
				return ptr.get();
		}
	}
	return nullptr;
}

std::vector<Entity*> EntityManager::getEntitiesByCategoryLayer(std::size_t category, 
	const std::string& layer)
{
	std::vector<Entity*> entities;

	auto& iter = findIterByLayer(layer);
	if (iter == mEntities.end())
		return entities;
	
	for (auto& ptr : iter->second){
		if (!ptr.get()->hasComp<CategoryComponent>())
			continue;
		if (ptr.get()->comp<CategoryComponent>()->getCategory() & category)
			entities.push_back(ptr.get());
	}
	return entities;
}

std::vector<Entity*> EntityManager::getEntitiesByComponentLayer(ComponentIdentifier::ID componentIdent,
	const std::string& layer)
{
	std::vector<Entity*> entities;

	auto iter = findIterByLayer(layer);
	if (iter == mEntities.end())
		return entities;

	for (auto& ptr : iter->second){
		//if (!(ptr.get()->getComponentsIdentifiers().test(componentIdent)))
		if (!(ptr.get()->getComponentsIdentifiers() & componentIdent))
			continue;
		
		entities.push_back(ptr.get());
	}
	return entities;
}

std::vector<Entity*> EntityManager::getEnts()
{
	std::vector<Entity*> entities;

	typedef std::vector<Entity::Ptr>::const_iterator EntityIter;

	for (auto& iter : mEntities){
		
		for (EntityIter i = iter.second.begin(); i != iter.second.end();)
		{
			if (!isEntityRemoveable(i->get())){
				entities.push_back(i->get());
				i++;
				continue;
			}
			
			///mRemoveableIter.push_back(i);
			i = iter.second.erase(i);
			//i++;
		}
		/*for (auto& ptr : iter.second){
			entities.push_back(ptr.get());
		}*/

	}

	return entities;
}

std::vector<Entity*> EntityManager::getEntsByBound(const sf::FloatRect& worldBound)
{
	std::vector<Entity*> entities;

	typedef std::vector<Entity::Ptr>::const_iterator EntityIter;

	/*for (auto& iter : mEntities){
		std::vector<Entity*> curLayerEntities;
		for (EntityIter i = iter.second.begin(); i != iter.second.end();)
		{
			Entity *curEntity = i->get();
			if (isEntityRemoveable(curEntity)){
				if (curEntity->hasComp<EntityParentComponent>()){
					Entity* parentEntity = curEntity->comp<EntityParentComponent>()->getParent();
					parentEntity->comp<EntityChildrenComponent>()->removeChild(curEntity);
				}

				i = iter.second.erase(i);
				continue;
			}

			if (!curEntity->hasComp<BoxCollisionComponent>() || curEntity->comp
				<BoxCollisionComponent>()->getTransfromedRect().intersects(worldBound))
			{
				//entities.push_back(i->get());
				curLayerEntities.push_back(i->get());
			}

			//entities.push_back(i->get());
			i++;
		}
		std::sort(curLayerEntities.begin(), curLayerEntities.end()
			, [](Entity* entity1, Entity* entity2) -> bool
		{
			TransformableComponent* entity1Transform = entity1->nonCreateComp<TransformableComponent>();
			TransformableComponent* entity2Transform = entity2->nonCreateComp<TransformableComponent>();

			if (!entity1Transform || !entity2Transform)
				return false;

			sf::Vector2f entity1WorldPos = entity1Transform->getWorldPosition(true);
			sf::Vector2f entity2WorldPos = entity2Transform->getWorldPosition(true);

			return ((entity1WorldPos.y < entity2WorldPos.y) ||
				(entity1WorldPos.y == entity2WorldPos.y &&
				entity1WorldPos.x < entity2WorldPos.x));
				
		});

		std::copy(curLayerEntities.begin(), curLayerEntities.end(), 
			std::back_inserter(entities));

		curLayerEntities.clear();
	}*/
	int previousIndex = 0;
	int latestIndex = -1;
	for (auto& iter : mEntities){
		
		auto finalIter = std::remove_if(iter.second.begin(), iter.second.end(), 
			[&](Entity::Ptr& entityPtr) -> bool
		{
			Entity *curEntity = entityPtr.get();
			FunctionCallerManager::getInstance()->checkEntityForFunctionCaller(curEntity);

			if (isEntityRemoveable(curEntity)){
				if (curEntity->hasComp<EntityParentComponent>()){
					Entity* parentEntity = curEntity->comp<EntityParentComponent>()->getParent();
					parentEntity->comp<EntityChildrenComponent>()->removeChild(curEntity);
				}
				return true;
			}

			if (!curEntity->hasComp<BoxCollisionComponent>() || curEntity->comp
				<BoxCollisionComponent>()->getTransfromedRect().intersects(worldBound))
			{
				entities.push_back(curEntity);
				latestIndex++;
			}
			return false;
		});
				
		iter.second.erase(finalIter, iter.second.end());

		if (latestIndex > previousIndex){
			std::sort(entities.begin() + previousIndex, entities.begin() + latestIndex
				, [](Entity* entity1, Entity* entity2) -> bool
			{
				TransformableComponent* entity1Transform = entity1->nonCreateComp<TransformableComponent>();
				TransformableComponent* entity2Transform = entity2->nonCreateComp<TransformableComponent>();

				if (!entity1Transform || !entity2Transform)
					return false;

				sf::Vector2f entity1WorldPos = entity1Transform->getWorldPosition(true);
				sf::Vector2f entity2WorldPos = entity2Transform->getWorldPosition(true);

				return ((entity1WorldPos.y < entity2WorldPos.y) ||
					(entity1WorldPos.y == entity2WorldPos.y &&
					entity1WorldPos.x < entity2WorldPos.x));

			});
		}
		previousIndex = latestIndex;
		
	}
	return entities;
}


bool EntityManager::isEntityRemoveable(Entity* entity) const
{
	/*if (entity->hasComp<CategoryComponent>() && 
		entity->comp<CategoryComponent>()->getCategory() & Category::Player)
		return false;

	if (entity->hasComp<HealthComponent>()){
		HealthComponent* healthComp = entity->comp<HealthComponent>();
		return healthComp->getCurrentHealth() <= 0;
	}*/
	if (!entity->hasComp<DestroyableComponent>())
		return false;

	DestroyableComponent* destroyableComp = entity->nonCreateComp<DestroyableComponent>();

	return (destroyableComp && destroyableComp->isRemoveable());
	
	//return false;
}