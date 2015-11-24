#include "SceneNode.h"

#include <iostream>
#include <string>

#include <algorithm>
//#include "VertexNode.h"

SceneNode::SceneNode(Category::ID type)
: mParent(nullptr), 
mCategory(type),
mChildren(),
mBoundaries(0.f, 0.f, 0.f, 0.f),
mCanBeUpdated(true),
mLayerName("")
{

}

SceneNode::~SceneNode()
{}



void SceneNode::setCanBeUpdated(bool flag)
{
	mCanBeUpdated = flag;
}

void SceneNode::setCategory(Category::ID category)
{

}

Category::ID SceneNode::getCategory() const
{
	return mCategory;
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

sf::FloatRect SceneNode::getUpdateBoundaries() const
{
	return mBoundaries;
}

SceneNode* SceneNode::getParent() const
{
	return mParent;
}

void SceneNode::removeWrecks()
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(),
		std::mem_fn(&SceneNode::isMarkedForRemoval));
		
	mChildren.erase(wreckfieldBegin, mChildren.end());

	for (auto& children : mChildren)
		children->removeWrecks();

	//std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

/*void SceneNode::removeWrecks(CommandQueue& commandQueue)
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(),
		std::mem_fn(&SceneNode::isMarkedForRemoval));

	
	for (auto iter = wreckfieldBegin; iter != mChildren.end(); iter++)
		commandQueue.deleteCommand(iter->get());
	

	mChildren.erase(wreckfieldBegin, mChildren.end());
	

	for (auto& children : mChildren)
		children->removeWrecks(commandQueue);

	//std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}*/

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	//transfer the ownership/ move the object to the container
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	//using lambda expression, [&] captures all variables by references
	//then we casted it by using (Ptr& p, a '&' is needed since we capture by [&]
	//then -> specifies the return type in which we want bool
	//then the statement is on the {} body
	//lambda was similar to the like of anonymous function
	auto found = std::find_if(mChildren.begin(), mChildren.end(),
		[&](Ptr& p) -> bool {return p.get() == &node; });

	assert(found != mChildren.end());
	//transfer the ownership once again
	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

/*VertexNode* SceneNode::findVertexNode(int tileSetId)
{
	VertexNode *finalPtr = nullptr;
	for (auto& child : mChildren){
		finalPtr = dynamic_cast<VertexNode *>(child.get());
		if (finalPtr){
			if (finalPtr->getTileSetId() == tileSetId)
				return finalPtr;
			else
				finalPtr = nullptr;
		}
	}
	return finalPtr;
}*/

//will draw all its childs
void SceneNode::draw(sf::RenderTarget& target
	, sf::RenderStates states) const
{
	//*= operator combines the parent's absolute transform with the current node's relative one
	//this is what makes child nodes follow its parent node
	//and also drawing sprite on the windows.
	states.transform *= getTransform();
	
	drawCurrent(target, states);
	
		
	if (getCategory() & Category::Layer)
		std::sort(mChildren.begin(), mChildren.end(), &SceneNode::sortUnit);

	//will draw its children
	for (const Ptr& child : mChildren)
		child->draw(target, states);
}

bool SceneNode::sortUnit(const std::unique_ptr<SceneNode>& lhs, const std::unique_ptr<SceneNode>& rhs)
{
	return ((lhs->getPosition().y < rhs->getPosition().y) ||
		(lhs->getPosition().y == rhs->getPosition().y &&
		lhs->getPosition().x < rhs->getPosition().x));
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);

	

	updateChildren(dt, commands);
}

void SceneNode::drawCurrent(sf::RenderTarget& target
	, sf::RenderStates states) const
{}
void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	int i = 0;
	for (const Ptr& child : mChildren){
		if (child->mCanBeUpdated)
			child->update(dt, commands);
		i++;
	}
}


void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	//& bit wise operator to see any enum was matched 
	if (command.category == Category::None ||
		command.category & getCategory() || 
		(command.targetObject && command.targetObject == this))
		command.action(*this, dt);

	for (Ptr& child : mChildren)
		child->onCommand(command, dt);

}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;//no effect... but its just for default
	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::setLayerName(const std::string& layerName)
{
	if (mCategory & Category::Layer)
		mLayerName = layerName;
}

std::string SceneNode::getLayerName() const
{
	if (mCategory & Category::Layer)
		return mLayerName;

	return "";
}

void SceneNode::fillInContainerWithChild(std::vector<SceneNode*>& sceneNodes)
{
	/*if (!(mCategory & Category::FloorTile) && !(mCategory & Category::Layer) &&
		mCategory != Category::None)
		sceneNodes.push_back(this);*/

	for (Ptr& child : mChildren)
		child->fillInContainerWithChild(sceneNodes);
}


void SceneNode::handleCollidedSceneNode(SceneNode* sceneNode, CommandQueue& commandQueue)
{

}

