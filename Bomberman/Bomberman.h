#pragma once
#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <SFML/Graphics.hpp>
#include "Animator.h"

using namespace sf;

class Bomberman
{
private:

public:

	Bomberman(int startX, int startY);
	void update(sf::Time deltaTime);
	

	Vector2i position;
	Vector2i destination;
	Vector2f movement;
	Vector2i sprite_size;
	float speed;
	 Texture  bomberman_up, bomberman_down, bomberman_left, bomberman_right;
	Sprite sprite;
	Time currentTime;

	struct animation
	{
	    Texture texture;
	    int nFrames;
	    bool looped;
	    Time duration;
	    int startFrameOffset=0;
	};
	animation currentAnimation, moveLeft, idleLeft, moveRight, idleRight, moveUp, idleUp, moveDown, idleDown;

	void updateCurrentAnimation(animation &currentAnimation, Time &dt);

	Text movement_text, text;
	Font font;
	bool isMoving;
	char direction[6];
};
#endif 