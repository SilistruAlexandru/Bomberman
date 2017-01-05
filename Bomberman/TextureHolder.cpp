#include "TextureHolder.h"
#include "SFML/Graphics.hpp"



void TextureHolder::load(Textures::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);

	mTextureMap.insert(std::make_pair(id, std::move(texture)));
}
