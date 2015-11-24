#include "SteeringBehavior.h"
#include "Utility.h"
#include "VelocityComponent.h"

SteeringBehavior::SteeringBehavior()
{
}


SteeringBehavior::~SteeringBehavior()
{
}



/*sf::Vector2f SteeringBehavior::seekTarget(MoveableEntity* entity, const sf::Vector2f& targetPos, sf::Time dt)
{
	return seek(entity, unitVector(targetPos - entity->getPosition()), dt);
}*/

sf::Vector2f SteeringBehavior::seekTarget(VelocityComponent* veloComp,
	const sf::Vector2f& initialPos, const sf::Vector2f& targetPos, sf::Time dt)
{
	return seek(veloComp, Utility::unitVector(targetPos - initialPos), dt);
}

sf::Vector2f SteeringBehavior::seek(VelocityComponent* veloComp, const sf::Vector2f& targetDir, sf::Time dt)
{
	sf::Vector2f curVelocity = veloComp->getVelocity();
	if (curVelocity == sf::Vector2f(0.f, 0.f)) 
		curVelocity = veloComp->getFacingDirection();
	sf::Vector2f targetVelo = targetDir;

	sf::Vector2f steering = targetVelo - curVelocity;
	steering /= (900.f * dt.asSeconds());

	steering += curVelocity;
	
	return steering;
}