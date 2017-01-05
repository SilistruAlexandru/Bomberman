#pragma once
#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.hpp"
#include "Bomberman.h"

class Game
{
public:
	Game();
	void run();
	static const sf::Time TimePerFrame;

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	//static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	
	sf::Texture mTexture;
	sf::Sprite mPlayer;
	Bomberman bomberman;
	
	bool mIsMovingLeft;
	bool mIsMovingRight;
	bool mIsMovingUp;
	bool mIsMovingDown;
	//sf::ContextSettings settings;
	
	
};
#endif 