#pragma once
#include "Component.h"


class ComponentArranger;

class EntityExpertiseComponent :	public Component{
public:
	friend ComponentArranger;
public:
	EntityExpertiseComponent(Entity* entity);
	virtual ~EntityExpertiseComponent();

	void setAbleToFloat(bool flag);
	bool isAbleToFloat() const;

	void setAbleToWalk(bool flag);
	bool isAbleToWalk() const;
private:
	//able to float/fly is the same
	bool mAbleToFloat;
	//means on floor/ground/soil
	bool mAbleToWalk;


};

