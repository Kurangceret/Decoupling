#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

class ComponentArranger;
class CircularPathComponent :	public Component{
	friend ComponentArranger;
public:
	CircularPathComponent(Entity* entity);
	virtual ~CircularPathComponent();
	
	void setIsRotatingAroundParentPos(bool flag);
	bool isRotatingAroundParentPos() const;

	void setClockwise(bool flag);
	bool isClockwise() const;

	void setCenter(sf::Vector2f center);
	sf::Vector2f getCenter() const;
	void setSpeed(float speed);

	void increaseCurRadius(float val);
	void decreaseCurRadius(float val);

	void setCurRadius(float val);
	float getCurRadius() const;
	float getSpeed() const;
private:
	bool mIsRotatingAroundParentPos;
	bool mIsClockwise;
	sf::Vector2f mCenter;

	float mMaxRadius;
	float mCurRadius;
	float mMinRadius;

	float mSpeed;
};

