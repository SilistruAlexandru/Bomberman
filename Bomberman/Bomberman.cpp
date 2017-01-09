#include "SFML/Graphics.hpp"
#include "Bomberman.h"
#include "AssetManager.h"
#include <cstring>
#include <fstream>

#include "Game.h"

using namespace std;
using namespace sf;



Bomberman::Bomberman(int startX, int startY)
{
	position.x = startX;
	position.y = startY;
	destination.x = startX;
	destination.y = startY;
	movement.x = 0;
	movement.y = 0;
	speed = 120.f;
	isMoving = false;
	currentTime = Time::Zero;

	bomberman_down.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");

	moveLeft.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_left.png");
	moveLeft.looped = true;
	moveLeft.nFrames = 3;
	moveLeft.duration = seconds(0.5);
	//moveLeft.startFrameOffset = 1;

	idleLeft.texture = moveLeft.texture;
	idleLeft.looped = false;
	idleLeft.nFrames = 1;
	idleLeft.duration = seconds(1);

	moveRight.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_right.png");
	moveRight.looped = true;
	moveRight.nFrames = 3;
	moveRight.duration = seconds(0.5);
	//moveRight.startFrameOffset = 0;

	idleRight.texture = moveRight.texture;
	idleRight.looped = false;
	idleRight.nFrames = 1;
	idleRight.duration = seconds(1);

	moveUp.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_up.png");
	moveUp.looped = true;
	moveUp.nFrames = 3;
	moveUp.duration = seconds(0.5);
	//moveUp.startFrameOffset = 1;

	idleUp.texture = moveUp.texture;
	idleUp.looped = false;
	idleUp.nFrames = 1;
	idleUp.duration = seconds(1);

	moveDown.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");
	//moveDown.texture = AssetManager::GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");
	moveDown.looped = true;
	moveDown.nFrames = 3;
	moveDown.duration = seconds(0.5);
	//moveDown.startFrameOffset = 1;

	idleDown.texture = moveDown.texture;
	idleDown.looped = false;
	idleDown.nFrames = 1;
	idleDown.duration = seconds(1);

	strcpy(direction, "down");
	currentAnimation = moveRight;
	
	//currentAnimation.texture = moveLeft.texture;
	//texture.setSmooth(true);
	//texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_right.png");
	
	//sprite.setTexture(bomberman_down);
	//sprite.setTextureRect(IntRect(3, 3, 51, 51));
	sprite.setPosition(position.x, position.y);
	font.loadFromFile("Resources/Fonts/calibri.ttf");
	movement_text.setFont(font);

	//movingDownAnimation = animator.createAnimation("movingDownAnimation", texture, seconds(1), true));
	//ifstream fin_coord("bomberman_sprite_coord.in");
	//for (int i = 0; i < 19; i++)
		//fin_coord >> sprite_coord[i].x >> sprite_coord[i].y >> sprite_coord[i].w >> sprite_coord[i].h;
	//fin_coord.close();
}
void Bomberman::update(sf::Time deltaTime)
{	
    updateCurrentAnimation(currentAnimation, deltaTime);
	sprite.move(movement*deltaTime.asSeconds());
	position.x = sprite.getPosition().x; 
	position.y = sprite.getPosition().y;
	movement_text.setString("x=" + to_string(position.x) + " y=" + to_string(position.y) + "xd=" + to_string(destination.x) + " yd=" + to_string(destination.y)+"\n");


	if((strcmp(direction, "left")==0 && position.x<=destination.x) || (strcmp(direction, "right") == 0 && position.x>=destination.x)
	    || (strcmp(direction, "up") == 0 && position.y<=destination.y) || (strcmp(direction, "down") == 0 && position.y>=destination.y))
	//if ((int)(position.x) ==(int)(destination.x) && (int)position.y == (int)destination.y)
	
	
	
	{  
	    //position.x = (int)position.x;
	    //position.y = (int)position.y;
	    sprite.setPosition(destination.x, destination.y);
		//if(isMoving == false)
	    isMoving = false;
		//{
		    //sprite.setPosition((int)(position.x), (int)(position.y));
		    if (strcmp(direction, "left") == 0)
			   currentAnimation = idleLeft;
		    else if (strcmp(direction, "right") == 0)
			   currentAnimation = idleRight;
		    else if (strcmp(direction, "up") == 0)
			   currentAnimation = idleUp;
		    else if (strcmp(direction, "down") == 0)
			   currentAnimation = idleDown;
	//	}
	
		//movement_text.setString("Done");
	}
	movement.x = 0;
	movement.y = 0;
	
}

void Bomberman::updateCurrentAnimation(animation &currentAnimation, Time &dt)
{   
    sprite.setTexture(currentAnimation.texture);
   // if (currentAnimation.nFrames == 1)
	//   return;
    currentTime += dt;
    float scaledTime = (currentTime.asSeconds() / currentAnimation.duration.asSeconds());
    int currentFrame = (int)(scaledTime*currentAnimation.nFrames)+currentAnimation.startFrameOffset;

    if (currentAnimation.looped)
    {
	   currentFrame %= currentAnimation.nFrames;
	   currentFrame +=currentAnimation.startFrameOffset;
    }
    else if (currentFrame - currentAnimation.startFrameOffset >= currentAnimation.nFrames)
	   currentFrame = currentAnimation.nFrames - 1 + currentAnimation.startFrameOffset;
    text.setString(to_string(currentFrame));
    sprite.setTextureRect(IntRect(currentFrame * 51, 0, 51, 51));
}
