#pragma once
#include "Component.h"
#include <SFML/Graphics/Transformable.hpp>
class TransformableComponent :	public Component{
public:
	TransformableComponent(Entity* entity);
	virtual ~TransformableComponent();
	//will only affect if the owner entity has parent component
	sf::Transform getWorldTransform(bool withOrigin = false) const;
	sf::Transform getTransformWithOrigin() const;
	sf::Vector2f getWorldPosition(bool withOrigin = false) const;

	void setPositionX(float x);
	void setPositionY(float y);

	//exclusive for script usage
	void setScriptPosition(const float x, const float y);
	void moveScript(const float x, const float y);
	//

	void setPosition(const sf::Vector2f& vector);
	void setPosition(const float x, const float y);

	sf::Vector2f getLocalPosition(bool withOrigin = false) const;

	void setOriginX(float x);
	void setOriginY(float y);

	void setOrigin(const sf::Vector2f& origin);
	void setOrigin(float x, float y);

	sf::Vector2f getOrigin() const;

	void move(float x, float y);
	void move(const sf::Vector2f& v);

	void rotate(float angle);
	void setRotation(float newAngle);
private:
	sf::Transformable mTransformable;
};

