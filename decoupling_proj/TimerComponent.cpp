#include "TimerComponent.h"

TimerData::TimerData()
:mElapsedTime(sf::Time::Zero),
mTimeLimit(sf::Time::Zero)
{}


TimerComponent::TimerComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::TimerComponent;
}


TimerComponent::~TimerComponent()
{
}


void TimerComponent::updateEntireTimers(float seconds)
{
	for (auto& data : mTimerDataList)
		data.second.mElapsedTime += sf::seconds(seconds);
}

void TimerComponent::updateTimer(const std::string& timerName, float dt)
{
	auto iter = mTimerDataList.find(timerName);
	if (iter == mTimerDataList.end())
		return;

	iter->second.mElapsedTime += sf::seconds(dt);
}

void TimerComponent::setNewTimeLimitToTimer(const std::string& timerName, float newSeconds)
{
	auto iter = mTimerDataList.find(timerName);
	if (iter == mTimerDataList.end())
		return;

	iter->second.mTimeLimit = sf::seconds(newSeconds);
}

bool TimerComponent::isTimerElapsed(const std::string& timerName) const
{
	auto iter = mTimerDataList.find(timerName);
	if (iter == mTimerDataList.end())
		return false;

	return iter->second.mElapsedTime >= iter->second.mTimeLimit;
}