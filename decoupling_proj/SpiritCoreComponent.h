#pragma once
#include "Component.h"
#include <SFML/System/Time.hpp>


class ComponentArranger;

class SpiritCoreComponent :	public Component{
public:
	friend ComponentArranger;
public:
	SpiritCoreComponent(Entity* entity);
	virtual ~SpiritCoreComponent();

	void updateRestoreStatus(sf::Time dt);

	void startRestoring();
	bool isRestoring() const;

	void restoreSpiritCoreFully();

	void increaseSpiritCore(int val);
	void decreaseSpiritCore(int val);

	int getCurrentSpiritCore() const;
	int getMaxSpiritCore() const;
	
	bool noSpiritCoreLeft() const;

	float getCurTimeRatioInRestoring() const;

private:
	int mCurrentSpiritCore;
	int mMaxSpiritCore;

	sf::Time mRestoreTimePerCore;
	sf::Time mElapsedTime;
};

