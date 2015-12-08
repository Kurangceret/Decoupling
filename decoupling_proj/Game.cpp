#include "Game.h"
#include "TransformableComponent.h"
#include "SpriteComponent.h"
#include "VelocityComponent.h"
#include "CategoryComponent.h"
#include "Utility.h"
#include "MovementSystem.h"
#include "RenderingSystem.h"
#include "AnimationSystem.h"
#include "Constant.h"
#include "BoxToBoxHandlerSystem.h"
#include "BoxCollisionComponent.h"
#include "ScriptUpdateComponent.h"
#include "AutomaticPathComponent.h"
#include "AutomaticMovementSystem.h"
#include "PlayerLogicSystem.h"
#include "VertexNodeComponent.h"
#include "MeleeRectComponent.h"
#include "MeleeRectSystem.h"
#include "MeleeRectToBoxHandlerSystem.h"
#include "RotatedBoxToBoxHandlerSystem.h"
#include "EventManager.h"
#include "ScriptAIComponent.h"
#include "AvoidanceBoxComponent.h"
#include "LightPointComponent.h"
#include "CircularMovementSystem.h"
#include "HarmfulBoxesToBoxHandlerSystem.h"
#include "HarmfulBoxesComponent.h"
#include "RotatedBoxCollisionComponent.h"
#include "SpiritCoreComponent.h"
#include "FallingSystem.h"
#include "EntityChildrenComponent.h"
#include "HealthComponent.h"
#include "BuffableComponent.h"
#include <SFML/Graphics/Text.hpp>
#include <LTBL\Light\Light_Point.h>
#include <LTBL\Utils.h>


Game::Game(GeneralData* generalData, sf::RenderWindow& renderWindow)
:mWindow(renderWindow),
mTextures(*generalData->getTextureHolder()),
mTexturesInt(*generalData->getTextureHolderInt()),
mTiledMap(*generalData->getTiledMap()),
mEntityManager(*generalData->getEntityManager()),

mTexturesStringManager(*generalData->getTexturesStringManager()),
mPlayer(generalData->getPlayer()),
mComponentArranger(*generalData->getComponentArranger()),
//mTestLight(nullptr),
mFogOfWar(generalData),
mFourTree(
	25, 
	3,
	sf::FloatRect(0.f, 0.f, 
	mTiledMap.getMapSize().x * mTiledMap.getTileSize().x, 
	mTiledMap.getMapSize().y * mTiledMap.getTileSize().y)
	),
mLightSystem(*generalData->getLightSystem()),
mCamera(mWindow, mWindow.getSize().x, mWindow.getSize().y, 1.f, sf::Vector2f(
	mTiledMap.getMapSize().x * mTiledMap.getTileSize().x, mTiledMap.getMapSize().y * mTiledMap.getTileSize().y)),
mAvoidBoxSystem(nullptr),
mLuaState(generalData->getLuaState())
{
	
	
	//mFogOfWar.createFog(sf::Vector2u(mTiledMap.getMapSize() * mTiledMap.getTileSize().x));
	mLightSystem.m_useBloom = false;

	//mLightSystem.RemoveLight();
	/*mPlayer = mEntityManager.createEntity("Foreground");
	mComponentArranger.readFromLuaScript(mPlayer, scriptDir + 
		"PlayerScript.lua", 
		"Player", generalData->getLuaState());*/
		
	initializeSystem();
	mFont.loadFromFile(fontDir + "Sansation.ttf");
}


Game::~Game()
{
}


void Game::initializeSystem()
{
	mMeleeRectSystem.reset(new MeleeRectSystem());



	AnimationSystem::Ptr animationSystem(new AnimationSystem());
	mAnimationSystems.addSystem(animationSystem.get());
	mSystems.push_back(std::move(animationSystem));

	
	if (luaL_dofile(mLuaState, std::string(scriptDir + 
		"PlayerStateDataScript.lua").c_str()) == 0)
	{
		luaL_openlibs(mLuaState);

		lua_pcall(mLuaState, 0, 0, 0);

		luabridge::LuaRef luaRef = luabridge::getGlobal(mLuaState, "PlayerStates");
		
		PlayerLogicSystem::Ptr playerLogicSystem(new PlayerLogicSystem(mPlayer, luaRef));
		mGameLogicSystems.addSystem(playerLogicSystem.get());
		mPlayerLogicSystem = playerLogicSystem.get();
		mSystems.push_back(std::move(playerLogicSystem));
	}
	else{
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
	}



	AutomaticMovementSystem::Ptr automaticMoveSystem(new AutomaticMovementSystem());
	mMovementSystems.addSystem(automaticMoveSystem.get());
	mSystems.push_back(std::move(automaticMoveSystem));

	CircularMovementSystem::Ptr circularMoveSystem(new CircularMovementSystem());
	mMovementSystems.addSystem(circularMoveSystem.get());
	mSystems.push_back(std::move(circularMoveSystem));

	AvoidanceBoxSystem::Ptr avoidBoxSystem(new AvoidanceBoxSystem());
	mMovementSystems.addSystem(avoidBoxSystem.get());
	mAvoidBoxSystem = avoidBoxSystem.get();

	mSystems.push_back(std::move(avoidBoxSystem));

	MovementSystem::Ptr movementSystem(new MovementSystem());
	mMovementSystems.addSystem(movementSystem.get());
	mMovementSystem = movementSystem.get();
	mSystems.push_back(std::move(movementSystem));

	RenderingSystem::Ptr renderingSystem(new RenderingSystem(mWindow));
	mRenderingSystems.addSystem(renderingSystem.get());
	mSystems.push_back(std::move(renderingSystem));



	BoxToBoxHandlerSystem::Ptr boxHandlerSystem(new BoxToBoxHandlerSystem());
	mCollisionHandlerSystems.addSystem(boxHandlerSystem.get());
	mSystems.push_back(std::move(boxHandlerSystem));

	HarmfulBoxesToBoxHandlerSystem::Ptr harmfulBoxToBoxSystem(new HarmfulBoxesToBoxHandlerSystem());
	mCollisionHandlerSystems.addSystem(harmfulBoxToBoxSystem.get());
	mSystems.push_back(std::move(harmfulBoxToBoxSystem));

	MeleeRectToBoxHandlerSystem::Ptr meleeRectToBoxHandlerSystem(new MeleeRectToBoxHandlerSystem(mCommonBattleLogic));
	//mCollisionHandlerSystems.addSystem(meleeRectToBoxHandlerSystem.get());
	mMeleeRectToBoxHandlerSystem = meleeRectToBoxHandlerSystem.get();
	mSystems.push_back(std::move(meleeRectToBoxHandlerSystem));

	RotatedBoxToBoxHandlerSystem::Ptr rotatedBoxToBoxHandlerSystem(new RotatedBoxToBoxHandlerSystem());
	mCollisionHandlerSystems.addSystem(rotatedBoxToBoxHandlerSystem.get());
	mSystems.push_back(std::move(rotatedBoxToBoxHandlerSystem));

	FallingSystem::Ptr fallingSystem(new FallingSystem());
	mFallingSystem = fallingSystem.get();
	mSystems.push_back(std::move(fallingSystem));
}

void Game::draw()
{
	mWindow.setView(mCamera.getView());

	std::vector<Entity*> frontLightEntities;

	for (auto& entity : mCurEntitiesList){
		if (entity->hasComp<LightPointComponent>()){
			LightPointComponent* lightComp = entity->comp<LightPointComponent>();
			if (lightComp->getCurLightRadius() == 0.f){
				frontLightEntities.push_back(entity);
				continue;
			}
		}

		mRenderingSystems.update(entity, sf::Time());
		if (entity->hasComp<VertexNodeComponent>())
			entity->comp<VertexNodeComponent>()->drawVertexs(mWindow);
		
		if (entity->hasComp<HarmfulBoxesComponent>())
			entity->comp<HarmfulBoxesComponent>()->drawHarmfulBoxes(mWindow);
		if (entity->hasComp<RotatedBoxCollisionComponent>())
			Utility::drawRotatedRect(mWindow, &entity->comp<RotatedBoxCollisionComponent>()->getTransformedRotatedRect(), sf::Color::Black);

		if (entity->hasComp<AutomaticPathComponent>()){
			//entity->comp<AutomaticPathComponent>()->drawAutomaticPaths(mWindow);
		}

		/*if (entity->hasComp<AvoidanceBoxComponent>()){
			AvoidanceBoxComponent* avoidBoxComp = entity->comp<AvoidanceBoxComponent>();

			Utility::drawRotatedRect(mWindow, &avoidBoxComp->getRayRect(), sf::Color::Red);
			if (avoidBoxComp->justGotRayed()){
				sf::Text text;
				text.setFont(mFont);
				text.setString("Raying..");
				text.setCharacterSize(10);
				text.setPosition(entity->comp<TransformableComponent>()->getWorldPosition());
				mWindow.draw(text);
				//avoidBoxComp->setJustGotRayed(false);
			}
		}*/
		mMovementSystem->drawFacingDir(mWindow, entity);
	}
	mMeleeRectSystem->drawCollideableMeleeRect(mWindow);
		

	mLightSystem.SetView(mWindow.getView());
	mLightSystem.RenderLights();

	// Draw the lights
	mLightSystem.RenderLightTexture();
	
	for (auto& entity : frontLightEntities)
		mRenderingSystems.update(entity, sf::Time());
	
	
	//mFogOfWar.drawFog(mWindow, sf::Vector2f(), 0.f);
	//mFourTree.draw(mWindow);
	
}


void Game::handleEvent(const sf::Event& event)
{
	if (mPlayerLogicSystem)
		mPlayerLogicSystem->handleEvent(event, mWindow);
}


void Game::update(sf::Time dt)
{
	
	mFourTree.clear();
	processRealTimeInput(dt);
		
	
	EventManager::getInstance()->processQueuedEvents();

	mCurEntitiesList.clear();
	
	sf::FloatRect updateBound = mCamera.getBoundingRect();

	float additionRange = 200.f;
	updateBound.left -= additionRange;
	updateBound.top -= additionRange;
	updateBound.width += additionRange * 2.f;
	updateBound.height += additionRange * 2.f;

	mCurEntitiesList = mEntityManager.getEntsByBound(updateBound);

	mFourTree.fillInFourTree(mCurEntitiesList);

	updateCommonSystem(dt);
	
	
	
	mFourTree.updateBounds(sf::Vector2f(0.f, 0.f));
	std::vector<Entity*> individualCheckingEntities;
	for (auto& entity : mCurEntitiesList){
		if (!entity->hasComp<VelocityComponent>() 
			|| (!entity->hasComp<BoxCollisionComponent>() &&
			!entity->hasComp<RotatedBoxCollisionComponent>()))
			continue;

		individualCheckingEntities.clear();

		if (entity->hasComp<BoxCollisionComponent>())
			mFourTree.getObjects(individualCheckingEntities, entity);
		else if (entity->hasComp<RotatedBoxCollisionComponent>())
			mFourTree.getObjects(individualCheckingEntities, 
				entity->comp<RotatedBoxCollisionComponent>()->getTransformedRotatedRect());

		mCollisionHandlerSystems.handleEntityWithItsGroup(entity, dt, individualCheckingEntities);
		if (mAvoidBoxSystem)
			mAvoidBoxSystem->performUnalignedAvoidance(entity, individualCheckingEntities, dt);

		MeleeRectComponent* meleeRectComp = entity->nonCreateComp<MeleeRectComponent>();

		if (meleeRectComp && meleeRectComp->isNowCollidedable()){
			std::vector<Entity*> entitiesForRect;
			mFourTree.getObjects(entitiesForRect, meleeRectComp->getRotatedRect());

			for (auto& entityRect : entitiesForRect){
				if (entityRect == entity)
					continue;

				mMeleeRectToBoxHandlerSystem->checkingTwoEntities(entity, entityRect, dt);
			}
		}
	}

	mCommonBattleLogic.runThroughCheckedList(dt);

	updateFogSystem(dt);

	sf::View view = mCamera.getView();
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, 0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		mPlayer->comp<TransformableComponent>()->setPosition(sf::Vector2f(viewPort.left, viewPort.top) +
		sf::Vector2f(sf::Mouse::getPosition(mWindow)));

	
}

void Game::updateCommonSystem(sf::Time dt)
{
	for (auto& entity : mCurEntitiesList){
		//mMeleeRectSystem->update(dt, entity);
		

		if (entity->hasComp<SpriteComponent>())
			entity->comp<SpriteComponent>()->updateBlinkStatus(dt);

		if (entity->hasComp<BuffableComponent>())
			entity->comp<BuffableComponent>()->update(dt);

		if (entity->hasComp<ScriptUpdateComponent>())
			entity->comp<ScriptUpdateComponent>()->runScriptUpdateFunc(dt);

		if (entity->hasComp<ScriptAIComponent>())
			entity->comp<ScriptAIComponent>()->update(dt, mPlayer);

		if (entity->hasComp<LightPointComponent>())
			entity->comp<LightPointComponent>()->updateLightCenter(dt, mWindow.getSize());

		if (entity->hasComp<SpiritCoreComponent>())
			entity->comp<SpiritCoreComponent>()->updateRestoreStatus(dt);

		if (entity->hasComp<HarmfulBoxesComponent>())
			entity->comp<HarmfulBoxesComponent>()->updateHarmedEntityData(dt);

		if (entity->hasComp<HealthComponent>())
			entity->comp<HealthComponent>()->update(dt);

		mGameLogicSystems.update(entity, dt);
		mMovementSystems.update(entity, dt);

		if (mFallingSystem)
			mFallingSystem->update(dt, entity);

		if (mMeleeRectSystem)
			mMeleeRectSystem->update(dt, entity);

		mAnimationSystems.update(entity, dt);
	}
}


void Game::updateFogSystem(sf::Time dt)
{
	sf::Vector2f playerPos = mPlayer->comp<TransformableComponent>()->getWorldPosition(true);
	mCamera.goTo(playerPos.x, playerPos.y);

	playerPos -= mPlayer->comp<TransformableComponent>()->getOrigin();

	sf::FloatRect rectForFog(playerPos.x - mWindow.getSize().x, playerPos.y - mWindow.getSize().y,
		mWindow.getSize().x, mWindow.getSize().y);
	std::vector<Entity*> fogExclusive;

	mFourTree.getObjects(fogExclusive, Utility::getMidOfRect(rectForFog), rectForFog);
	//mFogOfWar.updateFog(fogExclusive, playerPos, 100.f);
	mCamera.update(dt);
}

void Game::processRealTimeInput(sf::Time dt)
{
	if (!mPlayer)
		return;
	if (mPlayerLogicSystem)
		mPlayerLogicSystem->processRealTimeInput(dt, mWindow);
}