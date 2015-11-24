#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "Entity.h"
#include <string>
#include <SFML/Graphics/Rect.hpp>

class EntityManager{
public:
	typedef std::unique_ptr<EntityManager> Ptr;
	//typedef std::unordered_map < std::string, std::vector<Entity::Ptr> >EntitiesLayers;
	typedef std::pair < std::string, std::vector<Entity::Ptr> > EntitiesPair;
	typedef std::vector < EntitiesPair >EntitiesLayers;
	typedef std::vector<Entity::Ptr>::const_iterator EntityIter;
public:
	EntityManager();
	~EntityManager();


	Entity* createEntity(const std::string& layer);
	std::vector<Entity*> getEnts();

	std::vector<Entity*> getEntsByBound(const sf::FloatRect& worldBound);
	template <class T> std::vector<Entity*> getEntsByComp();
	//template <class T, class... Other> std::vector<Entity*> getEntsByComps();

	Entity* getEntityById(std::size_t id) const;

	std::vector<Entity*> getEntitiesByCategoryLayer(std::size_t category,
		const std::string& layer);

	std::vector<Entity*> getEntitiesByComponentLayer(ComponentIdentifier::ID componentIdent,
		const std::string& layer);

	void addNewLayer(const std::string& layer);

	void removeRemoveableEntities();
private:
	EntitiesLayers::iterator findIterByLayer(const std::string& layer);

	bool isEntityRemoveable(Entity* entity) const;
private:
	EntitiesLayers mEntities;
	static std::size_t mCurrentEntityIncremental;
};

template <class T>
std::vector<Entity*> EntityManager::getEntsByComp()
{
	std::vector<Entity*> ents;

	for (auto& iter : mEntities){
		std::vector<Entity::Ptr> listOfEnts = iter.second;
		for (unsigned int i = 0; i < listOfEnts.size(); ++i)
		{
			if (listOfEnts[i]->hasComp<T>())
			{
				ents.push_back(listOfEnts[i].get());
			}
		}
	}

	return ents;
}

/*template <class T, class... Other> std::vector<Entity*> EntityManager::getEntsByComps()
{
	std::vector<Entity*> ents;

	for (unsigned int i = 0; i < mEntities.size(); ++i)
	{
		if (mEntities[i]->hasComps<T, Other...>())
		{
			ents.push_back(mEntities[i]);
		}
	}

	return ents;
}*/

