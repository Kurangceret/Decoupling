#pragma once

#include "GroupSystem.h"
#include "GroupCollisionSystem.h"
#include "GeneralData.h"
#include "ResourceHolder.h"
#include "MeleeRectSystem.h"
#include "CommonBattleLogic.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Font.hpp>
#include <LTBL\Light\LightSystem.h>
#include <LTBL\Light\Light_Point.h>
#include "FogOfWar.h"
#include "FourTree.h"
#include "Camera.h"
#include "AvoidanceBoxSystem.h"

class PlayerLogicSystem;
class MovementSystem;
class MeleeRectToBoxHandlerSystem;

class Game{
public:
	Game(GeneralData* generalData, sf::RenderWindow& renderWindow);
	~Game();

	void handleEvent(const sf::Event& event);
	void update(sf::Time dt);

	void draw();

private:
	//void loadManualTexture();
	void processRealTimeInput(sf::Time dt);

	void initializeSystem();
	void updateFogSystem(sf::Time dt);
	void updateCommonSystem(sf::Time dt);
private:
	EntityManager& mEntityManager;
	sf::Font mFont;


	MovementSystem* mMovementSystem;
	std::vector<System::Ptr> mSystems;

	AvoidanceBoxSystem* mAvoidBoxSystem;
	MeleeRectToBoxHandlerSystem* mMeleeRectToBoxHandlerSystem;

	MeleeRectSystem::Ptr mMeleeRectSystem;
	GroupSystem mAnimationSystems;
	GroupSystem mRenderingSystems;
	GroupSystem mMovementSystems;
	GroupSystem mGameLogicSystems;
	GroupCollisionSystem mCollisionHandlerSystems;

	CommonBattleLogic mCommonBattleLogic;

	Entity* mPlayer;
	PlayerLogicSystem* mPlayerLogicSystem;

	TextureHolder& mTextures;
	TextureHolderInt& mTexturesInt;
	TextureStringManager& mTexturesStringManager;
	TiledMapSystem& mTiledMap;

	ComponentArranger& mComponentArranger;

	sf::RenderWindow& mWindow;
	//PathFinder& mPathFinder;

	std::vector<Entity*> mCurEntitiesList;

	//ltbl::LightSystem mLightSystem;
	ltbl::LightSystem& mLightSystem;

	FogOfWar mFogOfWar;
	FourTree mFourTree;
	Camera mCamera;
};

