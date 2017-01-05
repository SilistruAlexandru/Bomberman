#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
using namespace sf;

class Block
{
private:
	Vector2f position;
	//Sprite block = Sprite(AssetManager::GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Assets/block.png"));
	Sprite block;

public:
	Block(float startX, float startY);
	void moveLeft();
	void moveRight();
	void update();
	void render();
};