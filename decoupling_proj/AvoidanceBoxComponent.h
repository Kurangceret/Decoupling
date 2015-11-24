#pragma once
#include "Component.h"
#include "RotatedRect.h"


class ComponentArranger;


class AvoidanceBoxComponent :	public Component{
	friend ComponentArranger;
public:
	AvoidanceBoxComponent(Entity* entity);
	virtual ~AvoidanceBoxComponent();

	float getRayLength() const;
	float getRayThickness() const;

	RotatedRect& getRayRect();

	void setCanBeRayCast(bool flag);
	bool currentlyCanBeRayCast() const;

	void setJustGotRayed(bool flag);
	bool justGotRayed() const;
private:
	RotatedRect mRayRect;
	float mRayLength;
	float mRayThickness;
	//if true entity with this object is applicable to be moved without consent.
	bool mCurrentlyCanBeRayCast;
	bool mJustGotRayed;
};

