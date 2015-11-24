#include "HarmfulBoxesToBoxHandlerSystem.h"
#include "Entity.h"
#include "HarmfulBoxesComponent.h"
#include "BoxCollisionComponent.h"
#include "TransformableComponent.h"

HarmfulBoxesToBoxHandlerSystem::HarmfulBoxesToBoxHandlerSystem()
{
	pushEntity1Requirement(ComponentIdentifier::HarmfulBoxesComponent);
	pushEntity1Requirement(ComponentIdentifier::TransformableComponent);
	pushEntity2Requirement(ComponentIdentifier::BoxCollisionComponent);
}


HarmfulBoxesToBoxHandlerSystem::~HarmfulBoxesToBoxHandlerSystem()
{
}



bool HarmfulBoxesToBoxHandlerSystem::isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
	CollisionComponent*& entity1Col, CollisionComponent*& entity2Col)
{
	HarmfulBoxesComponent* harmfulBoxComp1 = entity1->comp<HarmfulBoxesComponent>();
	sf::Transform worldTransform =  entity1->comp<TransformableComponent>()->getWorldTransform();

	BoxCollisionComponent* boxColComp2 = entity2->comp<BoxCollisionComponent>();

	std::vector<HarmfulBoxData>& harmfulBoxesData = harmfulBoxComp1->getHarmfulBoxesData();
	sf::FloatRect entity2Bound = boxColComp2->getTransfromedRect();

	entity1Col = harmfulBoxComp1;
	entity2Col = boxColComp2;

	int i = 0;
	bool foundAtLeastOne = false;

	for (auto& harmfulBoxData : harmfulBoxesData){
		if (!harmfulBoxData.mIsActivated)
			continue;

		sf::FloatRect transformedRect(worldTransform.transformRect(
			harmfulBoxData.mBoundingBox));

		if (transformedRect.intersects(entity2Bound)){
			foundAtLeastOne = true;
			harmfulBoxComp1->insertHarmfulBoxIndexForReactor(i);
		}
		i++;
	}

	return foundAtLeastOne;
}