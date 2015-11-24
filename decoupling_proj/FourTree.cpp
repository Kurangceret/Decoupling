#include "FourTree.h"
#include "TransformableComponent.h"
#include "BoxCollisionComponent.h"
#include "Entity.h"
#include "Utility.h"

FourTree::FourTree(int maxObjects, int maxLevels, sf::FloatRect bounds, int currentLevel)
:mMaxLevels(maxLevels),
mMaxObjects(maxObjects),
mBounds(bounds),
mCurrentLevel(currentLevel),
isSpilt(false),
mWorldLocation(0.f, 0.f)
{
	for (int i = 0; i < LocationCount; i++)
		mNodes[i] = NULL;

	if (mCurrentLevel < mMaxLevels){
		float subWidth = mBounds.width / 2;
		float subHeight = mBounds.height / 2;
		float x = mBounds.left;
		float y = mBounds.top;
		//top right
		FourTree::Ptr ptr1(new FourTree(maxObjects, maxLevels, 
			sf::FloatRect(x + subWidth, y, subWidth, subHeight), mCurrentLevel + 1));
		//top left
		FourTree::Ptr ptr2(new FourTree(maxObjects, maxLevels, 
			sf::FloatRect(x, y, subWidth, subHeight), mCurrentLevel + 1));
		//bottom left
		FourTree::Ptr ptr3(new FourTree(maxObjects, maxLevels, 
			sf::FloatRect(x, y + subHeight, subWidth, subHeight), mCurrentLevel + 1));
		//bottom right
		FourTree::Ptr ptr4(new FourTree(maxObjects, maxLevels, 
			sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight), mCurrentLevel + 1));

		mNodes[Top_Right] = std::move(ptr1);
		mNodes[Top_Left] = std::move(ptr2);
		mNodes[Bottom_Left] = std::move(ptr3);
		mNodes[Bottom_Right] = std::move(ptr4);
	}
			
	
}


FourTree::~FourTree()
{
}

void FourTree::draw(sf::RenderWindow& window)
{
	
	window.draw(mShape);
	if (!isSpilt)
		return;
	for (int i = 0; i < LocationCount; i++)
		mNodes[i]->draw(window);


}

void FourTree::clear()
{
	mEntities.clear();
	mWorldLocation = sf::Vector2f(0.f, 0.f);

	if (!isSpilt)
		return;

	for (int i = 0; i < LocationCount; i++)
		mNodes[i]->clear();
	
	isSpilt = false;
}

sf::FloatRect FourTree::getWorldBounds()
{
	sf::FloatRect bounds = mBounds;
	bounds.left += mWorldLocation.x;
	bounds.top += mWorldLocation.y;

	return bounds;
}

void FourTree::updateBounds(const sf::Vector2f& location)
{
	mWorldLocation = location;

	mShape.setSize(sf::Vector2f(mBounds.width, mBounds.height));
	mShape.setPosition(mBounds.left + location.x, mBounds.top + location.y);
	mShape.setOutlineColor(sf::Color::Red);
	mShape.setOutlineThickness(1.f);
	mShape.setFillColor(sf::Color::Transparent);
	if (mNodes[Top_Left] == NULL)
		return;

	if (!isSpilt)
		return;

	for (int i = 0; i < LocationCount; i++)
		mNodes[i]->updateBounds(location);
}

void FourTree::fillInFourTree(const std::vector<Entity*> entities)
{
	int i = 0;
	for (auto entity : entities){
		if (!entity->hasComp<BoxCollisionComponent>()
			|| !entity->hasComp<TransformableComponent>())
			continue;
		insert(entity);
		i++;
	}
	
}

void FourTree::insert(Entity *ptr)
{
	if (isSpilt){
		Indexes indexes = getIndexes(ptr);

		if (!indexes.empty()){
			for (int i : indexes)
				mNodes[i]->insert(ptr);
			return;
		}
	}


	mEntities.push_back(ptr);
	//std::cout << "wee";
	//if this level reaches it max objects, seperate it into 4 more cells
	if (!isSpilt && mEntities.size() > mMaxObjects && mCurrentLevel < mMaxLevels){
		//if (mNodes[0] == NULL)
			//split();
		isSpilt = true;

		int i = 0;
		while (i < mEntities.size()){
			
			Indexes indexes = getIndexes(mEntities[i]);
			if (!indexes.empty()){
				Entity *newEntity = deleteFromContainer(mEntities[i]);
				for (int i : indexes)
					mNodes[i]->insert(newEntity);
			}
			else
				i++;

		}
	}
}

FourTree::Indexes FourTree::getIndexes(Entity* ptr)
{
	sf::FloatRect boundingRect = ptr->comp<BoxCollisionComponent>()->getTransfromedRect();

	sf::Vector2f ptrPosition = ptr->comp<TransformableComponent>()->getWorldPosition(true);

	return getIndexes(ptrPosition, boundingRect);
}

FourTree::Indexes FourTree::getIndexes(const sf::Vector2f& centerRectPos,
		const sf::FloatRect& rect)
{
	Indexes indexes;
	indexes.resize(0);
	sf::FloatRect updatedBounds = mBounds;
	updatedBounds.top += mWorldLocation.y;
	updatedBounds.left += mWorldLocation.x;

	float verticalMidPoint = updatedBounds.left + (updatedBounds.width / 2);
	float horizontalMidPoint = updatedBounds.top + (updatedBounds.height / 2);

	/*TransformableComponent* transformableComp = ptr->comp<TransformableComponent>();
	BoxCollisionComponent* boxCollisionComp = ptr->comp<BoxCollisionComponent>();

	sf::FloatRect boundingRect = boxCollisionComp->getTransfromedRect();

	sf::Vector2f ptrPosition = transformableComp->getWorldPosition(true);*/

	sf::FloatRect boundingRect = rect;

	sf::Vector2f ptrPosition = centerRectPos;



	float topEntityY = ptrPosition.y - boundingRect.height / 2;
	float bottomEntityY = ptrPosition.y + boundingRect.height / 2;
	float leftEntityX = ptrPosition.x - boundingRect.width / 2;
	float rightEntityX = ptrPosition.x + boundingRect.width / 2;

	bool topQuardrant = (topEntityY < horizontalMidPoint);
	bool bottomQuardrant = (bottomEntityY > horizontalMidPoint);
	bool leftQuardrant = (leftEntityX < verticalMidPoint);
	bool rightQuardrant = (rightEntityX > verticalMidPoint);

	if (topQuardrant && leftQuardrant)
		indexes.push_back(1);
	if (topQuardrant && rightQuardrant)
		indexes.push_back(0);
	if (bottomQuardrant && leftQuardrant)
		indexes.push_back(2);
	if (bottomQuardrant && rightQuardrant)
		indexes.push_back(3);

	return indexes;
}

FourTree::Indexes FourTree::getIndexes(const RotatedRect& rotatedRect)
{
	Indexes indexes;
	indexes.resize(0);
	
	int i = 0;
	for (auto& index : mNodes){
		sf::FloatRect rect1 = index->mBounds;
		rect1.top += mWorldLocation.y;
		rect1.left += mWorldLocation.x;

		RotatedRect rotateRect1(rect1);
		if (Utility::rotatedCollision(rotatedRect, rotateRect1))
			indexes.push_back(i);
		i++;
	}
	return indexes;
}

void FourTree::getObjects(std::vector<Entity*>& entities, const sf::Vector2f& centerRectPos,
	const sf::FloatRect& rect)
{
	Indexes indexes;
	if (isSpilt)
		indexes = getIndexes(centerRectPos, rect);

	if (!indexes.empty() && isSpilt){
		for (int i : indexes)
			mNodes[i]->getObjects(entities, centerRectPos, rect);
		return;
	}

	for (Entity *entity : mEntities){
		if (std::find(entities.begin(), entities.end(), entity) == entities.end())
			entities.push_back(entity);

	}
}

void FourTree::getObjects(std::vector<Entity*>& entities, const RotatedRect& rotatedRect)
{
	Indexes indexes;
	if (isSpilt)
		indexes = getIndexes(rotatedRect);

	if (!indexes.empty() && isSpilt){
		for (int i : indexes)
			mNodes[i]->getObjects(entities, rotatedRect);
		return;
	}

	for (Entity *entity : mEntities){
		if (std::find(entities.begin(), entities.end(), entity) == entities.end())
			entities.push_back(entity);
	}
}

void FourTree::getObjects(std::vector<Entity *>& entities, Entity *ptr)
{
	
	sf::FloatRect boundingRect = ptr->comp<BoxCollisionComponent>()->getTransfromedRect();

	sf::Vector2f ptrPosition = ptr->comp<TransformableComponent>()->getWorldPosition(true);
	getObjects(entities, ptrPosition, boundingRect);
}


Entity * FourTree::deleteFromContainer(Entity *entit)
{
	auto wreckEntity = std::find_if(mEntities.begin(), mEntities.end(),
		[&](Entity *entity) -> bool { return entity == entit; });

	Entity *newEntity = *(wreckEntity);
	mEntities.erase(wreckEntity);
	return newEntity;
}


FourTree* FourTree::locationToPointer(FourTree::Location id) const
{
	return mNodes[static_cast<int>(id)].get();
}