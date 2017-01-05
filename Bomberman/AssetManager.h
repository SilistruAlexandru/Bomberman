#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>

using namespace sf;
using namespace std;


class AssetManager
{
public:
	AssetManager();
	
	static Texture& GetTexture(string const& filename);
	static void loadAllTextures();

private:
	map<string, Texture> m_Textures;
	static AssetManager *sInstance;
	

};

#endif 
