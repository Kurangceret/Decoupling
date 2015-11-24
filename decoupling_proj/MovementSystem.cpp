#include "MovementSystem.h"
#include "Entity.h"
#include "ComponentIdentifier.h"
#include "SpriteComponent.h"
#include "VelocityComponent.h"
#include "TransformableComponent.h"
#include "StaminaComponent.h"
#include "Utility.h"
#include "SpiritFormComponent.h"
#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/RenderTarget.hpp>
#include "Constant.h"

MovementSystem::MovementSystem()
{
	pushRequiredComponent(ComponentIdentifier::TransformableComponent);
	pushRequiredComponent(ComponentIdentifier::VelocityComponent);
}


MovementSystem::~MovementSystem()
{
}


void MovementSystem::drawFacingDir(sf::RenderTarget& target, Entity* entity)
{
	if (!System::checkForEntity(entity))
		return;

	VelocityComponent* movingComponent = entity->comp<VelocityComponent>();
	if (!movingComponent->isDrawingFacingDirection())
		return;

	TransformableComponent* transformableComponent = entity->comp<TransformableComponent>();

	sf::RenderStates states;
	states.transform.translate(transformableComponent->getWorldPosition(true));

	sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile(textureDir + "facing_arrow.png");
	sprite.setTexture(texture);

	sf::Vector2f facingDir = movingComponent->getFacingDirection();

	float facingDegree = Utility::vectorToDegree(facingDir, false);
	states.transform.rotate(facingDegree);

	target.draw(sprite, states);
}

void MovementSystem::processEntity(sf::Time dt, Entity* entity)
{
		
	VelocityComponent* movingComponent = entity->comp<VelocityComponent>();
	TransformableComponent* transformableComponent = entity->comp<TransformableComponent>();
	
	sf::Vector2f velocity = movingComponent->getVelocity();
	
	float combinationVelo = std::abs(velocity.x) + std::abs(velocity.y);
	if (combinationVelo > 1.f)
		velocity = velocity / std::sqrt(combinationVelo);

	/*bool isSprintOneFrame = movingComponent->isSprintingOneFrame();

	float finalSpeed = isSprintOneFrame ? 
		movingComponent->getSprintSpeed() : movingComponent->getSpeed();
	

	if (isSprintOneFrame && entity->hasComp<StaminaComponent>()){
		StaminaComponent* staminaComp =  entity->comp<StaminaComponent>();
		staminaComp->setStaminaRecover(false);
		staminaComp->decreaseCurStamina(15.f * dt.asSeconds());
	}*/
	float finalSpeed = movingComponent->getSpeed();

	if (entity->hasComp<SpiritFormComponent>() && entity->comp<SpiritFormComponent>()->isOnSpiritForm())
		finalSpeed = entity->comp<SpiritFormComponent>()->getSpiritSpeed();

	transformableComponent->
		move(velocity * finalSpeed * dt.asSeconds());
	//movingComponent->setVelocity(0.f, 0.f, false);
}
