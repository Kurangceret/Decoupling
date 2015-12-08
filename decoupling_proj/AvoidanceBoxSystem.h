#pragma once
#include "System.h"
#include <vector>
#include <SFML/System/Vector2.hpp>
class Entity;
class AutomaticPathComponent;
class BoxCollisionComponent;

struct CollidedAvoidBoxEntityData{
	Entity* collidedEntity;
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
		std::vector<Entity*>& toBeCheckedList, sf::Time dt);
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
private:
	CollidedAvoidBoxEntityData* calculateClosestEntity(const sf::Vector2f& centerPos, 
		std::vector<CollidedAvoidBoxEntityData>& listEntities) const;

	//void checkForStaticTile(Entity* entity, std::vector<Entity*>& staticTile);

	bool checkForPathWill(const sf::Vector2f& entityWorldPos,
		AutomaticPathComponent* autoPathComp, 
		BoxCollisionComponent* boxColComp,
		BoxCollisionComponent* boxColCompToBeAvoided);
};

