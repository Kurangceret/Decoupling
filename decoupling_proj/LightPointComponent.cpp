#include "LightPointComponent.h"
#include "TransformableComponent.h"
#include "Entity.h"

LightPointComponent::LightPointComponent(Entity* entity)
:Component(entity),
mIsAlwaysUpdate(false),
mLightPoint(nullptr),
mNoLightForOneFrame(false),
mMaxLightRadius(0.f),
mMinLightRadius(0.f)
{
	mIdentifier = ComponentIdentifier::LightPointComponent;
}


LightPointComponent::~LightPointComponent()
{
}

void LightPointComponent::updateLightCenter(sf::Time dt, const sf::Vector2u& windowSize)
{
	if (!mLightPoint)
		return;
	
	TransformableComponent* transformComp = mOwnerEntity->nonCreateComp<TransformableComponent>();
	if (!transformComp)
		return;

	sf::Vector2f ownerPos = transformComp->getWorldPosition(true);
	//ownerPos -= transformComp->mTransformable.getOrigin() / 2.f;


	mLightPoint->SetCenter(Vec2f(ownerPos.x, windowSize.y - ownerPos.y));
}

ltbl::Light_Point* LightPointComponent::getLightPoint()
{
	return mLightPoint;
}

void LightPointComponent::createNewLightPoint(bool alwaysUpdate)
{
	mLightPoint = new ltbl::Light_Point();
	mIsAlwaysUpdate = alwaysUpdate;
}

bool LightPointComponent::isAlwaysUpdate() const
{
	return mIsAlwaysUpdate;
}

void LightPointComponent::setNoLightForOneFrame(bool flag)
{
	mNoLightForOneFrame = flag;
}

bool LightPointComponent::noLightForOneFrame() const
{
	return mNoLightForOneFrame;
}

void LightPointComponent::setCurLightRadius(float val)
{
	float finalVal = val;
	if (finalVal > mMaxLightRadius)
		finalVal = mMaxLightRadius;
	if (finalVal < mMinLightRadius)
		finalVal = mMinLightRadius;

	mLightPoint->SetRadius(finalVal);
}

float LightPointComponent::getCurLightRadius() const
{
	return mLightPoint->m_radius;
}