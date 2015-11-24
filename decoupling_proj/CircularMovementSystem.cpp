#include "CircularMovementSystem.h"
#include <cmath>
#include "ComponentIdentifier.h"
#include "CircularPathComponent.h"
#include "VelocityComponent.h"
#include "TransformableComponent.h"
#include "Entity.h"
#include "Utility.h"
#include "EntityParentComponent.h"
#include "BoxCollisionComponent.h"

CircularMovementSystem::CircularMovementSystem()
{
	pushRequiredComponent(ComponentIdentifier::VelocityComponent);
	pushRequiredComponent(ComponentIdentifier::CircularPathComponent);
	pushRequiredComponent(ComponentIdentifier::TransformableComponent);
}


CircularMovementSystem::~CircularMovementSystem()
{
}


void CircularMovementSystem::processEntity(sf::Time dt, Entity* entity)
{
	VelocityComponent* veloComp = entity->comp<VelocityComponent>();
	CircularPathComponent* circularPathComp = entity->comp<CircularPathComponent>();
	TransformableComponent* transformComp = entity->comp<TransformableComponent>();

	sf::Vector2f origin = circularPathComp->getCenter();
	float radius = circularPathComp->getCurRadius();
	bool isClockwise = circularPathComp->isClockwise();

	sf::Vector2f entityWorldPos = transformComp->getWorldPosition(true);
	sf::Vector2f dir = Utility::unitVector(entityWorldPos - origin);
	
	
	float angle = std::abs(Utility::vectorToDegree(dir, false));
	
	float realAngle = angle;
	
	if (entityWorldPos.y > origin.y)
		realAngle = 360 - realAngle;
		
	//worked version
	float sinX = std::sin(Utility::toRadian(realAngle));
	float cosY = std::cos(Utility::toRadian(realAngle));

	sf::Vector2f velocity = sf::Vector2f(sinX, cosY);
	if (!isClockwise)
		velocity *= -1.f;

	if (std::abs(realAngle) == 270.f || std::abs(realAngle) == 90.f)
		velocity.y = 0.f;
	if (std::abs(realAngle) == 180.f || std::abs(realAngle) == 360.f)
		velocity.x = 0.f;
	
	float combinationVelo = std::abs(velocity.x) + std::abs(velocity.y);
	if (combinationVelo > 1.f)
		velocity = velocity / std::sqrt(combinationVelo);

	float originalSpeed = circularPathComp->getSpeed();
	
	
	sf::Vector2f finMov = velocity * originalSpeed;

	transformComp->move(finMov * dt.asSeconds());

	entityWorldPos = transformComp->getWorldPosition(true);
	float lengthFromOrigin = Utility::vectorLength(entityWorldPos - origin);

	float diff = lengthFromOrigin - radius;
	if (std::abs(diff) > 1.f){
		
		sf::Vector2f dirToGo = Utility::unitVector(origin - entityWorldPos);
		if (dirToGo.x + dirToGo.y > 1.f)
			dirToGo /= std::sqrt(dirToGo.x + dirToGo.y);

		if (diff < 0.f)
			dirToGo *= -1.f;
		transformComp->move(dirToGo * 50.f * dt.asSeconds());
	}
	
}