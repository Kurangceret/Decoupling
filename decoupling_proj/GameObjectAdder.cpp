#include "GameObjectAdder.h"
#include "GeneralData.h"
#include "Entity.h"
#include "BoxCollisionComponent.h"
#include "TransformableComponent.h"
#include "VelocityComponent.h"
#include "SpriteComponent.h"
#include "CategoryComponent.h"
#include "VertexNodeComponent.h"
#include "Utility.h"
#include "Constant.h"
#include "EventManager.h"
#include "HealthComponent.h"
#include "CreateNewEntityEvent.h"
#include "SpiritGrabberComponent.h"

GameObjectAdder::GameObjectAdder(GeneralData* generalData)
:mEntityManager(*generalData->getEntityManager()),
mTexturesInt(*generalData->getTextureHolderInt()),
mTexturesStr(*generalData->getTextureHolderStr()),
mTextures(*generalData->getTextureHolder()),
mTiledMap(*generalData->getTiledMap()),
mComponentArranger(*generalData->getComponentArranger()),
mGeneralData(generalData)
{
	EventManager* eventManager = EventManager::getInstance();
		
	eventManager->addListener(EventType::CreateNewEntity,
		EventCallBackFunc(Utility::convertPointerToAddress(this),
		std::bind(&GameObjectAdder::addEntityFromEventScript, this, std::placeholders::_1)));

	//luabridge::Stack<luabridge::LuaRef>::push;
}


GameObjectAdder::~GameObjectAdder()
{
	EventManager::getInstance()->deleteListener(this);
}


void GameObjectAdder::addTile(const std::string& layer, const sf::Vector2f& pos,
	const sf::IntRect& intRect, int tileSetId)
{
	Entity* tile = mEntityManager.createEntity(layer);
	tile->comp<TransformableComponent>()->setPosition(pos);

	PathFinder* pathFinder = PathFinder::getInstance();
	pathFinder->sceneToGraph(pos)->tile = tile;

	tile->comp<CategoryComponent>()->setCategory(Category::Tile);

	sf::Sprite& sprite = tile->comp<SpriteComponent>()->mSprite;

	sprite.setTexture(mTexturesInt.get(tileSetId));
	sprite.setTextureRect(intRect);
	tile->comp<TransformableComponent>()->setOrigin(sf::Vector2f(intRect.width / 2.f, 
		intRect.height / 2.f));
	
	
	tile->comp<BoxCollisionComponent>()->mBoundingRect = sf::FloatRect(0, 0, sprite.getGlobalBounds().width, 
		sprite.getGlobalBounds().height);
	
	mComponentArranger.readFromLuaScript(tile, scriptDir + "NormalTileScript.lua",
		"NormalTile", mGeneralData->getLuaState());
}

void GameObjectAdder::addVertexNode(const std::string& layer, const sf::Vector2f& pos,
	const sf::IntRect& intRect, int tileSetId, int tileId)
{
	std::string value = mTiledMap.propertyKeyToValue(tileSetId, tileId, "IsFallable");
	if (value != ""){
		

		int boolVal = static_cast<int>(std::stoi(value));
		AStarNode* curTileNode = PathFinder::getInstance()->sceneToGraph(pos);

		if (boolVal == 1)
			curTileNode->isFallable = true;
	}

	std::vector<Entity*> vertexNodes;
	vertexNodes = mEntityManager.getEntitiesByComponentLayer(ComponentIdentifier::VertexNodeComponent, layer);
		
	for (auto vertexNode : vertexNodes){
		VertexNodeComponent* vertexComp = vertexNode->comp<VertexNodeComponent>();

		if (vertexComp->getTileSetId() == tileSetId){
			vertexComp->addTile(pos, intRect);
			return;
		}
	}

	Entity* vertexNode = mEntityManager.createEntity(layer);
	vertexNode->comp<VertexNodeComponent>()->initializeTextureData(&mTexturesInt.get(tileSetId), 
		tileSetId);
	vertexNode->comp<VertexNodeComponent>()->addTile(pos, intRect);
	vertexNode->comp<CategoryComponent>()->setCategory(Category::VertexNode);

	

	
}

void GameObjectAdder::addSpider(const std::string& layer, const sf::Vector2f& pos,
	const sf::IntRect& intRect, int tileSetId)
{
	Entity* spider = mEntityManager.createEntity(layer);
	mComponentArranger.readFromLuaScript(spider, scriptDir +
		"SpiderScript.lua",
		"Spider", mGeneralData->getLuaState());
	spider->comp<TransformableComponent>()->setPosition(pos);
}

void GameObjectAdder::addSwampMoa(const std::string& layer, const sf::Vector2f& pos)
{
	Entity* swampMoa = mEntityManager.createEntity(layer);
	mComponentArranger.readFromLuaScript(swampMoa,scriptDir +
		"SwampMoaScript.lua",
		"SwampMoa", mGeneralData->getLuaState());
	swampMoa->comp<TransformableComponent>()->setPosition(pos);
}

void GameObjectAdder::addSkeletonArcher(const std::string& layer, const sf::Vector2f& pos)
{
	Entity* skeletonArcher = mEntityManager.createEntity(layer);
	mComponentArranger.readFromLuaScript(skeletonArcher, scriptDir +
		"SkeletonArcherScript.lua",
		"SkeletonArcher", mGeneralData->getLuaState());
	skeletonArcher->comp<TransformableComponent>()->setPosition(pos);
}

void GameObjectAdder::addFireman(const std::string& layer, const sf::Vector2f& pos)
{
	Entity* fireMan = mEntityManager.createEntity(layer);
	mComponentArranger.readFromLuaScript(fireMan, scriptDir +
		"FiremanScript.lua",
		"Fireman", mGeneralData->getLuaState());
	fireMan->comp<TransformableComponent>()->setPosition(pos);
}

void GameObjectAdder::addEntityFromEventScript(EventBase* eventBase)
{
	if (!eventBase || eventBase->getEventType() != EventType::CreateNewEntity)
		return;
	
	CreateNewEntityEvent* createNewEntityEvent = dynamic_cast<CreateNewEntityEvent*>(eventBase);

	while (!createNewEntityEvent->mEntitiesToBeCreated.empty()){
		auto& topRef = createNewEntityEvent->mEntitiesToBeCreated.front();
		
		Entity* newEntity = mEntityManager.createEntity(topRef.mLayer);
		mComponentArranger.readFromLuaScript(newEntity, topRef.mScriptDir, 
			topRef.mTableName, mGeneralData->getLuaState());

		

		if (topRef.mCustomInitializeData.get() && topRef.mCustomInitializeLuaFunc.get()){
			try{
				(*topRef.mCustomInitializeLuaFunc)(newEntity, (*topRef.mCustomInitializeData));
			}
			catch (luabridge::LuaException e){
				std::cout << "Add Entity from script : " << e.what() << std::endl;
			}
		}

		if (topRef.mEngineInitializeFunc.get())
			(*topRef.mEngineInitializeFunc)(newEntity);
		
		createNewEntityEvent->mEntitiesToBeCreated.pop();
	}
}


void GameObjectAdder::addPlayer(const std::string& layer, const sf::Vector2f& pos)
{
	Entity* player = mEntityManager.createEntity(layer);
	mComponentArranger.readFromLuaScript(player, scriptDir +
		"PlayerScript.lua",
		"Player", mGeneralData->getLuaState());
	player->comp<TransformableComponent>()->setPosition(pos);
	player->comp<HealthComponent>()->setDamagedReactor(
		[](float& damageNum, Entity* damagerEntity)
	{
		SpiritGrabberComponent* spiritComp = damagerEntity->nonCreateComp<SpiritGrabberComponent>();
		if (!spiritComp)
			return;
		spiritComp->increaseHealthSpiritGrabbed(damageNum / 2.f);
	});

	mGeneralData->setPlayer(player);
}
