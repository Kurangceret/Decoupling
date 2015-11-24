#pragma once
#include "Component.h"
#include <vector>
#include <string>

class ComponentArranger;
class AttackModeComponent :	public Component{
public:
	friend ComponentArranger;
public:
	AttackModeComponent(Entity* entity);
	virtual ~AttackModeComponent();


	void incrementIndex();
	std::string getCurrentStringIndex() const;

private:
	std::vector<std::string> mAttackModeList;
	int mCurrentModeIndex;
};

