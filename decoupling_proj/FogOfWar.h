#pragma once
#include "EntityManager.h"
#include "Textures.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>


class GeneralData;
class VertexNodeComponent;
class SpriteComponent;
class TransformableComponent;

class FogOfWar{
public:
	FogOfWar(GeneralData* generalData);
	~FogOfWar();

	void createFog(const sf::Vector2u& fogSize);
	void updateFog(const std::vector<Entity*>& entities, const sf::Vector2f& centerPoint,
		float sightRadius);


	void drawFog(sf::RenderTarget& target, const sf::Vector2f& centerPoint, 
		float sightRadius);
private:
	void intializeVertexNodeComp(const VertexNodeComponent* comp);
	void initializeSpriteTransformableComp(const SpriteComponent* spriteComp, 
		const TransformableComponent* trasformComp);

	void initializeVertexsGiven(const sf::VertexArray& vertices, 
		const sf::Texture* tileSet);
private:

	sf::Texture mMapTexture;
	sf::Texture mToBeRenderedTexture;
	TextureHolderInt& mTexturesInt;
	sf::Vector2f mCenterPoint;
	float mSightRadius;
};

