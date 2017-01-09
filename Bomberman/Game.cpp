#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Bomberman.h"
#include <cstring>
#include "Animator.h"


using namespace std;
using namespace sf;


//const  sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game():
mWindow(sf::VideoMode(640, 480), "Bomberman")
,bomberman(20, 20)
, mIsMovingUp(false)

{
	
	mWindow.setFramerateLimit(60);
//	bomberman_texture.loadFromFile("Resources/Textures/bomberman_down.png");

	

}

void Game::run()
{	
	sf::Clock clock;
	sf::Time deltaTime;
	sf::Time elapsedTime=Time::Zero;
	//sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{	
		deltaTime = clock.restart();
		if(bomberman.isMoving)
		    elapsedTime += deltaTime;
		//if (elapsedTime >= seconds(1))
		//{
		//    bomberman.isMoving = false;
		//    elapsedTime = Time::Zero;
		//}

		processEvents();
	//timeSinceLastUpdate += elapsedTime;
		//timeSinceLastUpdate += clock.restart();
		//while (timeSinceLastUpdate > TimePerFrame)
	//	{
		//	timeSinceLastUpdate -= TimePerFrame;
			//processEvents();
			update(deltaTime);
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
			bomberman.destination.y -= 100;
			strcpy(bomberman.direction, "up");
			bomberman.currentAnimation = bomberman.moveUp;

		}
	}
		
	else if (key == sf::Keyboard::Down)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.y += 100;
			strcpy(bomberman.direction, "down");
			bomberman.currentAnimation = bomberman.moveDown;
		}
	}
	else if (key == sf::Keyboard::Left)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.x -= 100;
			strcpy(bomberman.direction, "left");
			bomberman.currentAnimation = bomberman.moveLeft;
		}
	}
	else if (key == sf::Keyboard::Right)
	{

		if (bomberman.isMoving == false)
		{
			bomberman.isMoving = true;
			bomberman.destination.x += 100;
			strcpy(bomberman.direction, "right");
			bomberman.currentAnimation = bomberman.moveRight;
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
		bomberman.movement.y -= bomberman.speed; 
		bomberman.position.y -= bomberman.speed ;
	}
		//before, bomberman.speed/60
	else if (bomberman.isMoving && strcmp(bomberman.direction, "down") == 0)
	{
		bomberman.movement.y += bomberman.speed;
		bomberman.position.y += bomberman.speed ;
	}
	else if (bomberman.isMoving && strcmp(bomberman.direction, "left") == 0)
	{
		bomberman.movement.x -= bomberman.speed;
		bomberman.position.x -= bomberman.speed ;
	}
	else if (bomberman.isMoving && strcmp(bomberman.direction, "right") == 0)
	{
		bomberman.movement.x += bomberman.speed;
		bomberman.position.x += bomberman.speed ;
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
	sf::sleep(milliseconds(1));
	
}

