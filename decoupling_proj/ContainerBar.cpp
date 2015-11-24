#include "ContainerBar.h"
#include "PlayerStaminaChangedEvent.h"

ContainerBar::ContainerBar(const sf::Color& barColor)
:mBarColor(barColor)
{
	/*mOuterRect.setFillColor(sf::Color::Transparent);
	mOuterRect.setOutlineThickness(1.f);
	mOuterRect.setOutlineColor(sf::Color::Blue);
	mOuterRect.setSize(sf::Vector2f(70.f, 15.f));

	mFillRect.setFillColor(barColor);
	mFillRect.setSize(sf::Vector2f(70.f, 15.f));*/
}


ContainerBar::~ContainerBar()
{
}


void ContainerBar::setSize(const sf::Vector2f& size)
{
	/*mOuterRect.setSize(size);
	mFillRect.setSize(size);*/
}

void ContainerBar::setSize(float width, float height)
{
	setSize(sf::Vector2f(width, height));
}


void ContainerBar::drawContainers(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& container : mContainerList){
		target.draw(container.mOuterRect, states);
		target.draw(container.mFillRect, states);

		states.transform.translate(container.mOuterRect.getSize().x + 5.f, 0.f);
	}
}


void ContainerBar::reactToChangingValue(EventBase* eventBase)
{
	if (eventBase->getEventType() == EventType::PlayerStaminaChanged){
		PlayerStaminaChangedEvent* playerStaminaChangedEvent = dynamic_cast<PlayerStaminaChangedEvent*>(eventBase);

		float staminaIndexer = playerStaminaChangedEvent->mNumIndexer;
		float maxStamina = playerStaminaChangedEvent->mMaxStamina;

		int totalNum = std::ceil(maxStamina / staminaIndexer);
		while (mContainerList.size() < totalNum)
			addNewContainer();

		while (mContainerList.size() > totalNum)
			mContainerList.pop_back();

		float curStamina = playerStaminaChangedEvent->mCurrentStamina;

		int indexedStamina = static_cast<int>(curStamina) % static_cast<int>(staminaIndexer);

		int containerIndex = std::floor(curStamina  / staminaIndexer);


		indexedStamina = indexedStamina > 0 ? indexedStamina : 0;
		//curStamina = curStamina > 0.f ? curStamina : 0.f;

		int prevIndex = containerIndex - 1;
		int afterIndex = containerIndex + 1;

		while (prevIndex >= 0){
			ContainerData& prevContainer = mContainerList[prevIndex];
			prevContainer.mFillRect.setSize(sf::Vector2f(prevContainer.mOuterRect.getSize().x,
				prevContainer.mOuterRect.getSize().y));
			prevContainer.mFillRect.setFillColor(sf::Color::Green);
			prevIndex--;
		}

		while (afterIndex < mContainerList.size()){
			ContainerData& afterContainer = mContainerList[afterIndex];
			afterContainer.mFillRect.setSize(sf::Vector2f(afterContainer.mOuterRect.getSize().x,
				0.f));
			afterContainer.mFillRect.setFillColor(sf::Color::Yellow);
			afterIndex++;
		}

		/*if (indexedStamina == 0.f && containerIndex > 0.f){
			ContainerData& prevContainer = mContainerList[containerIndex - 1];
			prevContainer.mFillRect.setSize(sf::Vector2f(prevContainer.mOuterRect.getSize().x,
				prevContainer.mOuterRect.getSize().y));
		}*/

		if (containerIndex >= mContainerList.size())
			return;

		//float maxStamina = playerStaminaChangedEvent->mMaxStamina;
		ContainerData& container = mContainerList[containerIndex];

		container.mFillRect.setSize(sf::Vector2f(container.mOuterRect.getSize().x, 
			container.mOuterRect.getSize().y * (indexedStamina / staminaIndexer)));
		container.mFillRect.setFillColor(sf::Color::Yellow);
	}
}


void ContainerBar::addNewContainer()
{
	ContainerData containerData;
	containerData.mOuterRect.setFillColor(sf::Color::Transparent);
	containerData.mOuterRect.setOutlineThickness(1.f);
	containerData.mOuterRect.setOutlineColor(sf::Color::Blue);
	containerData.mOuterRect.setSize(sf::Vector2f(15.f, 25.f));

	containerData.mFillRect.setFillColor(mBarColor);
	containerData.mFillRect.setSize(sf::Vector2f(15.f, 25.f));

	mContainerList.push_back(containerData);
}