#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

class VelocityComponent :	public Component{
public:
	VelocityComponent(Entity* entity);
	virtual ~VelocityComponent();

	void setVelocity(const sf::Vector2f& vector, bool turnFaceDir = true);
	void setVelocity(float x, float y, bool turnFaceDir = true);

	void setVelocityScript(float x, float y, bool turnFaceDir = true);

	void setVelocityY(float y, bool turnFaceDir = true);
	void setVelocityX(float x, bool turnFaceDir = true);

	sf::Vector2f getVelocity() const;
	float getVelocityX() const;
	float getVelocityY() const;

	void setFacingDirectionX(float x);
	void setFacingDirectionY(float y);

	sf::Vector2f getFacingDirection() const;
	float getFacingDirectionX() const;
	float getFacingDirectionY() const;
	
	void setSpeed(float speed);
	float getSpeed() const;

	void setSpeedIdentifier(float val);
	float getSpeedIdentifier() const;

	void setSprintSpeed(float sprintSpeed);
	float getSprintSpeed() const;

	void sprintOneFrame();
	bool isSprintingOneFrame();

	bool isDrawingFacingDirection() const;
private:
	void updateDirBasedOnVelo();
private:

	sf::Vector2f mVelocity;
	sf::Vector2f mFacingDirection;

	//this variable will be times to mSpeed in the getSpeed() function
	float mSpeedIdentifier;

	float mSpeed;
	float mSprintSpeed;

	bool mIsSprintingOneFrame;
	bool mDrawFacingDirection;
};

