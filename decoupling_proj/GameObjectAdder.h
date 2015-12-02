#pragma once
//#include "GeneralData.h"

#include "EntityManager.h"
#include "ResourceHolder.h"
#include "TiledMapSystem.h"
#include "PathFinder.h"
#include "ComponentArranger.h"

class GeneralData;
class EventBase;

class GameObjectAdder{
public:
	GameObjectAdder(GeneralData* generalData);
	~GameObjectAdder();


	void addTile(const std::string& layer, const sf::Vector2f& pos,
		const sf::IntRect& intRect, int tileSetId);
	void addVertexNode(const std::string& layer, const sf::Vector2f& pos,
		const sf::IntRect& intRect, int tileSetId, int tileId);
	void addSpider(const std::string& layer, const sf::Vector2f& pos,
		const sf::IntRect& intRect, int tileSetId);
	void addSwampMoa(const std::string& layer, const sf::Vector2f& pos);
	void addSkeletonArcher(const std::string& layer, const sf::Vector2f& pos);
	void addPlayer(const std::string& layer, const sf::Vector2f& pos);

	
	void addEntityFromEventScript(EventBase* eventBase);
	void addEntityFromEvent(EventBase* eventBase);

private:
	EntityManager& mEntityManager;
	TextureHolder& mTextures;
	TiledMapSystem& mTiledMap;
	TextureHolderInt& mTexturesInt;
	TextureHolderStr& mTexturesStr;
	//PathFinder& mPathFinder;
	ComponentArranger& mComponentArranger;
	GeneralData* mGeneralData;
};

