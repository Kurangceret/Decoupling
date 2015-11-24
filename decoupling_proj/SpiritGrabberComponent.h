#pragma once
#include "Component.h"
class SpiritGrabberComponent :	public Component{
public:
	SpiritGrabberComponent(Entity* entity);
	virtual ~SpiritGrabberComponent();

	void increaseHealthSpiritGrabbed(float num);
	void decreaseHealthSpiritGrabbed(float num);
	float getHealthSpiritGrabbed() const;
	void emptyHealthSpiritGrabbed();


private:
	float mHealthSpiritGrabbed;
};

