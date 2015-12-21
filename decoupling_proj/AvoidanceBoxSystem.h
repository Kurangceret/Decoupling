#pragma once
#include "System.h"
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "RotatedRect.h"

class Entity;
class AutomaticPathComponent;
class BoxCollisionComponent;
struct AStarNode;

struct CollidedAvoidBoxEntityData{
	Entity* collidedEntity;
	float dotProd;
	sf::Vector2f diff;
};
struct CollidedBoxAStarNodeData{
	CollidedBoxAStarNodeData();
	AStarNode* node;
	float dotProd;
	sf::Vector2f diff;
};

class AvoidanceBoxSystem :	public System{
public:
	typedef std::unique_ptr<AvoidanceBoxSystem> Ptr;
public:
	AvoidanceBoxSystem();
	virtual ~AvoidanceBoxSystem();
		
	void performUnalignedAvoidance(Entity* entity, 
		std::vector<Entity*>& toBeCheckedList, const std::vector<AStarNode*>& nodesAroundEntity, 
		sf::Time dt);
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
private:
	CollidedAvoidBoxEntityData* calculateClosestEntity(const sf::Vector2f& centerPos, 
		std::vector<CollidedAvoidBoxEntityData>& listEntities) const;

	CollidedBoxAStarNodeData getClosestPossibleFallableNode(Entity* mainEntity,
		RotatedRect& mainEntityRayRect,
		const std::vector<AStarNode*>& fallableNodeList) const;
	//void checkForStaticTile(Entity* entity, std::vector<Entity*>& staticTile);

	bool checkForPathWill(const sf::Vector2f& entityWorldPos,
		AutomaticPathComponent* autoPathComp, 
		BoxCollisionComponent* boxColComp,
		BoxCollisionComponent* boxColCompToBeAvoided);
};

