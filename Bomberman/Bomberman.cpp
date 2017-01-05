#include "SFML/Graphics.hpp"
#include "Bomberman.h"
#include "AssetManager.h"
#include <cstring>


using namespace std;
using namespace sf;

Bomberman::Bomberman(float startX, float startY)
{
	position.x = startX;
	position.y = startY;
	destination.x = startX;
	destination.y = startY;
	movement.x = 0;
	movement.y = 0;
	isMoving = false;
	strcpy(direction, "down");
	texture.loadFromFile("Resources/Textures/bomberman.png");
	//Texture texture(GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Resources/Textures/bomberman.png"));
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(3, 3, 50, 50));
	sprite.setPosition(position.x, position.y);
	font.loadFromFile("Resources/Fonts/calibri.ttf");
	movement_text.setFont(font);

}
void Bomberman::update(sf::Time deltaTime)
{	
	
	sprite.move(movement*deltaTime.asSeconds());
	//position.x = sprite.getPosition().x; 
	//position.y = sprite.getPosition().y;
	movement_text.setString("x=" + to_string(position.x) + " y=" + to_string(position.y) + "\n");

	if ((int)(position.x) == (int)(destination.x) && (int) position.y == (int) destination.y)
	{
		isMoving = false;
		movement_text.setString("Done");
	}
	movement.x = 0;
	movement.y = 0;
	
}