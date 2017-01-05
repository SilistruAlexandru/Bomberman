#pragma once
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include "SFML/Graphics.hpp"

namespace Textures
{
	enum ID { Bomberman, Bomb, Destroyable_Block, Undrestoyable_Block };
}

class TextureHolder
{
private:
	std::map<Textures::ID, std::unique_ptr<sf::Texture>> mTextureMap;

	void load(Textures::ID id, const std::string& filename);
};