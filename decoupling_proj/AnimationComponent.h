#pragma once
#include "Component.h"
#include "Animation.h"

class TextureStringManager;

struct AnimationData{
	AnimationData();

	// the origin of each frame in the sprite sheet, usually its top left
	// also contains other utility data like individual duration and frame zie
	Animation::OrigList origList; 

	std::string textureLocation;
	//how long the animation are going to be
	//will be used in constrast to origList size, if the individual has 0 value
	sf::Time wholeDuration;	
	//each frame's size
	//get the same treatment as wholeDuration, see above
	sf::Vector2i wholeFrameSize;	
	bool flipX; //whether this animtion needs to be flipped horizontally or not
	bool endToStart;	//whether this animation will ran back from end to start
	bool repeat;	//whether this animation is repeating

	sf::Vector2f transformOrigin;
};

class AnimationComponent :	public Component{
public:
	AnimationComponent(Entity* entity);
	virtual ~AnimationComponent();

	void pauseAnimation();
	void resumeAnimation();

	void setTextureStringManager(TextureStringManager* texturesStrManager);

	bool isAnimationFinished() const;
	void restartAnimation();

	void updateAnimation(sf::Time dt);

	void setAnimation(const std::string& animName);


	void insertNewAnimData(const std::string& animName, 
		const AnimationData& animData);
private:
	void setRealAnimation(sf::Sprite& sprite, bool repeat,
		const sf::Time& duration, const sf::Vector2i& frameSize,
		bool flipX, bool endToStart, const Animation::OrigList& list);

private:
	Animation mAnimationUpdater;

	std::string mCurrentAnimationName;

	std::unordered_map<std::string, AnimationData> mAnimationsData;

	TextureStringManager* mTexturesStringManager;
};

