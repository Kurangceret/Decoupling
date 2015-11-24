#pragma once
#include <string>
#include <unordered_map>
//#include "GameObject.h"
#include <vector>
#include "CommandQueue.h"
#include <SFML/Graphics.hpp>

class SceneGraph{
public:
	SceneGraph();
	~SceneGraph();

	void draw(sf::RenderTarget& target);
	void updateGameObjects(sf::Time dt, CommandQueue& commandQueue);


	void addNewLayer(const std::string& layer);
	void addGameObject(const std::string& layerName, GameObject::Ptr gameObject);

	void traverseGameObjectCommand(const Command& command, sf::Time dt);
	void reactToSceneGraphCommand(const CommandQueue::SceneGraphCommand& command, sf::Time dt);
private:
	std::unordered_map<std::string, std::vector<GameObject::Ptr> > mGameObjects;
};

