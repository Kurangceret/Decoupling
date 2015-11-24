#include "FogOfWar.h"
#include "CategoryComponent.h"
#include "SpriteComponent.h"
#include "VertexNodeComponent.h"
#include "GeneralData.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "Utility.h"
#include "TransformableComponent.h"

FogOfWar::FogOfWar(GeneralData* generalData)
:mTexturesInt(*generalData->getTextureHolderInt())
{
	
}


FogOfWar::~FogOfWar()
{
}


void FogOfWar::createFog(const sf::Vector2u& fogSize)
{
	//mMapTexture.~Texture();
	mMapTexture.create(fogSize.x, fogSize.y);
	mToBeRenderedTexture.create(fogSize.x, fogSize.y);
	sf::Uint8* pixels = new sf::Uint8[fogSize.x * fogSize.y * 4];
	//std::unique_ptr<sf::Uint8> pixels(new sf::Uint8[fogSize.x * fogSize.y * 4]);
	for (int i = 0; i < fogSize.x * fogSize.y * 4; i += 4){
		pixels[i] = 0;
		pixels[i + 1] = 0;
		pixels[i + 2] = 0;
		pixels[i + 3] = 0;
	}
	//std::cout << "Texture max size :" << mMapTexture.getMaximumSize() << std::endl;
	mMapTexture.update(pixels);
	mToBeRenderedTexture.update(pixels);
	delete pixels;

	
}

void FogOfWar::updateFog(const std::vector<Entity*>& entities, const sf::Vector2f& centerPoint, 
	float sightRadius)
{
	mCenterPoint = centerPoint;
	mSightRadius = sightRadius;
	//mToBeRenderedTexture = mMapTexture;

	for (auto entity : entities){
		if (!entity->hasComp<CategoryComponent>())
			continue;

		CategoryComponent* categoryComp = entity->comp<CategoryComponent>();
		if (categoryComp->getCategory() & Category::Player)
			continue;

		VertexNodeComponent* vertexNode = entity->nonCreateComp<VertexNodeComponent>();
		
		
		if (vertexNode){
			intializeVertexNodeComp(vertexNode);
			continue;
		}

		SpriteComponent* spriteComp = entity->nonCreateComp<SpriteComponent>();
		TransformableComponent* transformableComp = entity->nonCreateComp<TransformableComponent>();
		if (spriteComp && transformableComp)
			initializeSpriteTransformableComp(spriteComp, transformableComp);
					
		
	}
}

void FogOfWar::intializeVertexNodeComp(const VertexNodeComponent* vertexNode)
{
	initializeVertexsGiven(vertexNode->getVertices(), vertexNode->getTileSet());
}

void FogOfWar::initializeSpriteTransformableComp(const SpriteComponent* spriteComp,
	const TransformableComponent* trasformComp)
{
	const sf::Texture* texture = spriteComp->mSprite.getTexture();
	sf::Vector2f topLeftPos = trasformComp->getWorldPosition();


	sf::VertexArray vertices(sf::Quads);
	
	sf::IntRect textureRect = spriteComp->mSprite.getTextureRect();
	//const sf::Sprite& sprite = spriteComp->mSprite;

	vertices.append(sf::Vertex(
		sf::Vector2f(topLeftPos.x, topLeftPos.y),
		sf::Vector2f(textureRect.left, textureRect.top)
		));
	vertices.append(sf::Vertex(
		sf::Vector2f(topLeftPos.x + textureRect.width, topLeftPos.y),
		sf::Vector2f(textureRect.left + textureRect.width, textureRect.top)
		));
	vertices.append(sf::Vertex(
		sf::Vector2f(topLeftPos.x + textureRect.width, topLeftPos.y + textureRect.height),
		sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)
		));
	vertices.append(sf::Vertex(
		sf::Vector2f(topLeftPos.x, topLeftPos.y + textureRect.height),
		sf::Vector2f(textureRect.left, textureRect.top + textureRect.height)
		));

	initializeVertexsGiven(vertices, texture);
}


void FogOfWar::initializeVertexsGiven(const sf::VertexArray& vertices,
	const sf::Texture* tileSet)
{
	//const sf::VertexArray& vertices = vertexNode->getVertices();
	Utility::CircleGeometry circle(mCenterPoint, mSightRadius);
	for (int i = 0; i < vertices.getVertexCount(); i += 4){
		const sf::Vertex& topLeft = vertices[i];
		const sf::Vertex& topRight = vertices[i + 1];
		const sf::Vertex& bottomRight = vertices[i + 2];
		const sf::Vertex& bottomLeft = vertices[i + 3];

		sf::FloatRect floatRect(topLeft.position.x, topLeft.position.y, 
			topRight.position.x - topLeft.position.x, bottomLeft.position.y - topLeft.position.y);

		/*if (!Utility::isPointInsideImaginaryCircle(mCenterPoint, mSightRadius, topLeft.position) &&
			!Utility::isPointInsideImaginaryCircle(mCenterPoint, mSightRadius, topRight.position) &&
			!Utility::isPointInsideImaginaryCircle(mCenterPoint, mSightRadius, bottomRight.position) &&
			!Utility::isPointInsideImaginaryCircle(mCenterPoint, mSightRadius, bottomLeft.position))
			continue;*/
		
		if (!Utility::isCircleIntersectsWithFloatRect(circle, floatRect))
			continue;

		//get the texture that the vertex is using
		//const sf::Texture* tileSet = vertexNode->getTileSet();
		//get the corresponding img also
		sf::Image* tileImg = mTexturesInt.textureKeyToImg(tileSet);
		
		if (!tileImg)
			continue;

		sf::Vector2u imgSize = tileImg->getSize();

		//is the size of the current vertex's texture size
		sf::Vector2f textureRectSize;
		textureRectSize.x = topRight.texCoords.x - topLeft.texCoords.x;
		textureRectSize.y = bottomLeft.texCoords.y - topLeft.texCoords.y;

		//the top left position of the 4 vertexs combined together
		sf::Vector2f vertexPosInGame = topLeft.position;
		

		const sf::Uint8* tilePixels = tileImg->getPixelsPtr();

		//img we use to put into our mMapTexture
		/*sf::Image finalImageToTxt;
		finalImageToTxt.create(textureRectSize.x, textureRectSize.y);

		

	

		//for (float txCoord = 0.f; txCoord < textureRectSize.x; txCoord++){

		for (float tyCoord = 0.f; tyCoord < textureRectSize.y; tyCoord++){
			for (float txCoord = 0.f; txCoord < textureRectSize.x; txCoord++){

				int a = (topLeft.texCoords.x + txCoord) +
					(imgSize.x *  (topLeft.texCoords.y + tyCoord));

				a *= 4;

				finalImageToTxt.setPixel(txCoord, tyCoord,
					sf::Color(
					tilePixels[a],
					tilePixels[a + 1],
					tilePixels[a + 2],
					20));
			}
		}
		mMapTexture.update(finalImageToTxt, vertexPosInGame.x, vertexPosInGame.y);*/

		float finalSize = textureRectSize.x * textureRectSize.y * 4.f;
		int totalIndex = static_cast<int>(finalSize);
		std::unique_ptr<sf::Uint8> newPixels(new sf::Uint8[totalIndex]);
		sf::Uint8* ptrToPixel = newPixels.get();

		int newI = 0;
		for (float tyCoord = 0.f; tyCoord < textureRectSize.y; tyCoord++){
			for (float txCoord = 0.f; txCoord < textureRectSize.x; txCoord++){

				int a = (topLeft.texCoords.x + txCoord) +
					(imgSize.x *  (topLeft.texCoords.y + tyCoord));

				a *= 4;
				ptrToPixel[newI] = tilePixels[a];
				ptrToPixel[newI + 1] = tilePixels[a + 1];
				ptrToPixel[newI + 2] = tilePixels[a + 2];
				ptrToPixel[newI + 3] = 20;

				newI += 4;
			}
		}
		mMapTexture.update(ptrToPixel, textureRectSize.x, textureRectSize.y, vertexPosInGame.x, vertexPosInGame.y);
		
	}
}

void FogOfWar::drawFog(sf::RenderTarget& target, const sf::Vector2f& centerPoint,
	float sightRadius)
{
	sf::Vector2u size = mMapTexture.getSize();

	/*sf::VertexArray vertexs(sf::Quads);
	vertexs.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)));
	vertexs.append(sf::Vertex(sf::Vector2f(800.f, 0.f), sf::Vector2f(size.x, 0.f)));
	vertexs.append(sf::Vertex(sf::Vector2f(800.f, 600.f), sf::Vector2f(size.x, size.y)));
	vertexs.append(sf::Vertex(sf::Vector2f(0.f, 600.f), sf::Vector2f(0.f, size.y)));

	sf::RenderStates states;
	states.texture = &mMapTexture;*/


	//target.draw(vertexs);
	//sf::Texture textureTest;
	
	sf::Sprite realSprite;
	realSprite.setTexture(mMapTexture);
	target.draw(realSprite);

	sf::Sprite sprite;
	sprite.setTexture(mToBeRenderedTexture);

	target.draw(sprite, sf::BlendAdd);
}