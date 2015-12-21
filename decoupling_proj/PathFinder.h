#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <stack>
#include <map>
#include <queue>
#include <functional>
#include <set>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>


#include "MovingNode.h"

class Entity;
class EntityExpertiseComponent;
struct lua_State;

struct ComparePair{
	bool operator()(const std::pair<AStarNode*, float>& left, 
	const std::pair<AStarNode*, float>& right) const {
		return left.second > right.second;
	}
};

class PathFinder{
	
public:
	PathFinder();
	~PathFinder();

	void resetAllMapData();

	void initializeAStarNodes(const sf::Vector2f& tileSize, const sf::Vector2f& mapSize);

	//returns the achievable target node
	//in the case the original target was not reacheable
	AStarNode* moveSceneTo(Entity *entity, const sf::Vector2f& targetPos);
	
	void setAdjacentNode(const sf::Vector2f& tileSize);
	
	AStarNode* sceneToGraph(Entity *);
	AStarNode* sceneToGraph(const sf::Vector2f& pos);
	AStarNode* sceneToGraph(const float x, const float y);
	
	/*exclusive for Lua script usage*/
	AStarNode* sceneToGraphScript(const float x, const float y);
	AStarNode* AtScript(int x, int y) const;
	sf::Vector2i sceneToCoordinateScript(const float x, const float y);
	/**/

	sf::Vector2i sceneToCoordinate(Entity *entity);
	sf::Vector2i sceneToCoordinate(const sf::Vector2f& pos);
	sf::Vector2i sceneToCoordinate(const float x, const float y);

	

	sf::Vector2f coordToPosition(int x, int y) const;
	AStarNode* At(int x, int y) const;
	AStarNode* At(const sf::Vector2i& coordinate) const;
	
	sf::Vector2f getTileSize() const { return mTileSize; }
	sf::Vector2f getMapSize() const { return mMapSize; }

	void drawDebugGraph(sf::RenderWindow& window);

	static PathFinder* getInstance();

	void PathFinder::getListOfNodesBasedOnBoundingRect(const sf::FloatRect& rect,
		std::vector<AStarNode*>& markedNodes);

	bool isRectOverallNodeSafe(const sf::Vector2f& entityPos,
		const sf::FloatRect& entityRect, EntityExpertiseComponent* entExpertiseComp = nullptr);

	/*bool isLuaRectOverallNodeSafe(const sf::Vector2f& entityPos,
		const sf::FloatRect& entityRect, lua_State* luaState);*/
private:
	
	

	void addNode(const sf::Vector2f& pos);
	void clearParent();
	std::vector<MovingNode> constructPath(Entity *entity, const sf::Vector2f& pos, 
		AStarNode*& reacheableNode);
		
	void deleteSpecificNode(std::vector<AStarNode*>& set, AStarNode* specificNode);
	void deleteSpecificNode(std::map<AStarNode*, int>& map, AStarNode* specificNode);
	AStarNode* nodeWithLowestF(std::vector<AStarNode*>& set);
	AStarNode* nodeWithLowestF(std::set<AStarNode*>& set);
	bool hasNode(const std::vector<AStarNode*>& set, const AStarNode *val);
	std::vector<AStarNode*>::const_iterator getIter(const std::vector<AStarNode*>& set, const AStarNode *val);
		
private:
	static PathFinder* mPathFinder;
	std::vector<AStarNode::Ptr> mNodes;
	//this will determine the size of the node
	sf::Vector2f mTileSize;
	//size of the map, not in the precised size, instead on number of tiles
	sf::Vector2f mMapSize;

	
};

