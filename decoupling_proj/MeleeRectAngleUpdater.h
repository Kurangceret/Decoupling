#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

struct PosAngleData{
	sf::Vector2f position;
	sf::Time delayDuration;
	sf::Time swingDuration;
	sf::Vector2f offSetPos;
};

class MeleeRectAngleUpdater{
public:
	MeleeRectAngleUpdater();
	~MeleeRectAngleUpdater();



	void update(sf::Time dt);

	void startAngleUpdater(const std::vector<PosAngleData>& posAngles,
		sf::Time wholeSwingDuration, sf::Time wholeDelayDuration);

	bool isDelaying() const;
	bool isUpdating() const;

	sf::Vector2f getImaginaryPos() const;
	sf::Vector2f getOffSetPos() const;

	
	void stopUpdating();
private:
	
	//bool mIsDelaying;
	bool mIsUpdating;

	std::vector<PosAngleData> mPosAngleDataList;
	
	float mMovingSpeed;
	sf::Vector2f mMovingDirection;

	sf::Time mWholeSwingDuration;
	sf::Time mWholeDelayDuration;
	sf::Time mElapsedTime;

	int mCurrentIndex;

	sf::Vector2f mCurrentImaginaryPos;

	sf::Vector2f mCurrentOffSetPos;
	sf::Vector2f mOffSetDirection;
	float mOffSetSpeed;
};

