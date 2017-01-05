#include "AssetManager.h"
#include <assert.h>
#include <iostream>
#include <fstream>


using namespace sf;
using namespace std;

AssetManager *AssetManager::sInstance = nullptr;

AssetManager::AssetManager()
{	
	//only allow one AssetManager to exist, otherwise throw an exception
	assert(sInstance == nullptr);
	sInstance = this;
	
}

Texture &AssetManager::GetTexture(string const& filename)
{
	auto& texMap = sInstance->m_Textures;

	//check if the texture is already loaded
	auto pairFound = texMap.find(filename);
	//If yes, return the texture
	if (pairFound != texMap.end())
	{
		return pairFound->second;
	}
	else
		//Else, load the texture and return it
	{	//Create an element in the texture map
		auto& texture = texMap[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}

 void  AssetManager::loadAllTextures()
{
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/block.PNG");
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/blockdestroyable.PNG");
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/bomb.png");
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/bomberman.png");
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/destroyable_block.PNG");
	 GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/explosion.png");

}
