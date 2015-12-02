#pragma once
#include "Component.h"
class FloatableComponent :	public Component{
public:
	FloatableComponent(Entity* entity);
	virtual ~FloatableComponent();

	void setIsFloating(bool flag);
	bool isFloating() const;
private:
	bool mIsFloating;
	
};

