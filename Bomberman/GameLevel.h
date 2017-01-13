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
	void placeBomb();
	void bombsUpdate(sf::Time deltaTime);
	bool checkDirectionChange();
	void explode(int gridX, int gridY);
	void explosionsUpdate(sf::Time deltaTime);
	void drawExplosion(int i);
#endif 