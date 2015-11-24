#include "AnimationSystem.h"
#include "Entity.h"
#include "AnimationComponent.h"

AnimationSystem::AnimationSystem()
{
	pushRequiredComponent(ComponentIdentifier::AnimationComponent);
}


AnimationSystem::~AnimationSystem()
{
}


void AnimationSystem::processEntity(sf::Time dt, Entity* entity)
{
	entity->comp<AnimationComponent>()->updateAnimation(dt);
}