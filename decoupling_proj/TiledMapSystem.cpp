#include "TiledMapSystem.h"
#include "SceneNode.h"
//#include "Tile.h"
#include "Constant.h"
#include "ResourceHolder.h"

TiledMapSystem::TiledMapSystem(TextureHolderInt& texture)
:mTextures(texture),
mSize(0.f, 0.f),
mTileSize(0.f, 0.f),
mMap(),
mTileList(),
mLayers(),
mTmxMap(new tmxparser::TmxMap())
{
}


TiledMapSystem::~TiledMapSystem()
{
}
//returns the tileId for map.
unsigned int TiledMapSystem::At(std::string layer
	, unsigned int x, unsigned int y) const
{
	auto& layerVector = mMap.find(layer);
	assert(layerVector != mMap.end());

	return layerVector->second[x + mSize.x * y];
}

unsigned int TiledMapSystem::At(std::string layer, Coord pos) const
{
	return TiledMapSystem::At(layer, pos.x, pos.y);
}

ObjectRectCollection TiledMapSystem::layerToObjectData(std::string layer)
{
	return mObjects[layer];
}

sf::Vector2f TiledMapSystem::coordToPosition(Coord pos)
{
	return TiledMapSystem::coordToPosition(pos.x, pos.y);
}

sf::Vector2f TiledMapSystem::coordToPosition(unsigned int x, unsigned int y)
{
	return sf::Vector2f((x * mTileSize.x + mTileSize.x / 2),
		(y * mTileSize.y + mTileSize.y / 2));
}


//Example : if the tileId is 74 and it was located on tileSet with id
//of 73 and size of 10 individual tile, the logic should be 
//73 >= x <= 82 (10 + 73 - 1), since individual tile can also the same numberId with
// the tileSetId
unsigned int TiledMapSystem::tileToTileSet(int tileGId)
{
	for (auto& m : mTileList){
		
		if (tileGId >= m.first && tileGId <= (m.second.size() + m.first - 1))
			return m.first;
	}
}

//don't forget that vector itself start from 0
//(if size is 73, means its 0 - 72)
//and example if the tileId is 74 and the tileSetId is 73 with the size
//of 10 individual tile, if we are to put 74 onto a vector
//with only a size of 10, it does not make sense, however if we did
//tileId(74) - tileSetId(73), it will become 1 and can be accessed
//from our vector
sf::IntRect TiledMapSystem::tileSetToRect(int tileSetId, int tileId)
{	
	return mTileList[tileSetId][tileId - tileSetId].tileRect;
}

//use the same concept as the tileSetToRect function to
//have access to the tileData
std::string TiledMapSystem::propertyKeyToValue(int tileSetId,
	int tileId, const std::string& key)
{
	tmxparser::TmxPropertyMap_t& temp = mTileList[tileSetId][tileId - tileSetId].properties;
	auto find = temp.find(key);

	if (find != temp.end())
		return find->second;
	
	return std::string("");
}

void TiledMapSystem::loadMap(const std::string& filePath)
{
	tmxparser::TmxReturn error = tmxparser::parseFromFile(filePath, mTmxMap.get());
	tmxparser::TmxMap t;
	tmxparser::parseFromFile(filePath, &t);
	if (error == tmxparser::TmxReturn::kErrorParsing){
		throw std::runtime_error("Failed to load map");
		return;
	}
	mSize = sf::Vector2f(mTmxMap->width, mTmxMap->height);
	mTileSize = sf::Vector2f(mTmxMap->tileWidth, mTmxMap->tileHeight);

	tmxparser::TmxTilesetCollection_t& tileSets = mTmxMap->tilesetCollection;

	for (tmxparser::TmxTileset& tileSet : tileSets){
		//individual tile's size
		float tileWidth, tileHeight;
		//the tileSet's/Spritesheeet image size
		float imageWidth, imageHeight;
		//image's size after being filtered by space and margin;
		float filImageWidth, filImageHeight;
		//temp var
		int maxX, maxY;

		/*if (tileSet._tiles.size() != 0)
			std::cout << tileSet._tiles.begin()->propertyMap.begin()->first << std::endl;*/
		tileWidth = tileSet.tileWidth;
		tileHeight = tileSet.tileHeight;

		imageWidth = tileSet.image.width;
		imageHeight = tileSet.image.height;

		maxX = imageWidth / tileWidth;
		maxY = imageHeight / tileHeight;

		filImageWidth = (imageWidth - (maxX * tileSet.tileSpacingInImage)) - tileSet.tileMarginInImage;
		filImageHeight = (imageHeight - (maxY * tileSet.tileSpacingInImage)) - tileSet.tileMarginInImage;
		
		//int totalTile = (imageWidth / tileWidth) * (imageHeight / tileHeight);
		int totalTile = (filImageWidth / tileWidth) * (filImageHeight / tileHeight);
		
		//imageWidth will be divided by tileWidth and vice versa to count
		//how many individual tile in X region and Y region
		//tileSet._tiles.size()
		populateRect(tileSet.firstgid, tileSet._tiles.size(), filImageWidth, filImageHeight,
			tileSet.tileMarginInImage, tileSet.tileSpacingInImage,
			sf::Vector2f(tileWidth, tileHeight));

		//we are going to set any custom properties for individual tile found in the .tmx file
		populateProperties(tileSet);

		//if there is transperency to be done, we used sf::Image resources to mask the color we wanted
		//to be transparent and load it again later using sf::Texture
		//if (tileSet.image.transparentColor != ""){
		sf::Image img;
		img.loadFromFile(levelDir + tileSet.image.source);
		if (tileSet.image.transparentColor != "")
			img.createMaskFromColor(hexToColor(tileSet.image.transparentColor));
			
		mTextures.loadFromImage(tileSet.firstgid, img);
		continue;
		//}
		//mTextures.load(tileSet.firstgid, levelDir + tileSet.image.source);
		
	}

	buildMap();
	buildObject();
}

void TiledMapSystem::populateRect(unsigned int tileSetId, int totalTile, 
	float imageWidth, float imageHeight, int margin, int spacing,
	const sf::Vector2f& tileSetTileSize)
{
	mTileList[tileSetId].resize(totalTile);
	
	float fMaxX = (imageWidth - 1) / tileSetTileSize.x;
	float fMaxY = (imageHeight - 1) / tileSetTileSize.y;
	int count = 0;
	fMaxX = std::floor(fMaxX + 0.5f);
	fMaxY = std::floor(fMaxY + 0.5f);

	int maxX = static_cast<int>(fMaxX);
	int maxY = static_cast<int>(fMaxY);

	for (int y = 0; y < maxY; y++){
		for (int x = 0; x < maxX; x++){
			mTileList[tileSetId][count].tileRect = 
				sf::IntRect((margin + x * tileSetTileSize.x) + (spacing * x)
				, (margin + y * tileSetTileSize.y) + (spacing * y)
				, tileSetTileSize.x, tileSetTileSize.y);
			count++;
		}
	}
}

//call this function after populateRect was called
void TiledMapSystem::populateProperties(const tmxparser::TmxTileset& tileSet)
{
	for (const tmxparser::TmxTileDefinition& t : tileSet._tiles){
		for (Property p : t.propertyMap){
			//the t.id is not the same with the At function returns,
			//the t.id here is relative to the each individual tileSet
			//notice here is only "id" not "gid"
			mTileList[tileSet.firstgid][t.id].properties
				.insert(std::make_pair(p.first, p.second));
		}
	}
}

void TiledMapSystem::buildMap()
{
	tmxparser::TmxLayerCollection_t& layerSet = mTmxMap->layerCollection;
	int count;
	for (tmxparser::TmxLayer& layer : layerSet){
		mMap[layer.name].resize(layer.tiles.size());
		//mLayers will be used later for buildObject() function as well
		mLayers.push_back(layer.name);
		count = 0;
		for (tmxparser::TmxLayerTile& tile : layer.tiles){
			mMap[layer.name][count] = tile.gid;
			count++;
		}
	}
}


void TiledMapSystem::buildObject()
{
	tmxparser::TmxObjectGroupCollection_t& objectSet = mTmxMap->objectGroupCollection;
	int count;
	for (tmxparser::TmxObjectGroup& group : objectSet){
		mObjects[group.name].resize(group.objects.size());
		count = 0;
		for (tmxparser::TmxObject& object : group.objects){
			mObjects[group.name][count].objectRect = 
				sf::IntRect(object.x, object.y, object.width, object.height);
			mObjects[group.name][count].objectType = 
				static_cast<ObjectsType::ID>( std::atoi(object.type.c_str()) );
			mObjects[group.name][count].objectName = object.name;
			count++;
		}
	}
}


sf::Color TiledMapSystem::hexToColor(std::string hex)
{
	if (hex.size() != 6)
		return sf::Color();
	
	sf::Uint8 r, g, b;
	
	int number[6];

	int count = 0;
	for (char c : hex){
		number[count] = charToNumber(c);
		count++;
	}
	//formula from hex to rgb value
	r = number[0] * 16 + number[1];
	g = number[2] * 16 + number[3];
	b = number[4] * 16 + number[5];
		
	return sf::Color(r, g, b);
}

int TiledMapSystem::charToNumber(char c)
{
	std::string ab = "";
	ab[0] = c;
	int temp;

	switch (c){
	case 'A':
	case 'a':
		temp = 10;
		break;
	case 'B':
	case 'b':
		temp = 11;
		break;
	case 'C':
	case 'c':
		temp = 12;
		break;
	case 'D':
	case 'd':
		temp = 13;
		break;
	case 'E':
	case 'e':
		temp = 14;
		break;
	case 'F':
	case 'f':
		temp = 15;
		break;
	default:
		temp = std::atoi(ab.c_str());
		break;
		
	}

	return temp;
}