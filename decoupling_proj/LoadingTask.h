#pragma once
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Category.h"

class GeneralData;

class LoadingTask{
public:
	LoadingTask(GeneralData* general);
	~LoadingTask();

	void execute();
	bool isFinished();
	float getCompletion();
	std::string getCurrentText();
	bool isThreadTerminated();
private:
	void runTask();
	//will setup the map and add entities and vertex to the mSceneGraph
	//also will add any rectangular object if there is any and will cam setupTmxObject
	//for every object in the layer
	void initializeLayerMap(const std::string& layerName);

	void setupTextureObject(const std::string& layer, const sf::IntRect& objectRect,
		const std::string& objectName, sf::Vector2f *objPos,
		sf::Vector2f *objSize);


	void loadManualTexture();

	void bindLogicsToLuaScript();
private:
	sf::Thread mThread;
	bool mFinished;
	float mCurrentPercent;
	std::string mCurrentProcess;
	sf::Mutex mMutex;

	
	GeneralData* mGeneralData;
	bool mIsThreadTerminated;
};

