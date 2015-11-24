#include "MeleeRectAngleUpdater.h"
#include "Utility.h"

MeleeRectAngleUpdater::MeleeRectAngleUpdater()
{
}


MeleeRectAngleUpdater::~MeleeRectAngleUpdater()
{
}

bool MeleeRectAngleUpdater::isDelaying() const
{
	if (mPosAngleDataList.empty())
		return false;

	return mPosAngleDataList[mCurrentIndex].delayDuration > sf::Time::Zero || 
		mWholeDelayDuration > sf::Time::Zero;
}

bool MeleeRectAngleUpdater::isUpdating() const
{
	return mIsUpdating;
}

sf::Vector2f MeleeRectAngleUpdater::getImaginaryPos() const
{
	return mCurrentImaginaryPos;
}

sf::Vector2f MeleeRectAngleUpdater::getOffSetPos() const
{
	return mCurrentOffSetPos;
}


void MeleeRectAngleUpdater::update(sf::Time dt)
{
	if (!mIsUpdating)
		return;

	if (mPosAngleDataList[mCurrentIndex].delayDuration > sf::seconds(0.f))
		mPosAngleDataList[mCurrentIndex].delayDuration -= dt;

	if (mWholeDelayDuration > sf::seconds(0.f))
		mWholeDelayDuration -= dt;

	sf::Time currentSwingDur = mPosAngleDataList[mCurrentIndex].swingDuration;
	if (currentSwingDur == sf::Time::Zero)
		currentSwingDur = mWholeSwingDuration;

	mElapsedTime += dt;

	if (mElapsedTime >= currentSwingDur && 
		mCurrentIndex + 1 < mPosAngleDataList.size())
	{
		mElapsedTime -= currentSwingDur;

		if (mCurrentIndex != 0){
			mCurrentImaginaryPos = mPosAngleDataList[mCurrentIndex].position;
			mCurrentOffSetPos = mPosAngleDataList[mCurrentIndex].offSetPos;
		}

		sf::Vector2f diff = mPosAngleDataList[mCurrentIndex + 1].position -
			mPosAngleDataList[mCurrentIndex].position;

		mMovingDirection = Utility::unitVector(diff);

		sf::Vector2f offSetDiff = mPosAngleDataList[mCurrentIndex + 1].offSetPos -
			mPosAngleDataList[mCurrentIndex].offSetPos;

		mOffSetDirection = Utility::unitVector(offSetDiff);
		mCurrentIndex++;
		
		
		currentSwingDur = mPosAngleDataList[mCurrentIndex].swingDuration == sf::Time::Zero ?
			mWholeSwingDuration : mPosAngleDataList[mCurrentIndex].swingDuration;

		mMovingSpeed = Utility::vectorLength(diff) / currentSwingDur.asSeconds();
		mOffSetSpeed = Utility::vectorLength(offSetDiff) / currentSwingDur.asSeconds();
	}
	else if(mMovingDirection == sf::Vector2f()
		&& mOffSetDirection == sf::Vector2f() && 
		mCurrentIndex + 1 >= mPosAngleDataList.size())
	{
		mIsUpdating = false; //ended this instance of updating
		return;
	}

	//mElapsedTime += dt;

	if (mElapsedTime >= currentSwingDur){
		//mElapsedTime -= currentSwingDur;
		mMovingDirection = sf::Vector2f();
		mOffSetDirection = sf::Vector2f();
		return;
	}

	mCurrentImaginaryPos += mMovingDirection * dt.asSeconds() * mMovingSpeed;
	mCurrentOffSetPos += mOffSetDirection * dt.asSeconds() * mOffSetSpeed;
}


void MeleeRectAngleUpdater::startAngleUpdater(const std::vector<PosAngleData>& posAngles,
	sf::Time wholeSwingDuration, sf::Time wholeDelayDuration)
{
	if (posAngles.empty())
		return;

	mPosAngleDataList = posAngles;
	mWholeSwingDuration = sf::seconds(wholeSwingDuration.asSeconds() / posAngles.size());
	mWholeDelayDuration = sf::seconds(wholeDelayDuration.asSeconds() / posAngles.size());
	mIsUpdating = true;
	mCurrentIndex = 0;
	mCurrentImaginaryPos = posAngles.front().position;
	mCurrentOffSetPos = posAngles.front().offSetPos;
	mElapsedTime = sf::Time::Zero;
	mMovingDirection = sf::Vector2f();
	mOffSetDirection = sf::Vector2f();

	/*if (posAngles.size() > 1){
		mMovingDirection = posAngles[1].position
	}*/
}

void MeleeRectAngleUpdater::stopUpdating()
{
	mIsUpdating = false;
}