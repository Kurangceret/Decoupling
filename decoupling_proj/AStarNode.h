#ifndef ASTARNODE_H
#define ASTARNODE_H
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
class Entity;

struct AStarNode{
	typedef std::unique_ptr<AStarNode> Ptr;

	AStarNode *parent;
	float f; // g + h result
	float g; // actual path cosst from the start node to the current node/adjactent node related
	float h; // distance between currentNode and end Node

	Entity *tile;  //could be null ptr, if null means the node is empty

	sf::Vector2f pos;

	std::vector<AStarNode *> adjacent;

	bool isInsideQueue;

	virtual bool operator > (const AStarNode& right)const;

	virtual bool operator >= (const AStarNode& right)const;

	virtual bool operator < (const AStarNode& right)const;

	virtual bool operator <= (const AStarNode& right)const;

	
};


#endif