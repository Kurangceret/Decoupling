#include "RenderingSystem.h"
#include "ComponentIdentifier.h"
#include "DisplayComponent.h"
#include "TransformableComponent.h"
#include "BoxCollisionComponent.h"
#include "VelocityComponent.h"
#include "Entity.h"
#include "Utility.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "SpriteComponent.h"
#include "TextDisplayComponent.h"
#include "Constant.h"

RenderingSystem::RenderingSystem(sf::RenderWindow& renderWindow)
:mWindow(renderWindow)
{
	pushRequiredComponent(ComponentIdentifier::DisplayComponent);
	mFont.loadFromFile(fontDir + "Sansation.ttf");
}


RenderingSystem::~RenderingSystem()
{
}


void RenderingSystem::processEntity(sf::Time dt, Entity* entity)
{
	//DisplayComponent* displayComponent = entity->comp<DisplayComponent>();
	
	sf::RenderStates states;
	sf::Vector2f worldPos(0.f, 0.f);
	if (entity->hasComp<TransformableComponent>()){
		TransformableComponent* transformComp = entity->comp<TransformableComponent>();
		states.transform = transformComp->getWorldTransform();
		worldPos = transformComp->getWorldPosition(true);
	}

	if (entity->hasComp<SpriteComponent>()){
		SpriteComponent* spriteComp = entity->comp<SpriteComponent>();

		if (spriteComp->mElapsedPeriod < spriteComp->mPeriodBlink){
			Utility::drawFloatToInt(mWindow, states, worldPos,
				spriteComp->mSprite);
		}
		else{
			spriteComp->mElapsedPeriod -= spriteComp->mPeriodBlink;
		}
	}
	if (entity->hasComp<TextDisplayComponent>()){
		sf::Text& text = entity->comp<TextDisplayComponent>()->getText();
		text.setFont(mFont);

		Utility::drawFloatToInt(mWindow, states, worldPos, text);
	}

	//mWindow.draw(*displayComponent->getDrawable(), states);
	
	if (entity->hasComp<BoxCollisionComponent>() && entity->hasComp<VelocityComponent>()){
		sf::FloatRect rect = entity->comp<BoxCollisionComponent>()->getTransfromedRect();
		sf::RectangleShape shape;
		//sf::FloatRect rect = getBoundingRect();

		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);

		mWindow.draw(shape);
	}
}