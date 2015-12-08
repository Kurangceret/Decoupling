#include "TransformableComponent.h"
#include "Entity.h"
#include "EntityParentComponent.h"
#include "CategoryComponent.h"

TransformableComponent::TransformableComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::TransformableComponent;
}


TransformableComponent::~TransformableComponent()
{
}


sf::Transform TransformableComponent::getWorldTransform(bool withOrigin) const
{
	sf::Transform transform = mTransformable.getTransform();

	if (withOrigin)
		transform = getTransformWithOrigin();

	Entity* curEntity = mOwnerEntity;

	while (curEntity && curEntity->hasComp<EntityParentComponent>() &&  
		curEntity->hasComp<TransformableComponent>())
	{

		curEntity = curEntity->comp<EntityParentComponent>()->getParent();
		if (!curEntity)
			break;

		sf::Transform curTransform = curEntity->comp<TransformableComponent>()->
			mTransformable.getTransform();

		sf::Transform curTransform2 = curEntity->comp<TransformableComponent>()->
			getTransformWithOrigin();
		//if (withOrigin)
			transform *= curEntity->comp<TransformableComponent>()->
				getTransformWithOrigin();
		/*else
			transform *= curEntity->comp<TransformableComponent>()->
			mTransformable.getTransform();*/
				
	}

	return transform;
}

sf::Transform TransformableComponent::getTransformWithOrigin() const
{
	sf::Transform transform = mTransformable.getTransform();
	transform.translate(mTransformable.getOrigin());
	return transform;
}


sf::Vector2f TransformableComponent::getWorldPosition(bool withOrigin) const
{
	return getWorldTransform(withOrigin) * sf::Vector2f();
}

void TransformableComponent::setPositionX(float x)
{
	mTransformable.setPosition(x, mTransformable.getPosition().y);
}

void TransformableComponent::setPositionY(float y)
{
	mTransformable.setPosition(mTransformable.getPosition().x, y);
}

void TransformableComponent::setScriptPosition(const float x, const float y)
{
	setPosition(x, y);
}

void TransformableComponent::setPosition(const sf::Vector2f& vector)
{
	mTransformable.setPosition(vector.x, vector.y);
}

void TransformableComponent::setPosition(const float x, const float y)
{	
	setPositionX(x);
	setPositionY(y);
}

sf::Vector2f TransformableComponent::getLocalPosition(bool withOrigin) const
{
	sf::Transform transform = withOrigin ? getTransformWithOrigin() : mTransformable.getTransform();
	return transform * sf::Vector2f();
}

void TransformableComponent::setOriginX(float x)
{
	mTransformable.setOrigin(x, mTransformable.getOrigin().y);
}

void TransformableComponent::setOriginY(float y)
{
	mTransformable.setOrigin(mTransformable.getOrigin().x, y);
}

void TransformableComponent::setOrigin(const sf::Vector2f& origin)
{
	mTransformable.setOrigin(origin);
}

void TransformableComponent::setOrigin(float x, float y)
{
	setOriginX(x);
	setOriginY(y);
}

sf::Vector2f TransformableComponent::getOrigin() const
{
	return mTransformable.getOrigin();
}

void TransformableComponent::move(float x, float y)
{
	mTransformable.move(x, y);
}

void TransformableComponent::move(const sf::Vector2f& v)
{
	
	move(v.x, v.y);
}

void TransformableComponent::moveScript(const float x, const float y)
{
	move(x, y);
}

void TransformableComponent::rotate(float angle)
{
	mTransformable.rotate(angle);
}

void TransformableComponent::setRotation(float newAngle)
{
	mTransformable.setRotation(newAngle);
}