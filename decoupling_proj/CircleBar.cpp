#include "CircleBar.h"
#include "SpiritCoreChangedEvent.h"

const float finalRadius = 5.f;

CircleBar::CircleBar(const sf::Color& color)
:mColor(color)
{
}


CircleBar::~CircleBar()
{
}


void CircleBar::reactToChangingValue(EventBase* eventBase)
{
	if (eventBase->getEventType() == EventType::SpiritCoreChanged){
		SpiritCoreChangedEvent* spiritChangedEvent = dynamic_cast<SpiritCoreChangedEvent*>(eventBase);
		int currentSpiritCores = spiritChangedEvent->mCurrentSpiritCores;

		while (mCircleList.size() < currentSpiritCores)
			addNewCircle();

		while (mCircleList.size() > currentSpiritCores)
			mCircleList.erase(mCircleList.begin() + (mCircleList.size() - 1));


	}
}

void CircleBar::drawCircles(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& circle : mCircleList){
		target.draw(circle, states);
		
		states.transform.translate((circle.getRadius() * 2.f) + 5.f, 0.f);
	}
}

void CircleBar::addNewCircle()
{
	sf::CircleShape circleShape;
	circleShape.setFillColor(mColor);
	circleShape.setRadius(finalRadius);
	circleShape.setOutlineThickness(0.f);

	mCircleList.push_back(circleShape);
}