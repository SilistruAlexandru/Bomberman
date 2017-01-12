#pragma once
#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "SFML/Graphics.hpp"
//#include "Bomberman.h"
struct playerAnimation;
	void gameLevel();
	void gameLevelUpdate(sf::Time deltaTime);
	void gameLevelRender(sf::RenderWindow &window);
	//void currentPlayerAnimationUpdate(playerAnimation &currentPlayerAnimation, Time &dt);
	void playerCreate(int startX, int startY);
	void playerUpdate(sf::Time deltaTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	
#endif 