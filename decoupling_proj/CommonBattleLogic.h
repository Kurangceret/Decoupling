#pragma once


#include <SFML/System/Time.hpp>
#include <unordered_map>
#include <vector>

class Entity;
class MeleeRectComponent;

class CommonBattleLogic{
public:
	CommonBattleLogic();
	~CommonBattleLogic();

	void insertNewPair(MeleeRectComponent* meleeRectComp, Entity* collidedEntity);

	void runThroughCheckedList(sf::Time dt);
private:
	std::vector<std::pair<MeleeRectComponent*, Entity*>> mToBeCheckedList;
};

