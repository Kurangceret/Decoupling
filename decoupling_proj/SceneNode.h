#pragma once

#include "Utility.h"
//#include "Quadtree.h"
#include <memory>
#include <cmath>
#include <set>
#include "Category.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "CommandQueue.h"
#include "ResourceHolder.h"

class FourTree;
class Entity;
class VertexNode;
class MoveableEntity;

class SceneNode : public sf::Transformable, public sf::Drawable
	, sf::NonCopyable{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;
public:
	SceneNode(Category::ID category = Category::None);

	SceneNode(SceneNode&& other) : mChildren(std::move(other.mChildren)),
		mCategory(other.mCategory), mParent(other.mParent), mBoundaries(other.mBoundaries){}
	
	void swap(SceneNode& lhs, SceneNode& rhs){

		using std::swap;

		swap(lhs.mChildren, rhs.mChildren);
		swap(lhs.mCategory, rhs.mCategory);
		swap(lhs.mParent, rhs.mParent);
		swap(lhs.mBoundaries, rhs.mBoundaries);
	}

	SceneNode& operator = (SceneNode other){
		swap(*this, other);
		return *this;
	}

	virtual ~SceneNode();


	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	sf::Vector2f getBoundingRectMidPos() const;

	void update(sf::Time dt, CommandQueue& commands);
	sf::Vector2f getWorldPosition() const;

	//get the category of the command
	void setCategory(Category::ID category);
	virtual Category::ID getCategory() const;

	void onCommand(const Command& command, sf::Time dt);



	void fillInContainerWithChild(std::vector<SceneNode*>& sceneNodes);

	void removeWrecks();
	void removeWrecks(CommandQueue& commandQueue);

	//VertexNode* findVertexNode(int tileSetId);

	//void fillQuadTree(Quadtree& quadtree);
	//void fillFourTree(FourTree* fourTree, std::vector<Entity*>& mList);

	virtual bool isMarkedForRemoval() const;
	virtual bool isDestroyed() const;

	SceneNode* getParent() const;

	void setLayerName(const std::string& layerName);
	std::string getLayerName() const;

	void setCanBeUpdated(bool flag);


	//make sure to call this func only when you are sure the passed sceneNode
	//actually collided with this caller
	virtual void handleCollidedSceneNode(SceneNode* sceneNode, CommandQueue& commandQueue);

protected:
	//our original virtual function
	virtual void drawCurrent(sf::RenderTarget& target
		, sf::RenderStates states) const;

	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);

	sf::Transform getWorldTransform() const;
	//get the mBoundaries
	sf::FloatRect getUpdateBoundaries() const;

	//void checkSwingDamage(const RotatedRect& swordRect);
protected:
	mutable std::vector<Ptr> mChildren;



private:
	void updateChildren(sf::Time dt, CommandQueue& commands);

	//virtual function from sf::Drawable class
	virtual void draw(sf::RenderTarget& target
		, sf::RenderStates states) const;

	static bool sortUnit(const SceneNode::Ptr& lhss, const SceneNode::Ptr& rhss);
private:
	//mutable std::vector<Ptr> mChildren;
	SceneNode* mParent;
	Category::ID mCategory;
	//boundaries of the updating list
	//means the boundaries which allow certain entity to be update
	sf::FloatRect mBoundaries;

	bool mCanBeUpdated;
	//only applies if this category is layer
	std::string mLayerName;
};




