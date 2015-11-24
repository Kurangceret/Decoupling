#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class VelocityComponent;

class SteeringBehavior{
public:
	SteeringBehavior();
	~SteeringBehavior();

	//static SteeringBehavior* getInstance();

	/*static sf::Vector2f seekTarget(VelocityComponent* entity,
		const sf::Vector2f& targetPos, sf::Time dt);*/

	static sf::Vector2f seekTarget(VelocityComponent* veloComp,
		const sf::Vector2f& initialPos, const sf::Vector2f& targetPos, sf::Time dt);

	static sf::Vector2f seek(VelocityComponent* entity,
		const sf::Vector2f& direction, sf::Time dt);

private:
	//static std::unique_ptr<SteeringBehavior> mInstance;
};

