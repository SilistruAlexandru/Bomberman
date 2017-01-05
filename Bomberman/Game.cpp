#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Bomberman.h"
#include <cstring>

using namespace std;
using namespace sf;

//const  sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() :
mWindow(sf::VideoMode(640, 480), "Bomberman")
,bomberman(20, 20)
, mIsMovingUp(false)

{
	
	mWindow.setFramerateLimit(60);
	 
	

}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{	
		sf::Time elapsedTime = clock.restart();
		processEvents();
	//timeSinceLastUpdate += elapsedTime;
		//timeSinceLastUpdate += clock.restart();
		//while (timeSinceLastUpdate > TimePerFrame)
	//	{
		//	timeSinceLastUpdate -= TimePerFrame;
			//processEvents();
			update(elapsedTime);
	//	}
		
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type) 
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
			
			
		}
		
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up)
	{
		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.y -= 50.f;
			strcpy(bomberman.direction, "up");

		}
	}
		
	else if (key == sf::Keyboard::Down)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.y += 50.f;
			strcpy(bomberman.direction, "down");
		}
	}
	else if (key == sf::Keyboard::Left)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.x -= 50.f;
			strcpy(bomberman.direction, "left");
		}
	}
	else if (key == sf::Keyboard::Right)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.x += 50.f;
			strcpy(bomberman.direction, "right");
		}
	}
	else if(key==sf::Keyboard::Escape)
		mWindow.close();
}
void Game::update(sf::Time deltaTime)
{
	//mPlayer.move(0.01, 0.01);

	
	if (bomberman.isMoving && strcmp(bomberman.direction, "up") == 0)
	{
		bomberman.movement.y -= 50.f; 
		bomberman.position.y -= 50.f / 60;
	}
		
	else if (bomberman.isMoving && strcmp(bomberman.direction, "down") == 0)
	{
		bomberman.movement.y += 50.f;
		bomberman.position.y += 50.f / 60;
	}
	else if (bomberman.isMoving && strcmp(bomberman.direction, "left") == 0)
	{
		bomberman.movement.x -= 50.f; 
		bomberman.position.x -= 50.f/60;
	}
	else if (bomberman.isMoving && strcmp(bomberman.direction, "right") == 0)
	{
		bomberman.movement.x += 50.f;
		bomberman.position.x += 50.f/60;
	}
	bomberman.update(deltaTime);
	//mPlayer.move(movement*deltaTime.asSeconds());
}

void Game::render()
{
	mWindow.clear(sf::Color::Black);
	mWindow.draw(bomberman.sprite);
	mWindow.draw(bomberman.movement_text);
	mWindow.display();
	//sf::sleep(milliseconds(1));
	
}

