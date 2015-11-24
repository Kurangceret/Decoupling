#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class Entity;
class PathFinder;
struct AStarNode;

class RayCast{
public:
	RayCast();
	~RayCast();

	static bool castRayLinesFromRect(const sf::Vector2f& initialMidRectPos,
		const sf::FloatRect& rect, const sf::Vector2f& targetPos,
		PathFinder* pathFinder,
		const std::vector<AStarNode*>& toBeAvoidedNodes = std::vector<AStarNode*>(),
		bool* contactedAvoidedNodes = nullptr,
		std::vector<Entity*>& collidedTileList = std::vector<Entity*>());

	static bool castMultipleRayLines(const sf::Vector2f& initialPos,
		std::vector<sf::Vector2f>& otherPos,
		const sf::Vector2f& targetPos, PathFinder* pathFinder,
		const std::vector<AStarNode*>& toBeAvoidedNodes = std::vector<AStarNode*>(),
		bool* contactedAvoidedNodes = nullptr,
		std::vector<Entity*>& collidedTileList = std::vector<Entity*>());

	static bool castRayLineScript(float initialX, float initialY, 
		float targetX, float targetY, PathFinder* pathFinder);

	static bool castRayLine(const sf::Vector2f& initialPos,
		const sf::Vector2f& targetPos, PathFinder* pathFinder,

		const std::vector<AStarNode*>& toBeAvoidedNodes = std::vector<AStarNode*>(),
		bool* contactedAvoidedNodes = nullptr,
		std::vector<Entity*>& collidedTileList = std::vector<Entity*>());
private:
	static bool checkHorizontalTile(const sf::Vector2f& initialPos,
		const sf::Vector2f& targetPos, const sf::Vector2f& direction,
		std::vector<sf::Vector2f>& otherPos,
		PathFinder* pathFinder,

		const std::vector<AStarNode*>& toBeAvoidedNodes = std::vector<AStarNode*>(),
		bool* contactedAvoidedNodes = nullptr,
		std::vector<Entity*>& collidedTileList = std::vector<Entity*>());

	static bool checkVerticalTile(const sf::Vector2f& initialPos,
		const sf::Vector2f& targetPos, const sf::Vector2f& direction,
		std::vector<sf::Vector2f>& otherPos,
		PathFinder* pathFinder,

		const std::vector<AStarNode*>& toBeAvoidedNodes = std::vector<AStarNode*>(),
		bool* contactedAvoidedNodes = nullptr,
		std::vector<Entity*>& collidedTileList = std::vector<Entity*>());
};

