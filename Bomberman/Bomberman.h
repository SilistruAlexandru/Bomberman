#pragma once
#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Bomberman
{
private:

public:
	Bomberman(float startX, float startY);
	void update(sf::Time deltaTime);

	Vector2f position;
	Vector2f destination;
	Vector2f movement;
	Texture texture;
	Sprite sprite;
	Text movement_text;
	Font font;
	bool isMoving;
	char direction[6];
};
#endif 