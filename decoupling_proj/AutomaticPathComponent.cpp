#include "AutomaticPathComponent.h"
#include "TransformableComponent.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "Entity.h"
#include "VelocityComponent.h"

AutomaticPathComponent::AutomaticPathComponent(Entity* entity)
:Component(entity),
mUncalculatedDest(0.f, 0.f),
mCurrentDestNode(nullptr)
{
	mIdentifier = ComponentIdentifier::AutomaticPathComponent;
	mUncalculatedDest = getNoNeedCalculationVal();
}


AutomaticPathComponent::~AutomaticPathComponent()
{
}


std::vector<MovingNode>& AutomaticPathComponent::getAutomaticPaths()
{
	return mAutomaticPaths;
}


void AutomaticPathComponent::setDestinationToCalculate(AStarNode* destNode)
{
	setDestinationToCalculate(destNode->pos);
}

void AutomaticPathComponent::setDestinationToCalculate(const sf::Vector2f& pos)
{
	setDestinationToCalculate(pos.x, pos.y);
}

void AutomaticPathComponent::setDestinationToCalculate(float posX, float posY)
{
	mUncalculatedDest.x = posX;
	mUncalculatedDest.y = posY;
}

void AutomaticPathComponent::setDestToCalculate(float posX, float posY)
{
	setDestinationToCalculate(posX, posY);
}

void AutomaticPathComponent::setAutomaticPaths(const std::vector<MovingNode>& automaticPaths, 
	AStarNode* finalDestNode)
{
	mAutomaticPaths = automaticPaths;
	mCurrentDestNode = finalDestNode;
	mUncalculatedDest = getNoNeedCalculationVal();
}

void AutomaticPathComponent::clearAutomaticPaths()
{
	mAutomaticPaths.clear();
	if (mOwnerEntity->hasComp<VelocityComponent>())
		mOwnerEntity->comp<VelocityComponent>()->setVelocity(0.f, 0.f, false);
}

AStarNode* AutomaticPathComponent::getCurrentDestNode() const
{
	return mCurrentDestNode;
}

sf::Vector2f AutomaticPathComponent::getUncalculatedDest() const
{
	return mUncalculatedDest;
}

sf::Vector2f AutomaticPathComponent::getNoNeedCalculationVal() const
{
	return sf::Vector2f(-10000.f, -10000.f);
}

bool AutomaticPathComponent::isUncalculatedPosValid() const
{
	return mUncalculatedDest != getNoNeedCalculationVal();
}


bool AutomaticPathComponent::isAutomaticPathsEmpty() const
{
	return mAutomaticPaths.empty();
}

void AutomaticPathComponent::drawAutomaticPaths(sf::RenderTarget& target)
{


	sf::VertexArray vertexs(sf::LinesStrip);
	vertexs.append(sf::Vertex(mOwnerEntity->comp<TransformableComponent>()->getWorldPosition(true)));

	sf::VertexArray boxVertexs(sf::Quads);

	for (int i = mAutomaticPaths.size() - 1; i >= 0; i--){
		vertexs.append(sf::Vertex(mAutomaticPaths[i].starNode->pos));
		//box
		boxVertexs.append(sf::Vertex(mAutomaticPaths[i].starNode->pos +
			sf::Vector2f(-5.f, -5.f), sf::Color(sf::Color::Yellow)));
		boxVertexs.append(sf::Vertex(mAutomaticPaths[i].starNode->pos +
			sf::Vector2f(5.f, -5.f), sf::Color(sf::Color::Yellow)));
		boxVertexs.append(sf::Vertex(mAutomaticPaths[i].starNode->pos +
			sf::Vector2f(5.f, 5.f), sf::Color(sf::Color::Yellow)));
		boxVertexs.append(sf::Vertex(mAutomaticPaths[i].starNode->pos +
			sf::Vector2f(-5.f, 5.f), sf::Color(sf::Color::Yellow)));
		//boxVertexs.append(sf::Vertex(tempPaths[i].starNode->pos +
		//sf::Vector2f(-5.f, -5.f), sf::Color(sf::Color::Yellow)));

	}

	target.draw(vertexs);
	target.draw(boxVertexs);
}