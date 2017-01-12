#include "SFML/Graphics.hpp"
//#include "Bomberman.h"
#include "GameLevel.h"
#include <fstream>

using namespace std;
using namespace sf;

int playerGridX, playerGridY;
Vector2i playerPosition;
Vector2i playerDestination;
Vector2f playerMovement;
int playerSpeed;

Texture  bomberman_up, bomberman_down, bomberman_left, bomberman_right;
Sprite playerSprite;
RenderWindow window;
View view, currentView;
Texture background_texture, block_texture, destroyableBlock_texture;
Sprite backgroundSprite, blocksSprites[100], destroyableBlocksSprites[100];
int nrBlocks = 0, nrDestroyableBlocks = 0;

int playerStartX, playerStartY;
char grid[100][100];
int gridWidth, gridHeight;

bool mIsMovingLeft;
bool mIsMovingRight;
bool mIsMovingUp;
bool mIsMovingDown;

bool leftKey, rightKey, upKey, downKey;

struct playerAnimation
{
    Texture texture;
    int nFrames;
    bool looped;
    Time duration;
    int startFrameOffset = 0;
};

playerAnimation currentPlayerAnimation, playerMoveLeft, playerIdleLeft, playerMoveRight, playerIdleRight, playerMoveUp, playerIdleUp, playerMoveDown, playerIdleDown;


bool isPlayerMoving;
char playerDirection[6];

Time currentTime;
Text movement_text, text;
Font font;
//Functia principala
void gameLevel()
{
	  
    
    RenderWindow window(sf::VideoMode(1000, 500), "Bomberman");
	  // ,mIsMovingUp(false)  
	   window.setFramerateLimit(60);
	   //playerSprite.setOrigin(50 / 2., 50 / 2.);
	   View view(FloatRect(0, 0, window.getSize().x, window.getSize().y));
	   view.setCenter(playerSprite.getOrigin());
	   //window.setView(view);
	   background_texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/Tiles.png");
	   block_texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/block.PNG");
	   destroyableBlock_texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/destroyable_block.PNG");

	   backgroundSprite.setTexture(background_texture);
	   backgroundSprite.setScale((float)50 / 56, (float)50 / 56);
	   leftKey = rightKey = upKey = downKey = false;
	   ifstream fin("grid.in");
	   ofstream fout("debug.out");
	   fin >> gridWidth >> gridHeight;
	   for (int i = 0; i < 100; i++)
	   {
		  blocksSprites[i].setTexture(block_texture);
		  blocksSprites[i].setScale(50 / 59., 50 / 59.);
		  destroyableBlocksSprites[i].setTexture(destroyableBlock_texture);
		  destroyableBlocksSprites[i].setScale(50 / 76., 50 / 76.);
	   }
	   for (int i = 0; i < gridWidth; i++)
		  for (int j = 0; j < gridHeight; j++)
		  {
			 fin >> grid[i][j];
			 if (grid[i][j] == 'A')
			 {	 
				playerStartX = j * 50;
				playerStartY = i * 50;
				playerGridX = j;
				playerGridY = i;
			 }
			 else if (grid[i][j] == 'B')
				blocksSprites[nrBlocks++].setPosition(j * 50, i * 50);
			 else if(grid[i][j]=='D')
				destroyableBlocksSprites[nrDestroyableBlocks++].setPosition(j * 50, i * 50);
		  }
	   fin.close();
	   playerCreate(playerStartX, playerStartY);

    // Game::run()
  	
	   sf::Clock clock;
	   sf::Time deltaTime;
	   sf::Time elapsedTime=Time::Zero;
	   //sf::Time timeSinceLastUpdate = sf::Time::Zero;
	   while (window.isOpen())
	   {	
		  deltaTime = clock.restart();
		  if(isPlayerMoving)
			 elapsedTime += deltaTime;


		  //processEvents();
		  sf::Event event;
		  while (window.pollEvent(event))
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
				window.close();
				break;
			 }
		  }

//Update 
		  gameLevelUpdate(deltaTime);
		  view.setCenter(playerPosition.x, playerPosition.y);
		  window.setView(view);
		  //mPlayer.move(movement*deltaTime.asSeconds());
		  gameLevelRender(window);
	   }
    }
    //const  sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
    

//Functions

    void gameLevelRender(RenderWindow &window)
    {
	   window.clear(sf::Color::Black);
	   window.draw(backgroundSprite);
	   for (int i = 0; i < 100; i++)
	   {
		  window.draw(blocksSprites[i]);
		  window.draw(destroyableBlocksSprites[i]);
	   }
	   window.draw(playerSprite);
	  window.draw(movement_text);
	   window.display();
	   sf::sleep(milliseconds(1));

    }
    

void playerCreate(int startX, int startY)
    {
	   playerPosition.x = playerStartX;
	   playerPosition.y = playerStartY;
	   playerDestination.x = playerStartX;
	   playerDestination.y = playerStartY;
	   playerMovement.x = 0;
	   playerMovement.y = 0;
	   playerSpeed = 200;
	   isPlayerMoving = false;
	   currentTime = Time::Zero;
	   playerSprite.setScale(50 / 51., 50 / 51.);


	   // bomberman_down.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");

	   playerMoveLeft.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_left.png");
	   playerMoveLeft.looped = true;
	   playerMoveLeft.nFrames = 3;
	   playerMoveLeft.duration = seconds(0.5);
	   //playerMoveLeft.startFrameOffset = 1;

	   playerIdleLeft.texture = playerMoveLeft.texture;
	   playerIdleLeft.looped = false;
	   playerIdleLeft.nFrames = 1;
	   playerIdleLeft.duration = seconds(1);

	   playerMoveRight.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_right.png");
	   playerMoveRight.looped = true;
	   playerMoveRight.nFrames = 3;
	   playerMoveRight.duration = seconds(0.5);
	   //playerMoveRight.startFrameOffset = 0;

	   playerIdleRight.texture = playerMoveRight.texture;
	   playerIdleRight.looped = false;
	   playerIdleRight.nFrames = 1;
	   playerIdleRight.duration = seconds(1);

	   playerMoveUp.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_up.png");
	   playerMoveUp.looped = true;
	   playerMoveUp.nFrames = 3;
	   playerMoveUp.duration = seconds(0.5);
	   //playerMoveUp.startFrameOffset = 1;

	   playerIdleUp.texture = playerMoveUp.texture;
	   playerIdleUp.looped = false;
	   playerIdleUp.nFrames = 1;
	   playerIdleUp.duration = seconds(1);

	   playerMoveDown.texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");
	   //playerMoveDown.texture = AssetManager::GetTexture("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_down.png");
	   playerMoveDown.looped = true;
	   playerMoveDown.nFrames = 3;
	   playerMoveDown.duration = seconds(0.5);
	   //playerMoveDown.startFrameOffset = 1;

	   playerIdleDown.texture = playerMoveDown.texture;
	   playerIdleDown.looped = false;
	   playerIdleDown.nFrames = 1;
	   playerIdleDown.duration = seconds(1);

	   strcpy(playerDirection, "down");
	   currentPlayerAnimation = playerIdleDown;

	   //currentPlayerAnimation.texture = playerMoveLeft.texture;
	   //texture.setSmooth(true);
	   //texture.loadFromFile("C:/Users/Alex/Desktop/IP Bomberman/Bomberman/Bomberman/Resources/Textures/bomberman_right.png");

	   //playerSprite.setTexture(bomberman_down);
	   //playerSprite.setTextureRect(IntRect(3, 3, 51, 51));
	   playerSprite.setPosition(playerPosition.x, playerPosition.y);
	   font.loadFromFile("Resources/Fonts/calibri.ttf");
	   movement_text.setFont(font);
    }

void playerUpdate(Time deltaTime)
{
    playerSprite.setTexture(currentPlayerAnimation.texture);
    // if (currentPlayerAnimation.nFrames == 1)
    //   return;
    currentTime += deltaTime;
    float scaledTime = (currentTime.asSeconds() / currentPlayerAnimation.duration.asSeconds());
    int currentFrame = (int)(scaledTime*currentPlayerAnimation.nFrames) + currentPlayerAnimation.startFrameOffset;

    if (currentPlayerAnimation.looped)
    {
	   currentFrame %= currentPlayerAnimation.nFrames;
	   currentFrame += currentPlayerAnimation.startFrameOffset;
    }
    else if (currentFrame - currentPlayerAnimation.startFrameOffset >= currentPlayerAnimation.nFrames)
	   currentFrame = currentPlayerAnimation.nFrames - 1 + currentPlayerAnimation.startFrameOffset;
    text.setString(to_string(currentFrame));
    playerSprite.setTextureRect(IntRect(currentFrame * 51, 0, 51, 51));
    //end of animation update
    if (isPlayerMoving)
	   playerSprite.move(playerMovement*deltaTime.asSeconds());
    movement_text.move(playerMovement*deltaTime.asSeconds());
    playerPosition.x = playerSprite.getPosition().x;
    playerPosition.y = playerSprite.getPosition().y;
    

    if (isPlayerMoving)
	   if ((strcmp(playerDirection, "left") == 0 && playerPosition.x <= playerDestination.x) || (strcmp(playerDirection, "right") == 0 && playerPosition.x >= playerDestination.x)
		  || (strcmp(playerDirection, "up") == 0 && playerPosition.y <= playerDestination.y) || (strcmp(playerDirection, "down") == 0 && playerPosition.y >= playerDestination.y))
		  //if ((int)(position.x) ==(int)(playerDestination.x) && (int)position.y == (int)playerDestination.y)
	   {
		  //position.x = (int)position.x;
		  //position.y = (int)position.y;
		  playerSprite.setPosition(playerDestination.x, playerDestination.y);

		  isPlayerMoving = false;
		  //{
		  //playerSprite.setPosition((int)(position.x), (int)(position.y));
		  if (strcmp(playerDirection, "left") == 0)
		  {
			 currentPlayerAnimation = playerIdleLeft;
			 playerGridX--;
		  }
		  else if (strcmp(playerDirection, "right") == 0)
		  {
			 currentPlayerAnimation = playerIdleRight;
			 playerGridX++;
		  }
		  else if (strcmp(playerDirection, "up") == 0)
		  {
			 currentPlayerAnimation = playerIdleUp;
			 playerGridY--;
		  }
		  else if (strcmp(playerDirection, "down") == 0)
		  {
			 currentPlayerAnimation = playerIdleDown;
			 playerGridY++;
		  }
		  //	}

		  //playerplayerMovement_text.setString("Done");
	   }
    playerMovement.x = 0;
    playerMovement.y = 0;
    movement_text.setString("x=" + to_string(playerPosition.x) + " y=" + to_string(playerPosition.y) + "xd=" + to_string(playerDestination.x) + " yd=" + to_string(playerDestination.y) + "\n"
	   + to_string(playerGridY) + " " + to_string(playerGridX) + "\n");
    
    //currentView = window.getView();
    
}

void gameLevelUpdate(Time deltaTime)
{
    if (isPlayerMoving && strcmp(playerDirection, "up") == 0)
    {
	   playerMovement.y -= playerSpeed;
	   playerPosition.y -= playerSpeed;
    }
    //before, bomberman->speed/60
    else if (isPlayerMoving && strcmp(playerDirection, "down") == 0)
    {
	   playerMovement.y += playerSpeed;
	   playerPosition.y += playerSpeed;
    }
    else if (isPlayerMoving && strcmp(playerDirection, "left") == 0)
    {
	   playerMovement.x -= playerSpeed;
	   playerPosition.x -= playerSpeed;
    }
    else if (isPlayerMoving && strcmp(playerDirection, "right") == 0)
    {
	   playerMovement.x += playerSpeed;
	   playerPosition.x += playerSpeed;
    }
    //Player update
    playerUpdate(deltaTime);
    
}
void handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::Up)
    {
	   // if (isPressed)
	   //  leftKey = true;
	   // else leftKey = false;
	   if (isPlayerMoving == false && grid[playerGridY-1][playerGridX]!='B' &&grid[playerGridY - 1][playerGridX] != 'D')
	   {
		  isPlayerMoving = true;
		  playerDestination.y -= 50;
		  //if(strcmp(bomberman->direction, "up")!=0)
		  //{
		  strcpy(playerDirection, "up");
		  currentPlayerAnimation = playerMoveUp;
	   }
    }
    else if (key == sf::Keyboard::Down)
    {

	   if (isPlayerMoving == false && grid[playerGridY + 1][playerGridX] != 'B' && grid[playerGridY + 1][playerGridX] != 'D')
	   {
		  isPlayerMoving = true;
		  playerDestination.y += 50;
		  //	if (strcmp(bomberman->direction, "down") != 0)
		  //	{
		  strcpy(playerDirection, "down");
		  currentPlayerAnimation = playerMoveDown;
		  //	}
	   }
    }
    else if (key == sf::Keyboard::Left)
    {

	   if (isPlayerMoving == false && grid[playerGridY][playerGridX-1] != 'B' &&grid[playerGridY][playerGridX-1] != 'D')
	   {
		  isPlayerMoving = true;
		  playerDestination.x -= 50;
		  //if (strcmp(bomberman->direction, "left") != 0)
		  //{
		  strcpy(playerDirection, "left");
		  currentPlayerAnimation = playerMoveLeft;
		  //}
	   }
    }
    else if (key == sf::Keyboard::Right)
    {

	   if (isPlayerMoving == false && grid[playerGridY][playerGridX + 1] != 'B' &&grid[playerGridY][playerGridX + 1] != 'D')
	   {
		  isPlayerMoving = true;
		  playerDestination.x += 50;
		  //	if (strcmp(bomberman->direction, "right") != 0)
		  //	{
		  strcpy(playerDirection, "right");
		  currentPlayerAnimation = playerMoveRight;
		  //}
	   }
    }
    //else if (key == sf::Keyboard::Escape)
    // window.close();
}
