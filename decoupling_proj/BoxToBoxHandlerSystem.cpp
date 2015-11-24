#include "BoxToBoxHandlerSystem.h"
#include <SFML/System/Vector2.hpp>
#include "RotatedRect.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "TransformableComponent.h"
#include "SpiritFormComponent.h"
#include "SpiritGrabberComponent.h"
#include "CategoryComponent.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "CreateNewEntityEvent.h"
#include "EventManager.h"
#include "Constant.h"
#include "Utility.h"

BoxToBoxHandlerSystem::BoxToBoxHandlerSystem()
{
	//pushRequiredComponent(ComponentIdentifier::BoxCollisionComponent);
	pushEntity1Requirement(ComponentIdentifier::BoxCollisionComponent);
	pushEntity2Requirement(ComponentIdentifier::BoxCollisionComponent);
}


BoxToBoxHandlerSystem::~BoxToBoxHandlerSystem()
{
}


bool BoxToBoxHandlerSystem::isBothEntitesCollided(Entity* entity1, Entity* entity2, sf::Time dt,
	CollisionComponent*& entity1Col, CollisionComponent*& entity2Col)
{
	SpiritFormComponent* mainSpiritComp = entity1->nonCreateComp<SpiritFormComponent>();
	CategoryComponent* opposingCategoryComp = entity2->nonCreateComp<CategoryComponent>();

	if (!mainSpiritComp){
		mainSpiritComp = entity2->nonCreateComp<SpiritFormComponent>();
		opposingCategoryComp = entity1->nonCreateComp<CategoryComponent>();
	}

	if (mainSpiritComp && opposingCategoryComp	&& mainSpiritComp->isOnSpiritForm() &&
		mainSpiritComp->isEntityInsideExcemptionList(opposingCategoryComp->mOwnerEntity))
	{
		return false;
	}

	sf::FloatRect bound1, bound2;
	bound1 = entity1->comp<BoxCollisionComponent>()->getTransfromedRect();
	bound2 = entity2->comp<BoxCollisionComponent>()->getTransfromedRect();

	if (!bound1.intersects(bound2))
		return false;

	entity1Col = entity1->comp<BoxCollisionComponent>();
	entity2Col = entity2->comp<BoxCollisionComponent>();

	if (mainSpiritComp && opposingCategoryComp &&
		mainSpiritComp->isOnSpiritForm() &&
		mainSpiritComp->getPassedThroughCategory() & opposingCategoryComp->getCategory())
	{
		HealthComponent* opposingHealthComp = opposingCategoryComp->mOwnerEntity
			->nonCreateComp<HealthComponent>();
		StaminaComponent* mainStaminaComp = mainSpiritComp->mOwnerEntity
			->nonCreateComp<StaminaComponent>();

		SpiritGrabberComponent* opposingSpiritGrabber = opposingCategoryComp->mOwnerEntity
			->nonCreateComp<SpiritGrabberComponent>();

		if (opposingHealthComp)
			opposingHealthComp->damage(mainSpiritComp->getDamageToPassedThroughCategory()
				, mainSpiritComp->mOwnerEntity);

		if (mainStaminaComp && mainSpiritComp->isExcemptionListEmpty())
			mainStaminaComp->increaseCurStamina(mainSpiritComp->getStaminaIncrease());

		if (opposingSpiritGrabber && opposingSpiritGrabber->getHealthSpiritGrabbed() >= 1.f){
			int finalSpiritReleaseNum = static_cast<int>((opposingSpiritGrabber->getHealthSpiritGrabbed()));
			
			opposingSpiritGrabber->emptyHealthSpiritGrabbed();

			CreateNewEntityEvent::Ptr createEntitiesEvent(new CreateNewEntityEvent());

			QueueEntityScriptData::EngineInitializeFunc initializeFunc;
			sf::Vector2f mainReverseDir = Utility::unitVector(mainSpiritComp->
				mOwnerEntity->comp<VelocityComponent>()->getVelocity()) * -1.f;

			sf::Vector2f opposingWorldPos = opposingSpiritGrabber->mOwnerEntity
				->comp<TransformableComponent>()->getWorldPosition(true);

			initializeFunc = [mainReverseDir, opposingWorldPos](Entity* newEntity){
				float finalAngle = Utility::vectorToDegree(mainReverseDir, false) + 
					(static_cast<float>(Utility::randomRange(0, 181)) - 90.f);
				sf::Vector2f finalDir = Utility::degreeToVector(finalAngle);
				newEntity->comp<VelocityComponent>()->setVelocity(finalDir);
				newEntity->comp<TransformableComponent>()->setPosition(opposingWorldPos);
			};

			for (int i = 0; i < finalSpiritReleaseNum; i++)
				createEntitiesEvent->queueEntityFromEngine(
					"Foreground", scriptDir + "HealthSpiritScript.lua", 
					"HealthSpirit", &initializeFunc);

			EventManager::getInstance()->queueEvent(std::move(createEntitiesEvent));
		}

		mainSpiritComp->insertExcemptionEntity(opposingCategoryComp->mOwnerEntity);
		return true;
	}


	sf::Vector2f pos1(bound1.left + bound1.width / 2.f,
		bound1.top + bound1.height / 2.f);
	sf::Vector2f pos2(bound2.left + bound2.width / 2.f,
		bound2.top + bound2.height / 2.f);

	bool pos1XIsLarger = true;
	bool pos1YIsLarger = true;

	if (pos2.x > pos1.x) pos1XIsLarger = false;
	if (pos2.y > pos1.y) pos1YIsLarger = false;

	RotatedRect rect1(bound1);
	RotatedRect rect2(bound2);

	sf::Vector2f point1, point2;

	if (!pos1XIsLarger && !pos1YIsLarger)
		point1 = rect1.mPoints[RotatedRect::BottomRight],
		point2 = rect2.mPoints[RotatedRect::TopLeft];
	else if (!pos1XIsLarger && pos1YIsLarger)
		point1 = rect1.mPoints[RotatedRect::TopRight],
		point2 = rect2.mPoints[RotatedRect::BottomLeft];
	else if (pos1XIsLarger && !pos1YIsLarger)
		point1 = rect1.mPoints[RotatedRect::BottomLeft],
		point2 = rect2.mPoints[RotatedRect::TopRight];
	else if (pos1XIsLarger && pos1YIsLarger)
		point1 = rect1.mPoints[RotatedRect::TopLeft],
		point2 = rect2.mPoints[RotatedRect::BottomRight];

	sf::Vector2f diff(point2.x - point1.x, point2.y - point1.y);
	VelocityComponent* entity1VeloComp = entity1->nonCreateComp<VelocityComponent>();
	VelocityComponent* entity2VeloComp = entity2->nonCreateComp<VelocityComponent>();

	sf::Vector2f addition(0.f, 0.f);

	TransformableComponent* transformable1 = entity1->comp<TransformableComponent>();
	TransformableComponent* transformable2 = entity2->comp<TransformableComponent>();

	//sf::Vector2f otherVelo = entity2->comp<VelocityComponent>()->getVelocity();
	

	if (std::abs(diff.x) > std::abs(diff.y))	addition.y = diff.y;
	else	addition.x = diff.x;


	bool entity1Push, entity2Push;
	entity1Push = entity2Push = true;

	if (entity1VeloComp && entity1VeloComp->getVelocity() == sf::Vector2f())
		entity1Push = false;
	if (entity2VeloComp && entity2VeloComp->getVelocity() == sf::Vector2f())
		entity2Push = false;

	if (!entity1Push && !entity2Push){
		entity1Push = entity2Push = true;
		addition /= 2.f;
	}

	if (entity1Push)
		CollisionHandlerSystem::pushCollidedEntity(entity1, addition);
	if (entity2Push)
		CollisionHandlerSystem::pushCollidedEntity(entity2, addition * -1.f);

	/*velo *= -1.f;
	if ((velo.x != 0.f && otherVelo.x == velo.x) ||
		((velo.y != 0.f && otherVelo.y == velo.y)))
	{
		addition /= 2.f;
		transformable2->mTransformable.setPosition(transformable2->mTransformable.getPosition() + (addition * -1.f));
	}*/

	//transformable1->mTransformable.setPosition(transformable1->mTransformable.getPosition() + addition);

	return true;
}