#include "GeneralData.h"
#include "EventManager.h"
#include "PathFinder.h"

GeneralData::GeneralData()
:mCalledLuaStateBinder(false)
{
}


GeneralData::~GeneralData()
{
}


void GeneralData::clearAllData()
{
	mTextures.reset(nullptr);
	mTexturesInt->destroyAllResources();
	mTexturesInt.reset(nullptr);
	mTexturesStr.reset(nullptr);
	mTiledMap.reset(nullptr);
	mEntityManager.reset(nullptr);
	mGameObjectAdder.reset(nullptr);
	//mPathFinder.reset(nullptr);
	mTexturesStringmanager.reset(nullptr);
	mComponentArranger.reset(nullptr);
	mLightSystem.reset(nullptr);
	mPlayer = nullptr;
	if (mLightSystem)
		mLightSystem->ClearLights();

	PathFinder::getInstance()->resetAllMapData();
	EventManager::getInstance()->cleanEverything();
}