#pragma once
#include <tinyxml2.h>
#include <tmxparser.h>
#include <map>
#include "Textures.h"
#include <string>
#include <memory>
#include "ObjectsType.h"

class SceneNode;


typedef struct{
	//individual tile's rect
	sf::IntRect tileRect;
	//individual tile's properties
	tmxparser::TmxPropertyMap_t properties;
}TileData;



//for rect object only
typedef struct{
	//individual rect
	sf::IntRect objectRect;
	//the type of object
	//not the type of the shape or individual tile
	ObjectsType::ID objectType;
	//name of the object
	std::string objectName;

}ObjectRectData;

typedef std::vector<ObjectRectData> ObjectRectCollection;

class TiledMapSystem{
public:
	typedef sf::Vector2u Coord;
	typedef std::pair<std::string, std::string> Property;
public:
	TiledMapSystem(TextureHolderInt& texture);
	~TiledMapSystem();

	void loadMap(const std::string& filePath);

	sf::Vector2f getMapSize() const { return mSize; }

	//Example : if the tileId is 74 and it was located on tileSet with id
	//of 73 and size of 10 individual tile, the logic should be 
	//73 >= x <= 82 (10 + 73 - 1), since individual tile can also the same numberId with
	// the tileSetId
	unsigned int tileToTileSet(int tileGId);

	//don't forget that vector itself start from 0
	//(if size is 73, means its 0 - 72)
	//and example if the tileId is 74 and the tileSetId is 73 with the size
	//of 10 individual tile, if we are to put 74 onto a vector
	//with only a size of 10, it does not make sense, however if we did
	//tileId(74) - tileSetId(73), it will become 1 and can be accessed
	//from our vector
	sf::IntRect tileSetToRect(int tileSetId, int tileId);

	//used the same concept as tileSetToRect function for accessing individual tile
	std::string propertyKeyToValue(int tileSetId, int tileId, const std::string& key);

	unsigned int At(std::string layer, unsigned int x, unsigned int y) const;
	unsigned int At(std::string layer, Coord cord) const;

	ObjectRectCollection layerToObjectData(std::string layer);

	sf::Vector2f coordToPosition(unsigned int x, unsigned int y);
	sf::Vector2f coordToPosition(Coord pos);

	std::vector<std::string>& getLayersList(){ return mLayers; }
	std::vector<std::string> getLayersList() const { return mLayers; }

	sf::Vector2f getTileSize() const { return mTileSize; }

private:
	void populateRect(unsigned int tileSetId, int totalTile, float imageWidth, float imageHeight,
		int margin, int spacing, const sf::Vector2f& tileSetTileSize);

	//call this function after populateRect was called
	void populateProperties(const tmxparser::TmxTileset& tileSet);

	void buildMap();
	void buildObject();
	

	sf::Color hexToColor(std::string hex);
	int charToNumber(char c);
private:
	std::unique_ptr<tmxparser::TmxMap> mTmxMap;
	//unsigned int will contain the ID for the texture which 
	//is being hold by TextureHolder, while the std::vector<IntRect>
	//will contains the individual tile in the tileset
	std::map<unsigned int, std::vector<TileData> > mTileList;
	//std::map<unsigned int, std::vector<sf::IntRect> > mTileList;
	//'std::vector<int>' will hold all the map data and the int will be the
	//identifier's id for mTileList
	//the std::string will hold the layer name
	std::map < std::string ,std::vector<unsigned int>  > mMap;
	//the key type of string will hold the layer name, so each object 
	//in this map correspond to the mMap container's layer
	std::map < std::string, ObjectRectCollection >  mObjects;
	//size of the map
	sf::Vector2f mSize;
	//size of the tile on the map
	//not the size of the individual tile
	//since every tileset has their own inidividual tile
	sf::Vector2f mTileSize;
	//references to our textureholderint
	TextureHolderInt& mTextures;
	//will hold the name of the layer used for 'mMap' variable
	//and the mObjects variable
	std::vector<std::string> mLayers;
};

