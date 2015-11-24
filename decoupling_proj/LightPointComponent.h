#pragma once
#include "Component.h"
#include <LTBL\Light\Light_Point.h>

class ComponentArranger;

class LightPointComponent :	public Component{
public:
	typedef std::unique_ptr<LightPointComponent> Ptr;
	friend ComponentArranger;
public:
	LightPointComponent(Entity* entity);
	virtual ~LightPointComponent();

	//will update based on owner entity's position
	void updateLightCenter(sf::Time dt, const sf::Vector2u& windowSize);

	void createNewLightPoint(bool alwaysUpdate = false);
	ltbl::Light_Point* getLightPoint();

	bool isAlwaysUpdate() const;

	void setNoLightForOneFrame(bool flag);
	bool noLightForOneFrame() const;

	void setCurLightRadius(float val);
	float getCurLightRadius() const;
private:
	ltbl::Light_Point* mLightPoint;
	bool mIsAlwaysUpdate;
	bool mNoLightForOneFrame;

	float mMaxLightRadius;
	
	float mMinLightRadius;
};

