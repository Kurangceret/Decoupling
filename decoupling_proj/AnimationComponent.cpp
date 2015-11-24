#include "AnimationComponent.h"
#include "Entity.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "TextureStringManager.h"
#include "TransformableComponent.h"

AnimationData::AnimationData()
:wholeDuration(sf::seconds(0.f)),
wholeFrameSize(),
flipX(false),
repeat(false),
endToStart(false){}

AnimationComponent::AnimationComponent(Entity* entity)
:Component(entity),
mAnimationUpdater(Animation::Type::SelectiveSheet),
mCurrentAnimationName(""),
mTexturesStringManager(nullptr)
{
	mIdentifier = ComponentIdentifier::AnimationComponent;
}


AnimationComponent::~AnimationComponent()
{
}


void AnimationComponent::pauseAnimation()
{
	mAnimationUpdater.setPause(true);
}

void AnimationComponent::resumeAnimation()
{
	mAnimationUpdater.setPause(false);
}
void AnimationComponent::setTextureStringManager(TextureStringManager* texturesStrManager)
{
	mTexturesStringManager = texturesStrManager;
}

void AnimationComponent::setAnimation(const std::string& animName)
{
	auto iter = mAnimationsData.find(animName);
	if (iter == mAnimationsData.end()){
		//std::cout << animName << " " << " Not found to be anim key" << std::endl;
		return;
	}
	if (mCurrentAnimationName == animName)
		return;

	AnimationData& animData = iter->second;

	if (!mOwnerEntity->hasComp<SpriteComponent>())
		return;

	sf::Sprite& sprite = mOwnerEntity->comp<SpriteComponent>()->mSprite;

	if (animData.textureLocation != "" && mTexturesStringManager){
		sf::Texture* textures = mTexturesStringManager->loadTextures(animData.textureLocation);
		if (textures){
			sprite.setTexture(*textures);

			//sf::Vector2i frameSize = animData.wholeFrameSize == sf::Vector2i() ?  
			//sprite.setTextureRect(animData.)
		}
	}
	if (animData.transformOrigin != sf::Vector2f(-1.f, -1.f)
		&& mOwnerEntity->hasComp<TransformableComponent>())
	{
		mOwnerEntity->comp<TransformableComponent>()->setOrigin(animData.transformOrigin);
	}

	setRealAnimation(sprite, animData.repeat, animData.wholeDuration, animData.wholeFrameSize,
		animData.flipX, animData.endToStart, animData.origList);
	mCurrentAnimationName = animName;
}

void AnimationComponent::setRealAnimation(sf::Sprite& sprite, bool repeat,
	const sf::Time& duration, const sf::Vector2i& frameSize,
	bool flipX, bool endToStart, const Animation::OrigList& list)
{
	mAnimationUpdater.restart();
	mAnimationUpdater.setPause(false);
	mAnimationUpdater.setForeignSprite(&sprite);
	mAnimationUpdater.setFlipX(flipX);
	mAnimationUpdater.setIsEndToStart(endToStart);
	mAnimationUpdater.setRepeating(repeat);
	mAnimationUpdater.setNumFrames(list.size());
	mAnimationUpdater.setDuration(duration);
	mAnimationUpdater.setFrameSize(frameSize);
	mAnimationUpdater.setSelectiveList(list, flipX);
}

bool AnimationComponent::isAnimationFinished() const
{
	return mAnimationUpdater.isFinished();
}

void AnimationComponent::restartAnimation()
{
	mAnimationUpdater.restart();
}

void AnimationComponent::updateAnimation(sf::Time dt)
{
	mAnimationUpdater.update(dt);
}

void AnimationComponent::insertNewAnimData(const std::string& animName,
	const AnimationData& animData)
{
	auto iter = mAnimationsData.find(animName);
	if (iter == mAnimationsData.end()){
		mAnimationsData.insert(std::make_pair(animName, animData));
		return;
	}

	mAnimationsData[animName] = animData;
}