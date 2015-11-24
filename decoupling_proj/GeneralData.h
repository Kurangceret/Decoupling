#pragma once
#include "EntityManager.h"
#include "ResourceHolder.h"
#include "TiledMapSystem.h"
#include "GameObjectAdder.h"
#include "TextureStringManager.h"
#include "PathFinder.h"
#include "ComponentArranger.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include <LTBL/Light/LightSystem.h>
class GeneralData{
public:
	GeneralData();
	~GeneralData();

	bool mCalledLuaStateBinder;

	void setLuaState(lua_State* luaState)
	{
		mLuaState = luaState;
	}

	void setComponentArranger(ComponentArranger* componentArranger)
	{
		mComponentArranger.reset(componentArranger);
	}

	void setTexturesStringManager(TextureStringManager* manager)
	{
		mTexturesStringmanager.reset(manager);
	}

	void setEntityManager(EntityManager* entityManager)
	{
		mEntityManager.reset(entityManager);
	}

	void setTiledMap(TiledMapSystem *tiledMap)
	{
		mTiledMap.reset(tiledMap);
	}

	void setTextureHolderInt(TextureHolderInt *texturesInt)
	{
		mTexturesInt.reset(texturesInt);
	}

	void setTextureHolderStr(TextureHolderStr *texturesStr)
	{
		mTexturesStr.reset(texturesStr);
	}

	void setTextureHolder(TextureHolder *textures)
	{
		mTextures.reset(textures);
	}

	void setGameObjectAdder(GameObjectAdder *gameObjectAdder)
	{
		mGameObjectAdder.reset(gameObjectAdder);
	}

	/*void setPathFinder(PathFinder* pathFinder)
	{
		mPathFinder.reset(pathFinder);
	}*/

	void setLightSystem(ltbl::LightSystem* lightSystem)
	{
		mLightSystem.reset(lightSystem);
	}

	void setPlayer(Entity* player)
	{
		mPlayer = player;
	}

	TiledMapSystem* getTiledMap() { return mTiledMap.get(); }
	TextureHolderInt* getTextureHolderInt(){ return mTexturesInt.get(); }
	TextureHolderStr* getTextureHolderStr() { return mTexturesStr.get(); }
	TextureHolder* getTextureHolder() { return mTextures.get(); }
	GameObjectAdder* getGameObjectAdder() { return mGameObjectAdder.get(); }
	EntityManager* getEntityManager() { return mEntityManager.get(); }
	//PathFinder* getPathFinder() { return mPathFinder.get(); }
	TextureStringManager* getTexturesStringManager(){ return mTexturesStringmanager.get(); }
	ComponentArranger* getComponentArranger(){ return mComponentArranger.get(); }
	lua_State* getLuaState(){ return mLuaState; }
	ltbl::LightSystem* getLightSystem() { return mLightSystem.get(); }

	Entity* getPlayer(){ return mPlayer; }

	void clearAllData();
private:
	Entity* mPlayer;
	std::unique_ptr<EntityManager> mEntityManager;
	//std::unique_ptr<TiledMapSystem> mTiledMapSystem;
	std::unique_ptr<TiledMapSystem> mTiledMap;
	std::unique_ptr<TextureHolderInt> mTexturesInt;
	std::unique_ptr<TextureHolderStr> mTexturesStr;
	std::unique_ptr<TextureHolder> mTextures;
	std::unique_ptr<GameObjectAdder> mGameObjectAdder;
	//std::unique_ptr<PathFinder> mPathFinder;
	std::unique_ptr<TextureStringManager> mTexturesStringmanager;
	std::unique_ptr<ComponentArranger> mComponentArranger;
	lua_State* mLuaState;
	std::unique_ptr<ltbl::LightSystem> mLightSystem;
	
};

