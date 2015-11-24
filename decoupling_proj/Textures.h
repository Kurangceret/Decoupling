#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics/Texture.hpp>
#include <string>

//forward declaration
template<typename Resource, typename Identifier>
class ResourceHolder;


namespace Textures{
	enum ID{
		Player,
		TypeCount
	};
}


typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Texture, unsigned int> TextureHolderInt;
typedef ResourceHolder<sf::Texture, std::string> TextureHolderStr;
#endif