#pragma once
#include <vector>
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include "RotatedRect.h"


class Entity;
struct AStarNode;

class FourTree{
public:
	typedef std::unique_ptr<FourTree> Ptr;
	typedef std::vector<int> Indexes;
	enum Location{
		Top_Right,
		Top_Left,
		Bottom_Left,
		Bottom_Right,
		LocationCount
	};
public:
	FourTree(int maxObjects, int maxLevels, sf::FloatRect bounds, int currentLevel = 0);
	~FourTree();

	void draw(sf::RenderWindow& window);
	void clear();
	//

	void fillInFourTree(const std::vector<Entity*> entities);
	void setBounds(sf::FloatRect bound){ mBounds = bound; }
	void updateBounds(const sf::Vector2f& locationAdd);

	void getObjects(std::vector<Entity *>& entities, Entity *identifier, 
		std::vector<sf::FloatRect>& finalIndexesWorldBound = std::vector<sf::FloatRect>());
	void getObjects(std::vector<Entity*>& entities, const sf::Vector2f& centerRectPos,
		const sf::FloatRect& rect, std::vector<sf::FloatRect>& finalIndexesWorldBound 
		= std::vector<sf::FloatRect>());
	void getObjects(std::vector<Entity*>& entities, const RotatedRect& rotatedRect, 
		std::vector<sf::FloatRect>& finalIndexesWorldBound = std::vector<sf::FloatRect>());

	void draw(sf::RenderTarget& window);

	FourTree* locationToPointer(FourTree::Location id) const;

	sf::FloatRect getBounds() { return mBounds; }
	sf::FloatRect getWorldBounds();
	Indexes getIndexes(const sf::Vector2f& centerRectPos,
		const sf::FloatRect& rect);
	Indexes getIndexes(const RotatedRect& rotatedRect);
	Indexes getIndexes(Entity* entityPtr);
private:
	
	void insert(Entity *ptr);
	Entity* deleteFromContainer(Entity *entit);
private:
	int mMaxObjects;
	int mMaxLevels;

	int mCurrentLevel;
	std::vector<Entity *> mEntities;
	sf::FloatRect mBounds;
	std::array<FourTree::Ptr, FourTree::Location::LocationCount> mNodes;
	bool isSpilt;
	sf::RectangleShape mShape;
	//this location will be used to adjust the rectangle in all nodes and parent
	sf::Vector2f mWorldLocation;
};

