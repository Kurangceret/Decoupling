#include "AttackModeComponent.h"


AttackModeComponent::AttackModeComponent(Entity* entity)
:Component(entity),
mCurrentModeIndex(0)
{
	mIdentifier = ComponentIdentifier::AttackModeComponent;
}


AttackModeComponent::~AttackModeComponent()
{
}


void AttackModeComponent::incrementIndex()
{
	mCurrentModeIndex++;
	mCurrentModeIndex = mCurrentModeIndex % mAttackModeList.size();
}

std::string AttackModeComponent::getCurrentStringIndex() const
{
	return mAttackModeList[mCurrentModeIndex];
}