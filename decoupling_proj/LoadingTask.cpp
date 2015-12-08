#include "LoadingTask.h"
#include "Constant.h"
#include "GraphicsType.h"
#include "GeneralData.h"
#include "ResourceHolder.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "ScriptUpdateComponent.h"
#include "ScriptAIComponent.h"
#include "VelocityComponent.h"
#include "AnimationComponent.h"
#include "Entity.h"
#include "AutomaticPathComponent.h"
#include "CategoryComponent.h"
#include "CircularPathComponent.h"
#include "TransformableComponent.h"
#include "CollisionHandlerSystem.h"

#include "FireSpiritCollidedEvent.h"
#include "CreateNewEntityEvent.h"
#include "SoulHookStruckEvent.h"
#include "EntityGotDamagedEvent.h"

#include "LightPointComponent.h"
#include "MeleeRectComponent.h"
#include "BoxCollisionComponent.h"
#include "TextDisplayComponent.h"
#include "HarmfulBoxesComponent.h"
#include "HealthComponent.h"
#include "SpiritFormComponent.h"
#include "TimerComponent.h"
#include "DestroyableComponent.h"
#include "PathFinder.h"
#include "Utility.h"
#include "RayCast.h"
#include "FloatableComponent.h"
#include "BuffableComponent.h"


LoadingTask::LoadingTask(GeneralData* generalData)
: mThread(&LoadingTask::runTask, this),
mFinished(false),
mGeneralData(generalData),
mCurrentPercent(0.f),
mCurrentProcess("Loading...."),
mIsThreadTerminated(false)
{
}

LoadingTask::~LoadingTask()
{

}

void LoadingTask::execute()
{
	mFinished = false;
	mCurrentPercent = 0.f;
	mCurrentProcess = "Loading....";
	mThread.launch();
}

bool LoadingTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

bool LoadingTask::isThreadTerminated()
{
	sf::Lock lock(mMutex);
	return mIsThreadTerminated;
}

float LoadingTask::getCompletion()
{
	sf::Lock lock(mMutex);
	return mCurrentPercent;
}

std::string LoadingTask::getCurrentText()
{
	sf::Lock lock(mMutex);
	return mCurrentProcess;
}


void LoadingTask::runTask()
{
	if (!mGeneralData->getLuaState())
		mGeneralData->setLuaState(luaL_newstate());

	

	mGeneralData->setTextureHolderInt(new TextureHolderInt());
	mGeneralData->setTextureHolderStr(new TextureHolderStr());
	//mGeneralData->setPathFinder(new PathFinder());
	mGeneralData->setEntityManager(new EntityManager());
	mGeneralData->setTiledMap(new TiledMapSystem(*mGeneralData->getTextureHolderInt()));
	mGeneralData->setTexturesStringManager(new TextureStringManager());
	
	mGeneralData->setTextureHolder(new TextureHolder());
	mGeneralData->setComponentArranger(new ComponentArranger(mGeneralData));
	mGeneralData->setGameObjectAdder(new GameObjectAdder(mGeneralData));
	
	
	TiledMapSystem& mTiledMap = *(mGeneralData->getTiledMap());
	PathFinder& mPathFinder = *(PathFinder::getInstance());
	//std::map<std::string, SceneNode*>& mSceneLayers = *(mGeneralData->getSceneLayers());
	//SceneNode& mSceneGraph = *(mGeneralData->getSceneGraph());
	EntityManager& mEntityManager = *(mGeneralData->getEntityManager());
	TextureHolderInt& mTexturesInt = *(mGeneralData->getTextureHolderInt());
	TextureHolderStr& mTexturesStr = *(mGeneralData->getTextureHolderStr());
	//WaveSpawnerSystem& waveSpawnerSystem = *(mGeneralData->getWaveSpawnerSystem());

	/*TextureStringManager *manager =  mGeneralData->getTexturesStringManager();
	manager->loadTextures("Media/Textures/box_32_32.png");
	manager->deleteTexture("Media/Textures/box_32_32.png");*/

	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("Media/Textures/box_32_32.png");
	
	delete texture;
	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Loading Map...";
		mCurrentPercent = 0.1f;
	}
	bindLogicsToLuaScript();
	try{
		lua_State* luaState = mGeneralData->getLuaState();
		luaL_dofile(luaState, std::string(scriptDir +
			"SettingScripts/StartupScript.lua").c_str());

		luabridge::LuaRef settingTable = 
			luabridge::getGlobal(luaState, "InitialSetting");
		
		mTiledMap.loadMap(settingTable["initialLevel"].cast<std::string>());
	}
	catch (std::runtime_error message){
		//std::cout << message.what() << std::endl;
		mGeneralData->clearAllData();
		sf::Lock lock(mMutex);
		mIsThreadTerminated = true;
		mThread.terminate();
	}
	//waveSpawnerSystem.readWaveFile(waveDir + "Wave1.txt");

	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Preparing Path Finding...";
		mCurrentPercent = 0.25f;
	}

	


	mPathFinder.initializeAStarNodes(mTiledMap.getTileSize(), mTiledMap.getMapSize());
	mPathFinder.setAdjacentNode(mTiledMap.getTileSize());


	float incrementPerLayer = 0.f;

	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Loading general info and textures...";
		mCurrentPercent = 0.50f;
		incrementPerLayer = (1.f - mCurrentPercent) / static_cast<float>(mTiledMap.getLayersList().size());
	}

	loadManualTexture();

	for (const std::string& layer : mTiledMap.getLayersList()){
		/*SceneNode::Ptr scene(new SceneNode);
		scene->setCategory(Category::Layer);
		scene->setLayerName(layer);
		mSceneLayers[layer] = scene.get();
		mSceneGraph.attachChild(std::move(scene));
		initializeLayerMap(layer);*/

		mEntityManager.addNewLayer(layer);
		initializeLayerMap(layer);

		{
			sf::Lock lock(mMutex);
			mCurrentPercent += incrementPerLayer;
		}
	}

	{
		sf::Lock lock(mMutex);
		mCurrentPercent = 1.f;
		mCurrentProcess = "Finished!";
		mFinished = true;
	}

	
}

//will setup the map and add entities and vertex to the mSceneGraph
//also will add any rectangular object if there is any and will cam setupTmxObject
//for every object in the layer
void LoadingTask::initializeLayerMap(const std::string& layer)
{

	TiledMapSystem& mTiledMap = *mGeneralData->getTiledMap();
	//PathFinder& mPathFinder = *mGeneralData->getPathFinder();
	//std::map<std::string, SceneNode*>& mSceneLayers = *mGeneralData->getSceneLayers();
	//SceneNode& mSceneGraph = *mGeneralData->getSceneGraph();
	TextureHolderInt& mTexturesInt = *mGeneralData->getTextureHolderInt();
	TextureHolderStr& mTexturesStr = *mGeneralData->getTextureHolderStr();

	GameObjectAdder& gameObjAdder = *mGeneralData->getGameObjectAdder();
	/*WaveSpawnerSystem& waveSpawnerSystem = *mGeneralData->getWaveSpawnerSystem();*/

	sf::Vector2f pos;
	GraphicsType::ID type;

	

	for (std::size_t y = 0; y < mTiledMap.getMapSize().y; y++){
		for (std::size_t x = 0; x < mTiledMap.getMapSize().x; x++){
			unsigned int tileId = mTiledMap.At(layer, x, y);
			if (tileId <= 0)
				continue;
			//528, 816
			pos = mTiledMap.coordToPosition(x, y);

			unsigned int tileSetId = mTiledMap.tileToTileSet(tileId);
			sf::IntRect rect = mTiledMap.tileSetToRect(tileSetId, tileId);

			type = static_cast<GraphicsType::ID> (std::stoi(
				mTiledMap.propertyKeyToValue(tileSetId, tileId, "GraphicsType").c_str()));

			if (type == GraphicsType::NotGraphicsType)
				continue;

			//waveSpawnerSystem.insertNewEntityConsData(type, EntityConstructorData(layer, rect, tileSetId));
			/*Tile::Ptr tile(new Tile(mTexturesInt, tileSetId, rect));
			tile->setPosition(pos);

			mLayersScene[layer]->attachChild(std::move(tile));*/
			switch (type){
			case GraphicsType::Tile:
				gameObjAdder.addTile(layer, pos, rect, tileSetId);
				break;
			case GraphicsType::Vertex:
				gameObjAdder.addVertexNode(layer, pos, rect, tileSetId, tileId);
				break;
			case GraphicsType::Spider:
				gameObjAdder.addSpider(layer, pos, rect, tileSetId);
				break;
			case GraphicsType::SwampMoa:
				gameObjAdder.addSwampMoa(layer, pos);
				break;
			case GraphicsType::SkeletonArcher:
				gameObjAdder.addSkeletonArcher(layer, pos);
				break;
			case GraphicsType::Player:
				gameObjAdder.addPlayer(layer, pos);
				break;
			default:
				break;
			}

		}
	}
		

	ObjectRectCollection objects = mTiledMap.layerToObjectData(layer);


	for (ObjectRectData& data : objects){
		sf::Vector2f objPos, objSize;
		//mTexutresStr will load the new texture set on this object
		setupTextureObject(layer, data.objectRect, data.objectName, &objPos, &objSize);
		//we set back the position to its center
		//with the real size founded

		sf::Vector2f topLeftPos = objPos;

		objPos.x = objPos.x + objSize.x / 2;
		objPos.y = objPos.y + objSize.y / 2;

		switch (data.objectType){
		case ObjectsType::Tile:
			//gameObjAdder.addBigTile(layer, data.objectName, objPos);
			break;
		
		default:
			break;
		}
	}

	
	
}


void LoadingTask::setupTextureObject(const std::string& layer,
	const sf::IntRect& objectRect,
	const std::string& objectName,
	sf::Vector2f *objPos,
	sf::Vector2f *objSize)
{
	TiledMapSystem& mTiledMap = *mGeneralData->getTiledMap();
	//PathFinder& mPathFinder = *mGeneralData->getPathFinder();
	//std::map<std::string, SceneNode*>& mSceneLayers = *mGeneralData->getSceneLayers();
	//SceneNode& mSceneGraph = *mGeneralData->getSceneGraph();
	TextureHolderInt& mTexturesInt = *mGeneralData->getTextureHolderInt();
	TextureHolderStr& mTexturesStr = *mGeneralData->getTextureHolderStr();


	sf::Vector2i pos;
	//will contain all the pos of each object collided
	//with the rectangle
	std::vector<sf::Vector2i> containedPoints;
	//contain each object tileID
	std::vector<int> containedIds;

	//first of all we get which part of texture/object we want to rectangle to 
	//copy in the global map
	for (std::size_t y = 0; y < mTiledMap.getMapSize().y; y++){
		for (std::size_t x = 0; x < mTiledMap.getMapSize().x; x++){
			unsigned int tileIdtemp = mTiledMap.At(layer, x, y);
			pos = sf::Vector2i(mTiledMap.coordToPosition(x, y));

			if (!objectRect.contains(pos))
				continue;

			containedIds.push_back(tileIdtemp);

			containedPoints.push_back(pos);
		}
	}
	
	sf::Vector2i firstPos = containedPoints[0];
	//start from 1 since first pos is included
	int width = 1;
	int height = 1;
	//check the width and the height 
	for (const sf::Vector2i& p : containedPoints){
		if (firstPos.x != p.x && firstPos.y == p.y)
			width++;
		else if (firstPos.y != p.y && firstPos.x == p.x)
			height++;
	}
	//we set the position to the top left coordinate first
	objPos->x = firstPos.x - mTiledMap.getTileSize().x / 2;
	objPos->y = firstPos.y - mTiledMap.getTileSize().y / 2;
	//we set the size
	objSize->x = width * mTiledMap.getTileSize().x;
	objSize->y = height * mTiledMap.getTileSize().y;

	//final texture that the new rectangle/object will use
	sf::Texture totalTexture;
	//create the texture based on the width and height
	totalTexture.create(objSize->x, objSize->y);


	//will construct finalTexture by looping each object one by one
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			//temporary hold out all the texture data
			sf::Image img;
			//will hold a single tile image
			//and will be arranged and updated to totalTexture
			sf::Image realImg;
			realImg.create(mTiledMap.getTileSize().x, mTiledMap.getTileSize().y);
			//we get each individual object's tile setId
			unsigned int tileSetId = mTiledMap.tileToTileSet(containedIds[x + width * y]);

			//we get each individual object's IntRect on a spritesheet
			sf::IntRect rect = mTiledMap.tileSetToRect(tileSetId, containedIds[x + width * y]);

			//we get the individual texture(rected already)
			sf::Texture& temp = mTexturesInt.get(tileSetId);
			img = temp.copyToImage();
			//then we copy it pixel by pixel to the realImg
			for (int tileY = 0; tileY < rect.height; tileY++){
				for (int tileX = 0; tileX < rect.width; tileX++){
					realImg.setPixel(tileX, tileY, img.getPixel(rect.left + tileX, rect.top + tileY));
				}
			}
			//then update it into its corresponding position in the finalTexture
			totalTexture.update(realImg, x * mTiledMap.getTileSize().x, y * mTiledMap.getTileSize().y);
		}
	}
	mTexturesStr.loadFromResource(objectName, totalTexture);
}

void LoadingTask::loadManualTexture()
{
	/*TextureHolder& mTextures = *mGeneralData->getTextureHolder();

	mTextures.load(Textures::Player, textureDir + "main_player.png");*/
	//textures.load(Textures::)
}


static luabridge::LuaRef indexFunc(const std::string& key, luabridge::LuaRef& table)
{
	return table["key"];
}

static void testIndex(lua_State* L)
{
	luabridge::LuaRef param = luabridge::LuaRef::fromStack(L, 2);
	if (param.isTable())
	{
		param["value"] = "changed";
		//std::cout << param["value"].cast<std::string>() << std::endl;
		
		//if (param["printVal"].isFunction())
		std::cout << param["printVal"](param) << std::endl;
	}
}

void LoadingTask::bindLogicsToLuaScript()
{
	if (mGeneralData->mCalledLuaStateBinder)
		return;

	
	mGeneralData->mCalledLuaStateBinder = true;
	lua_State* luaState = mGeneralData->getLuaState();

	/*Registering LUABRIDGE classes*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<luabridge::LuaRef>("LuaRef")
		//.addConstructor<void(*)(void)>()
		.addStaticFunction("index", &indexFunc)
		.addStaticFunction("testIndex", &testIndex)
		.addFunction("castInt", &luabridge::LuaRef::cast<int>)
		
		.endClass();

	/**/

	/*Registering SFML Api*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Time>("sfTime")
		.addConstructor<void(*)(void)>()
		
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Vector2f>("sfVector2f")
		.addConstructor<void(*)(void)>()
		.addData("x", &sf::Vector2f::x)
		.addData("y", &sf::Vector2f::y)
		//.addProperty("", sf::Vector3)
		.endClass();
	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Vector2i>("sfVector2i")
		.addConstructor<void(*)(void)>()
		.addData("x", &sf::Vector2i::x)
		.addData("y", &sf::Vector2i::y)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Transformable>("sfTransformable")
		.addConstructor<void(*)(void)>()
		.endClass();
	
	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::FloatRect>("sfFloatRect")
		.addConstructor<void(*)(void)>()
		.addData("width", &sf::FloatRect::width)
		.addData("height", &sf::FloatRect::height)
		.addData("top", &sf::FloatRect::top)
		.addData("left", &sf::FloatRect::left)
		.endClass();
	/**/

	/*Registering C++ standard library*/
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Std")
			.beginClass<std::string>("StdString")
			.endClass()
		.endNamespace();

	/**/

	/*Registering Utility function*/

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("EngineUtil")
		.addFunction("printStringMessage", &Utility::printStringToCmd)
		.addFunction("vectorLength", &Utility::vectorLength)
		.addFunction("unitVector", &Utility::unitVectorScript)
		.addFunction("minusVector", &Utility::minusVector)
		.addFunction("plusVector", &Utility::plusVector)
		.addFunction("offSetRect1AndCheckRect2", &Utility::offSetRect1AndCheckRect2Script)
		.addFunction("randomRange", &Utility::randomRange)
		.addFunction("randomInt", &Utility::randomInt)
		.addFunction("getDotProduct", &Utility::getDotProduct)
		.addFunction("vectorToDegree", &Utility::vectorToDegree)
		.addFunction("degreeToVector", &Utility::degreeToVector)
		.addFunction("multiplyVectorByFloat", &Utility::multiplyVectorByFloat)
		.addFunction("minusVectorI", &Utility::minusVectorI)
		.addFunction("plusVectorI", &Utility::plusVectorI)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<RayCast>("RayCast")
		.addStaticFunction("castRayLine", &RayCast::castRayLineScript)
		.endClass();
	/**/

	/*Register coded C++ event*/
	
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Event")
		.beginClass<EventBase>("EventBase")
		.addConstructor<void(*)(void)>()
		.addConstructor<void(*)(Entity*)>()
		.addFunction("getEntityEventSender", &EventBase::getEntityEventSender)
		.endClass()
		.deriveClass<FireSpiritCollidedEvent, EventBase>("FireSpiritCollidedEvent")
		.addConstructor<void(*)(void)>()
		.addData("mIsClockwise", &FireSpiritCollidedEvent::mIsClockwise)
		.addData("mOwnerEntity", &FireSpiritCollidedEvent::mOwnerEntity)
		.endClass()
		.deriveClass<CreateNewEntityEvent, EventBase>("CreateNewEntityEvent")
		.addConstructor<void(*)(void)>()
		.addFunction("queueEntityScript", &CreateNewEntityEvent::queueEntityScript)
		.endClass()
		.deriveClass<SoulHookStruckEvent, EventBase>("SoulHookStruckEvent")
		.addConstructor<void(*)(void)>()
		.addData("mHookLatestPos", &SoulHookStruckEvent::mHookLatestPos)
		.addData("mCollidedEntityCategory", &SoulHookStruckEvent::mCollidedEntityCategory)
		.endClass()
		.deriveClass<EntityGotDamagedEvent, EventBase>("EntityGotDamagedEvent")
		.addConstructor<void(*)(Entity*)>()
		.endClass()
		.endNamespace();
	/**/

	/*Registering coded C++ classes/function*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<PathFinder>("PathFinder")
		.addStaticFunction("getInstance", &PathFinder::getInstance)
		//.addFunction("")
		.addFunction("sceneToGraph", &PathFinder::sceneToGraphScript)
		.addFunction("sceneToCoordinate", &PathFinder::sceneToCoordinateScript)
		.addFunction("At", &PathFinder::AtScript)
		.addFunction("isRectOverallNodeSafe", &PathFinder::isRectOverallNodeSafe)
		.addFunction("getTileSize", &PathFinder::getTileSize)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<HarmfulBoxData>("HarmfulBoxData")
		.endClass();

	/*luabridge::getGlobalNamespace(luaState)
		.beginClass<AStarNode>("AStarNode")
		.addData("pos", &AStarNode::pos)
		.addData("tile", &AStarNode::tile)
		.endClass();
		*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<EventManager>("EventManager")
		.addStaticFunction("getInstance", &EventManager::getInstance)
		
		.addFunction("addLuaListener", &EventManager::addLuaListener)
		.addFunction("deleteLuaListener", &EventManager::deleteLuaListener)
		.addFunction("deleteSpecificLuaListener", &EventManager::deleteSpecificLuaListener)
		.addFunction("queueEvent", &EventManager::queueScriptEvent)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Entity>("Entity")
		.addConstructor<void(*)(std::size_t)>()
		.addProperty("mId", &Entity::getId)
		.addFunction("compAnimation", &Entity::comp<AnimationComponent>)
		.addFunction("compVelocity", &Entity::comp<VelocityComponent>)
		.addFunction("compScriptAI", &Entity::comp<ScriptAIComponent>)
		.addFunction("compAutomaticPath", &Entity::comp<AutomaticPathComponent>)
		.addFunction("compCategory", &Entity::comp<CategoryComponent>)
		.addFunction("compTransform", &Entity::comp<TransformableComponent>)
		.addFunction("compCircularPath", &Entity::comp<CircularPathComponent>)
		.addFunction("compLightPoint", &Entity::comp<LightPointComponent>)
		.addFunction("compMeleeRect", &Entity::comp<MeleeRectComponent>)
		.addFunction("compBoxCollision", &Entity::comp<BoxCollisionComponent>)
		.addFunction("compTextDisplay", &Entity::comp<TextDisplayComponent>)
		.addFunction("compHarmfulBoxes", &Entity::comp<HarmfulBoxesComponent>)
		.addFunction("compHealth", &Entity::comp<HealthComponent>)
		.addFunction("compSpiritForm", &Entity::comp<SpiritFormComponent>)
		.addFunction("compScriptUpdate", &Entity::comp<ScriptUpdateComponent>)
		.addFunction("compTimer", &Entity::comp<TimerComponent>)
		.addFunction("compFloatable", &Entity::comp<FloatableComponent>)
		.addFunction("compDestroyable", &Entity::comp<DestroyableComponent>)
		.addFunction("compBuffable", &Entity::comp<BuffableComponent>)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<CollisionHandlerSystem>("CollisionHandlerSystem")
		.addFunction("getCalculatedSafeOffSet", &CollisionHandlerSystem::getCalculatedSafeOffSet)
		.endClass();
	/*Component classes begin*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<AnimationComponent>("AnimationComponent")
		.addConstructor<void(*)(Entity*)>()
		.addFunction("setAnimation", &AnimationComponent::setAnimation)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<CircularPathComponent>("CircularPathComponent")
		.addConstructor<void(*)(Entity*)>()
		.addProperty("mIsClockwise", &CircularPathComponent::isClockwise, &CircularPathComponent::setClockwise)
		.addFunction("increaseCurRadius", &CircularPathComponent::increaseCurRadius)
		.addFunction("decreaseCurRadius", &CircularPathComponent::decreaseCurRadius)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<VelocityComponent>("VelocityComponent")
		.addFunction("getVelocityX", &VelocityComponent::getVelocityX)
		.addFunction("getVelocityY", &VelocityComponent::getVelocityY)
		.addFunction("getVelocity", &VelocityComponent::getVelocity)
		.addFunction("setVelocity", &VelocityComponent::setVelocityScript)
		.addFunction("setVelocityX", &VelocityComponent::setVelocityX)
		.addFunction("setVelocityY", &VelocityComponent::setVelocityY)
		.addFunction("getFacingDirectionY", &VelocityComponent::getFacingDirectionY)
		.addFunction("getFacingDirectionX", &VelocityComponent::getFacingDirectionX)
		.addFunction("setSpeedIdentifier", &VelocityComponent::setSpeedIdentifier)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TextDisplayComponent>("TextDisplayComponent")
		.addFunction("setString", &TextDisplayComponent::setString)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<LightPointComponent>("LightPointComponent")
		.addProperty("mNoLightForOneFrame", &LightPointComponent::noLightForOneFrame, 
		&LightPointComponent::setNoLightForOneFrame)
		.addFunction("setCurLightRadius", &LightPointComponent::setCurLightRadius)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptAIComponent>("ScriptAIComponent")
		.addFunction("setCurState", &ScriptAIComponent::setCurState)
		.addFunction("getCurAIStateName", &ScriptAIComponent::getCurAIStateName)
		.addFunction("getCurAIState", &ScriptAIComponent::getCurAIState)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptAIState>("ScriptAIState")
		//.addConstructor<void(*)(Entity*)>()
		.addFunction("getLuaState", &ScriptAIState::getLuaReferenceToState)
		//.addFunction("getLuaStateRef", &ScriptAIState::getLuaStateRef)
		.endClass();

	/*luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptAIState>("ScriptAIState")
		.addConstructor<void(*)(Entity*)>()
		.endClass();*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TimerComponent>("TimerComponent")
		.addFunction("updateTimer", &TimerComponent::updateTimer)
		.addFunction("setNewTimeLimitToTimer", &TimerComponent::setNewTimeLimitToTimer)
		.addFunction("isTimerElapsed", &TimerComponent::isTimerElapsed)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<AutomaticPathComponent>("AutomaticPathComponent")
		.addFunction("setDestToCalculate", &AutomaticPathComponent::setDestToCalculate)
		.addFunction("isAutomaticPathsEmpty", &AutomaticPathComponent::isAutomaticPathsEmpty)
		.addFunction("clearAutomaticPaths", &AutomaticPathComponent::clearAutomaticPaths)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<CategoryComponent>("CategoryComponent")
		.addFunction("getCategory", &CategoryComponent::getCategory)
		.endClass();
	luabridge::getGlobalNamespace(luaState)
		.beginClass<TransformableComponent>("TransformableComponent")
		.addFunction("getWorldPosition", &TransformableComponent::getWorldPosition)
		.addFunction("setPosition", &TransformableComponent::setScriptPosition)
		.addFunction("move", &TransformableComponent::moveScript)
		.addFunction("rotate", &TransformableComponent::rotate)
		.addFunction("setRotation", &TransformableComponent::setRotation)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<MeleeRectComponent>("MeleeRectComponent")
		.addFunction("startMeleeRect", &MeleeRectComponent::startMeleeRectScript)
		.addFunction("meleeRectIsUpdating", &MeleeRectComponent::meleeRectIsUpdating)
		.addFunction("meleeRectIsDelaying", &MeleeRectComponent::meleeRectIsDelaying)
		.addFunction("isRecovering", &MeleeRectComponent::isRecovering)
		.addFunction("isVulnerable", &MeleeRectComponent::isVulnerable)
		.addFunction("stopMeleeRectUpdating", &MeleeRectComponent::stopMeleeRectUpdating)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<BoxCollisionComponent>("BoxCollisionComponent")
		.addFunction("getTransformedRect", &BoxCollisionComponent::getTransfromedRect)
		.addData("mBoundingRect", &BoxCollisionComponent::mBoundingRect)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<HarmfulBoxesComponent>("HarmfulBoxesComponent")
		.addFunction("activateBox", &HarmfulBoxesComponent::activateBox)
		.addFunction("deactivateBox", &HarmfulBoxesComponent::deactivateBox)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<HealthComponent>("HealthComponent")
		.addFunction("damage", &HealthComponent::damage)
		.addFunction("increaseHealth", &HealthComponent::increaseHealth)
		.addFunction("getCurrentHealth", &HealthComponent::getCurrentHealth)
		.addFunction("setIsImmune", &HealthComponent::setIsImmune)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<SpiritFormComponent>("SpiritFormComponent")
		.addFunction("isOnSpiritForm", &SpiritFormComponent::isOnSpiritForm)
		.addFunction("getDamageToPassedThroughCategory", &SpiritFormComponent::getDamageToPassedThroughCategory)
		.addFunction("insertExcemptionEntity", &SpiritFormComponent::insertExcemptionEntity)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptUpdateComponent>("ScriptUpdateComponent")
		.addFunction("getMemberVariables", &ScriptUpdateComponent::getScriptMemberVariables)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<FloatableComponent>("FloatableComponent")
		.addFunction("setIsFloating", &FloatableComponent::setIsFloating)
		.addFunction("isFloating", &FloatableComponent::isFloating)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<DestroyableComponent>("DestroyableComponent")
		.addFunction("isDestroyed", &DestroyableComponent::isDestroyed)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<BuffableComponent>("BuffableComponent")
		.addFunction("insertNewBuff", &BuffableComponent::insertNewBuffFromScript)
		.addFunction("getFirstBuffByName", &BuffableComponent::getFirstBuffByName)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<BuffScript>("BuffScript")
		.addFunction("getLuaReferenceToBuff", &BuffScript::getLuaReferenceToBuff)
		.endClass();

	/*Component classes end here~*/


	/**/

	/*Path Finding classes related begin*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<AStarNode>("AStarNode")
		.addConstructor<void(*)(void)>()
		.addData("pos", &AStarNode::pos)
		.addData("tile", &AStarNode::tile)
		.endClass();

	/*Path findng classes related end*/


	/*Coded C++ classes end here*/
}