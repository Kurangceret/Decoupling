#include "SceneGraph.h"


SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
}


void SceneGraph::draw(sf::RenderTarget& target)
{

}

void SceneGraph::updateGameObjects(sf::Time dt, CommandQueue& commandQueue)
{
	for (auto& iter : mGameObjects){

		for (auto& obj : iter.second){

		}

	}
}


void SceneGraph::addNewLayer(const std::string& layer)
{
	mGameObjects.insert(std::make_pair(layer, std::vector<GameObject::Ptr>()));
}

void SceneGraph::addGameObject(const std::string& layerName, GameObject::Ptr gameObject)
{
	auto layerIter = mGameObjects.find(layerName);
	if (layerIter == mGameObjects.end())
		return;

	layerIter->second.push_back(std::move(gameObject));
}

void SceneGraph::traverseGameObjectCommand(const Command& command, sf::Time dt)
{
	for (auto& iter : mGameObjects){

		for (auto& obj : iter.second){
			if (obj->getCategory() & command.category)
				command.action(*obj.get(), dt);

			for (auto& child : obj->mChildren){
				if (child->getCategory() & command.category)
					command.action(*child.get(), dt);
			}
				
		}
	}
}

void SceneGraph::reactToSceneGraphCommand(const CommandQueue::SceneGraphCommand& command, sf::Time dt)
{
	command(*this, dt);
}