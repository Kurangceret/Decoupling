#ifndef MOVINGNODE_H
#define MOVINGNODE_H
#include "AStarNode.h"

//this moving node will be exclusively used for entity

struct MovingNode{
	MovingNode() :
	starNode(nullptr), direction()/*isRayedNode(false),
	//isExactPos(false), direction(), additionalPos()*/{}

	AStarNode *starNode;
	//bool isExactPos;
	sf::Vector2f direction;
	/*sf::Vector2f additionalPos;
	bool isRayedNode;*/
};


#endif