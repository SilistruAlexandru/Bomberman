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
	void explode(int posX, int posY);
	void explosionsUpdate(sf::Time deltaTime);
	//void drawExplosion(int i);
	void enemy1Update(sf::Time deltaTime);
	void enemy2Update(sf::Time deltaTime);
	void enemy3Update(sf::Time deltaTime);
	void enemyValidDirections(int gridX, int gridY, int v[5]);
	//void enemy2ValidDirections(int gridX, int gridY, int v[5]);
	bool checkIntersection(sf::Sprite a, sf::Sprite b);
	void changeExplosionScale(int i, double x);
	void pickUpDraw();
	void pickUpUpdate(sf::Time deltaTime);
#endif 