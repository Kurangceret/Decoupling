#include "SpiritGrabberComponent.h"


SpiritGrabberComponent::SpiritGrabberComponent(Entity* entity)
:Component(entity),
mHealthSpiritGrabbed(0.0f)
{
	mIdentifier = ComponentIdentifier::SpiritGrabberComponent;
}


SpiritGrabberComponent::~SpiritGrabberComponent()
{
}


void SpiritGrabberComponent::increaseHealthSpiritGrabbed(float num)
{
	mHealthSpiritGrabbed += std::abs(num);
}

void SpiritGrabberComponent::decreaseHealthSpiritGrabbed(float num)
{
	mHealthSpiritGrabbed -= std::abs(num);
}

float SpiritGrabberComponent::getHealthSpiritGrabbed() const
{
	return mHealthSpiritGrabbed;
}

void SpiritGrabberComponent::emptyHealthSpiritGrabbed()
{
	mHealthSpiritGrabbed = 0.f;
}