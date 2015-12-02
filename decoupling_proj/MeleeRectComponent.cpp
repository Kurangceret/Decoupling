#include "MeleeRectComponent.h"
#include "Utility.h"
#include "Entity.h"
#include "TransformableComponent.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "StaminaComponent.h"
#include "Category.h"

SlashData::SlashData(float ang, const sf::Vector2f& pos, float duration,
	float delayDuration)
:angle(ang), movingPos(pos),
mDuration(sf::seconds(duration)),
mDelayDuration(sf::seconds(delayDuration))
{}
SlashData::SlashData(float ang, float x, float y, float duration ,
	float delayDuration)
: angle(ang), movingPos(x, y),
mDuration(sf::seconds(duration)),
mDelayDuration(sf::seconds(delayDuration))
{}


EntityMovingData::EntityMovingData()
:speedIdentifier(0.0f)
{}

MeleeRectComponent::MeleeRectComponent(Entity* entity)
:CollisionComponent(entity),
mCurrentMoveSetName(""),
mNowIsCollideable(false),
mCurrentRecoveryDur(sf::Time::Zero),
mCurrentVulnerableDur(sf::Time::Zero),
mStartEntityMovingData(false),
mCurrentAdditionalRange(0.f)
{
	mIdentifier = ComponentIdentifier::MeleeRectComponent;
}


MeleeRectComponent::~MeleeRectComponent()
{
}

void MeleeRectComponent::update(sf::Time dt)
{
	StaminaComponent* staminaComp = mOwnerEntity->nonCreateComp<StaminaComponent>();
	VelocityComponent* veloComp = mOwnerEntity->nonCreateComp<VelocityComponent>();

	mNowIsCollideable = false;
	mAngleUpdater.update(dt);

	if (!mAngleUpdater.isUpdating() && mStartEntityMovingData && veloComp)
	{
		mStartEntityMovingData = false;
		veloComp->setSpeedIdentifier(1.f);
		veloComp->setVelocity(0.f, 0.f, false);
	}

	if (mAngleUpdater.isUpdating() && staminaComp)
		staminaComp->setStaminaRecover(false);

	if (!mAngleUpdater.isDelaying() && mAngleUpdater.isUpdating()){
		mNowIsCollideable = true;

		/*if (staminaComp)
			staminaComp->setStaminaRecover(false);*/
		calculateMeleeRect();

		if (!mStartEntityMovingData && veloComp){
			mStartEntityMovingData = true;
			veloComp->setSpeedIdentifier(mMovementSetList[mCurrentMoveSetName].entityMovingData.speedIdentifier);
			
		}
		if (mStartEntityMovingData && veloComp)
			veloComp->setVelocity(mCurrentAttackDir);

		return;
	}

	if (mAngleUpdater.isDelaying())
		return;

	if (mCurrentVulnerableDur.asSeconds() > 0.f){
		mCurrentVulnerableDur -= dt;
		return;
	}

	if (mCurrentRecoveryDur.asSeconds() > 0.f){
		mCurrentRecoveryDur -= dt;
		return;
	}
	
	//if (mAngleUpdater.isUpdating())
		
}

void MeleeRectComponent::insertNewMovementSetData(const std::string& movementName,
	const MovementSetData& movementSetData)
{
	if (mMovementSetList.find(movementName) == mMovementSetList.end()){
		mMovementSetList.insert(std::make_pair(movementName, movementSetData));
		return;
	}

	mMovementSetList[movementName] = movementSetData;
}

void MeleeRectComponent::setAttackCategory(std::size_t newAttackCategory)
{
	mAttackCategory = newAttackCategory;
}

std::size_t MeleeRectComponent::getAttackCategory() const
{
	return mAttackCategory;
}

bool MeleeRectComponent::isNowCollidedable() const
{
	return mNowIsCollideable;
}

const RotatedRect& MeleeRectComponent::getRotatedRect() const
{
	return mRotatedRect;
}

bool MeleeRectComponent::meleeRectIsUpdating() const
{
	return mAngleUpdater.isUpdating();
}

float MeleeRectComponent::getMovementSetStaminaUsage(const
	std::string& movementSetName)
{
	float finalVal = 0.f;
	if (mMovementSetList.find(movementSetName) == mMovementSetList.end())
		return finalVal;

	const MovementSetData& movementSetData = mMovementSetList[movementSetName];
	return movementSetData.staminaUsage;
}

bool MeleeRectComponent::startMeleeRectScript(const sf::Vector2f& direction,
	const std::string& movementSetName)
{
	return startMeleeRect(direction, movementSetName, 0.f);
}

bool MeleeRectComponent::startMeleeRect(const sf::Vector2f& direction,
	const std::string& movementSetName, float additionalRelativeAngle,
	float additionalRange)
{
	if (mAngleUpdater.isUpdating() || direction == sf::Vector2f())
		return false;

	if (mMovementSetList.find(movementSetName) == mMovementSetList.end())
		return false;
		

	MovementSetData& movementSetData = mMovementSetList[movementSetName];
	StaminaComponent* staminaComp = mOwnerEntity->nonCreateComp<StaminaComponent>();

	if (staminaComp && !staminaComp->checkDecreaseStamina(movementSetData.staminaUsage))
		return false;

	if (staminaComp)
		staminaComp->decreaseCurStamina(movementSetData.staminaUsage);

	mTouchedEntities.clear();
	mCurrentAttackDir = direction;
	
	//Utility::printVector( Utility::degreeToVector(Utility::vectorToDegree(direction)), true) ;

	std::vector<PosAngleData> finalAnglePosList;

	

	mCurrentRecoveryDur = movementSetData.recoveryDur;
	mCurrentVulnerableDur = movementSetData.vulnerableDur;
	finalAnglePosList.resize(movementSetData.slashes.size());

	mCurrentAdditionalRange = additionalRange;
	float degreeFromDir = Utility::vectorToDegree(direction, false);
	float range = movementSetData.range;

	float dirX = direction.x > 0.f ? 1.f : -1.f;
	if (direction.x == 0.f)
		dirX = 0.f;


	float dirY = direction.y > 0.f ? 1.f : -1.f;
	if (direction.y == 0.f)
		dirY = 0.f;

	float dirToCalc = dirX == 0.f ? dirY : dirX;
	//dirToCalc = 1.f;
	if (dirX >= 0.f)
		dirToCalc = 1.f;

	for (int i = 0; i < movementSetData.slashes.size(); i++){
		//if (slashesData[i].angle != 0.f)
		PosAngleData posAngle;
		float finalAngle = movementSetData.slashes[i].angle;

		if (finalAngle != 0.f && additionalRelativeAngle != 0.f){
			if (finalAngle < 0.f && additionalRelativeAngle > 0.f)				
				finalAngle -= std::abs(additionalRelativeAngle);
			else if (finalAngle < 0.f && additionalRelativeAngle < 0.f)
				finalAngle += std::abs(additionalRelativeAngle);
			else
				finalAngle += additionalRelativeAngle;
		}
		posAngle.position = ((Utility::degreeToVector(degreeFromDir +
			(finalAngle) * dirToCalc)) * range);

		posAngle.swingDuration = movementSetData.slashes[i].mDuration;
		posAngle.delayDuration = movementSetData.slashes[i].mDelayDuration;

		sf::Vector2f offSet = movementSetData.slashes[i].movingPos;
		posAngle.offSetPos = sf::Vector2f(offSet.x * direction.x, offSet.y * direction.y);

		finalAnglePosList[i] = posAngle;
	}
	
	mCurrentMoveSetName = movementSetName;
	
	

	mAngleUpdater.startAngleUpdater(finalAnglePosList, movementSetData.wholeDuration, 
		movementSetData.wholeDelayDur);

	return true;
}

sf::Vector2f MeleeRectComponent::getTransformedMeleeRectPos() const
{
	if (!mOwnerEntity->hasComp<TransformableComponent>())
		return sf::Vector2f();

	/*sf::Transform originedTransform;
	originedTransform.translate(mOwnerEntity->comp<TransformableComponent>()->getWorldTransform(true));*/

	return mOwnerEntity->comp<TransformableComponent>()->getWorldTransform(true).transformPoint(
		mAngleUpdater.getImaginaryPos());
}

void MeleeRectComponent::calculateMeleeRect()
{
	if (!mOwnerEntity->hasComp<TransformableComponent>() || 
		!mOwnerEntity->hasComp<BoxCollisionComponent>())
		return;

	MovementSetData& movementSetData = mMovementSetList[mCurrentMoveSetName];

	float halfThick = movementSetData.thickness / 2.f;

	TransformableComponent* transformComp = mOwnerEntity->comp<TransformableComponent>();
	sf::Vector2f entityWorldPos = transformComp->getWorldPosition(true);
	//sf::Vector2f entityWorldPos = transformComp->getTransformWithOrigin() * sf::Vector2f();
	//transformComp->getWorldPosition();

	float range = movementSetData.range + mCurrentAdditionalRange;

	/*sf::Transform originedTransform;
	originedTransform.translate(entityWorldPos);

	sf::Vector2f swordCurPos = originedTransform.transformPoint(
		mAngleUpdater.getImaginaryPos());*/

	sf::Vector2f swordCurPos = getTransformedMeleeRectPos();

	sf::Vector2f dir = Utility::unitVector(swordCurPos - entityWorldPos);

	BoxCollisionComponent* boxCollisionComp = mOwnerEntity->comp<BoxCollisionComponent>();
	sf::FloatRect transformedRect(boxCollisionComp->getTransfromedRect());

	sf::Vector2f halfOfRect(boxCollisionComp->mBoundingRect.width / 2.f,
		boxCollisionComp->mBoundingRect.height / 2.f);

	halfOfRect.x *= dir.x;
	halfOfRect.y *= dir.y;

	float dirDegree = Utility::vectorToDegree(dir, false);
	sf::Vector2f dirPlus = Utility::degreeToVector(dirDegree + 90.f);
	sf::Vector2f dirMinus = Utility::degreeToVector(dirDegree + -90.f);


	sf::Vector2f offSet = mAngleUpdater.getOffSetPos();

	sf::Vector2f leftNavelPoint = (dirPlus * halfThick) + offSet;
	sf::Vector2f rightNavelPoint = (dirMinus * halfThick) + offSet;

	sf::Vector2f rightTipPoint = entityWorldPos + halfOfRect 
		+ (dir * range) + rightNavelPoint;
	sf::Vector2f leftTipPoint = entityWorldPos + halfOfRect 
		+ (dir * range) + leftNavelPoint;

	rightNavelPoint = entityWorldPos + halfOfRect
		+ rightNavelPoint;
	leftNavelPoint = entityWorldPos + halfOfRect
		+ leftNavelPoint;

	mRotatedRect.mPoints[RotatedRect::TopLeft] = leftNavelPoint;
	mRotatedRect.mPoints[RotatedRect::BottomLeft] = rightNavelPoint;
	mRotatedRect.mPoints[RotatedRect::TopRight] = leftTipPoint;
	mRotatedRect.mPoints[RotatedRect::BottomRight] = rightTipPoint;
}


void MeleeRectComponent::stopMeleeRectUpdating()
{
	mAngleUpdater.stopUpdating();
	mNowIsCollideable = false;
}

float MeleeRectComponent::getCurrentDamage() const
{
	auto iter = mMovementSetList.find(mCurrentMoveSetName);
	if (iter == mMovementSetList.end())
		return 0.f;
	
	return iter->second.damage;
}

void MeleeRectComponent::insertTouchedEntity(const Entity* entity, float d)
{
	mTouchedEntities.insert(std::make_pair(entity, d));
}

bool MeleeRectComponent::entityHasBeenTouched(const Entity* entity) const
{
	return (mTouchedEntities.find(entity) != mTouchedEntities.end());
}


bool MeleeRectComponent::isVulnerable() const
{
	return mCurrentVulnerableDur.asSeconds() > 0.f;
}

bool MeleeRectComponent::isRecovering() const
{
	return mCurrentRecoveryDur.asSeconds() > 0.f;
}

bool MeleeRectComponent::meleeRectIsDelaying() const
{
	return mAngleUpdater.isDelaying();
}