#include "SFML/Graphics.hpp"
#include "GameLevel.h"
#include <fstream>
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

int playerGridX, playerGridY;
Vector2i playerPosition;
Vector2i playerDestination;
Vector2f playerMovement;
int playerSpeed;
Time playerDyingTime = seconds(2);
Time currentPlayerDyingTime = Time::Zero;
int lives = 5, score = 0;
Time levelTime = seconds(500);
int menu = 1;

Sprite s1, s2;

struct ui
{
    Sprite lives, bombs, explosion, speed, gems, enemy1, enemy2, enemy3, bar;
    Text status, end;
    Text score;
    Sprite title, pause, win, lose;
}UI;

Texture bar_texture;
Texture title_texture, pause_texture, win_texture, lose_texture;
SoundBuffer bomb_buffer, explosion_buffer, pickup_buffer, enemy_buffer, playerDie_buffer, playerReset_buffer, win_buffer, lose_buffer;
Sound bomb, explosion, pickup, enemy, playerDie, playerReset, win, lose;

int validDirEnemy1[5], validDirEnemy2[5], validDirEnemy3[5];
int enemy1Speed = 80, enemy2Speed = 90, enemy3Speed = 100;

struct enemy1
{
    Sprite sprite;
    bool alive = false;
    bool active = false;
    int gridX, gridY;
    int dirX = 1, dirY = 0;
    Vector2i destination;
    Time currentTime = Time::Zero;
    Time dyingTime= Time::Zero;
    int direction;
    bool spriteFlip = false;
    bool isMoving = false;
}enemy1[10];


struct enemy2
{
    Sprite sprite;
    bool alive = false;
    bool active = false;
    int gridX, gridY;
    int dirX = 1, dirY = 0;
    Vector2i destination;
    Time currentTime = Time::Zero;
    Time dyingTime = Time::Zero;
    int direction;
    bool spriteFlip = false;
    bool isMoving = false;
}enemy2[10];

struct enemy3
{
    Sprite sprite;
    bool alive = false;
    bool active = false;
    int gridX, gridY;
    int dirX = 1, dirY = 0;
    Vector2i destination;
    Time currentTime = Time::Zero;
    Time dyingTime = Time::Zero;
    Time randomWaitingTime= Time::Zero;
    Time currentWaitingTime= Time::Zero;
    int direction;
    bool spriteFlip = false;
    bool isMoving = false;
}enemy3[10];



Texture  bomberman_up, bomberman_down, bomberman_left, bomberman_right, bomberman_destroy;
Texture bomb_texture;
Sprite playerSprite;
bool isPlayerAlive = true;
RenderWindow window;
int windowWidth=1200, windowHeight=600;


View view, currentView;
Texture background_texture, block_texture, destroyableBlock_texture;
Texture enemy1_texture, enemy2_texture, enemy3_texture;
Sprite backgroundSprite, blocksSprites[1000], destroyableBlocksSprites[1000];
int nrBlocks = 0, nrDestroyableBlocks = 0;
int nrEnemy1 = 0, nrEnemy2 = 0, nrEnemy3 = 0;
int countEnemy1 = 0, countEnemy2 = 0, countEnemy3 = 0, countGems = 0;
int playerStartX, playerStartY;
char grid[100][100];
bool hasPickUp[100][100];
int gridWidth, gridHeight;
int dirX=0, dirY=0;

bool mIsMovingLeft;
bool mIsMovingRight;
bool mIsMovingUp;
bool mIsMovingDown;

bool leftKey, rightKey, upKey, downKey;

struct Animation
{
    Texture texture;
    int nFrames;
    bool looped;
    Time duration;
    int startFrameOffset = 0;
};

Animation currentPlayerAnimation, playerMoveLeft, playerIdleLeft, playerMoveRight, playerIdleRight, playerMoveUp, playerIdleUp, playerMoveDown, playerIdleDown, playerDying;


bool isPlayerMoving;
char playerDirection[6];

Time currentTime;
Text movement_text, text, debug;
Font font;

bool isDestroyableBlockActive[1000];
int bombLimit = 1, bombCount = 0;
Time bombDuration = seconds(3);
struct bomb
{
    Sprite sprite;
    int gridX, gridY;
    bool active=false;
    Time currentTime=Time::Zero;
    bool forcedExplode = false;

}bombs[100];

struct explosion
{
    Sprite center;
    Sprite endUp, endDown, endLeft, endRight;
    bool isEndUp=true, isEndDown = true, isEndLeft = true, isEndRight = true;
    Sprite innerUp[8], innerDown[8], innerLeft[8], innerRight[8];
    int innerUpCount=0, innerDownCount=0, innerLeftCount=0, innerRightCount=0;
    bool active = false;
    Time currentTime = Time::Zero;
    Texture currentTexture;
}explosions[100];

Texture explosion1_texture, explosion2_texture, explosion3_texture, explosion4_texture;
int explosionLength = 1;
Time explosionDuration = seconds(1);

Texture gem_texture, bombPowerUp_texture, explosionPowerUp_texture, speedPowerUp_texture;

struct pickUp
{
    int gridX, gridY;
    bool active = true;
    Sprite sprite;
}gem[50], bombPowerUp[10], explosionPowerUp[10], speedPowerUp[10];

//Functia principala
void gameLevel()
{
    UI.status.setScale(1.2, 1.2);
  
    title_texture.loadFromFile("Resources/Textures/title.png");
    pause_texture.loadFromFile("Resources/Textures/pause.png");
    win_texture.loadFromFile("Resources/Textures/win.png");
    lose_texture.loadFromFile("Resources/Textures/lose.png");

    UI.title.setTexture(title_texture);
    UI.pause.setTexture(pause_texture);
    UI.win.setTexture(win_texture);
    UI.lose.setTexture(lose_texture);

    UI.title.setPosition(0, 0);
    UI.pause.setPosition(0, 0);
    UI.win.setPosition(0, 0);
    UI.lose.setPosition(0, 0);

    UI.score.setFont(font);
    UI.score.setPosition(510, 250);

    gem_texture.loadFromFile("Resources/Textures/gem.png");
    gem_texture.setSmooth(true);
    bombPowerUp_texture.loadFromFile("Resources/Textures/bomb_powerup.PNG");
    bombPowerUp_texture.setSmooth(true);
    explosionPowerUp_texture.loadFromFile("Resources/Textures/flame_powerup.PNG");
    explosionPowerUp_texture.setSmooth(true);
    speedPowerUp_texture.loadFromFile("Resources/Textures/speed_powerup.PNG");
    speedPowerUp_texture.setSmooth(true);

    bar_texture.loadFromFile("Resources/Textures/bar.png");
    bar_texture.setRepeated(false);
   
   

    bomb_buffer.loadFromFile("Resources/Sounds/BombDrop.wav");
    explosion_buffer.loadFromFile("Resources/Sounds/Explosion.wav");
    pickup_buffer.loadFromFile("Resources/Sounds/Pickup.wav");
    enemy_buffer.loadFromFile("Resources/Sounds/Enemy.wav");
    playerDie_buffer.loadFromFile("Resources/Sounds/lose.wav");
    playerReset_buffer.loadFromFile("Resources/Sounds/lose2.wav");
    lose_buffer.loadFromFile("Resources/Sounds/lose3.wav");
    win_buffer.loadFromFile("Resources/Sounds/win.wav");
    bomb.setBuffer(bomb_buffer);
    explosion.setBuffer(explosion_buffer);
    pickup.setBuffer(pickup_buffer);
    enemy.setBuffer(enemy_buffer);
    playerDie.setBuffer(playerDie_buffer);
    playerReset.setBuffer(playerReset_buffer);
    lose.setBuffer(lose_buffer);
    win.setBuffer(win_buffer);

    bomberman_up.loadFromFile("Resources/Textures/bomberman_up.png");
    bomberman_down.loadFromFile("Resources/Textures/bomberman_down.png");
    bomberman_left.loadFromFile("Resources/Textures/bomberman_left.png");
    bomberman_right.loadFromFile("Resources/Textures/bomberman_right.png");
    bomberman_destroy.loadFromFile("Resources/Textures/bomberman_destroy.png");

    UI.bar.setTexture(bar_texture);
    UI.bar.setScale(0.5, 0.5);
    UI.bar.setTextureRect(IntRect(200, 200, windowWidth*3, 200));

    enemy1_texture.setSmooth(true);
    enemy2_texture.setSmooth(true);
    enemy3_texture.setSmooth(true);
    bomberman_left.setSmooth(true);
    bomberman_right.setSmooth(true);
    bomberman_up.setSmooth(true);
    bomberman_down.setSmooth(true);
    bomberman_left.setSmooth(true);
    bomberman_right.setSmooth(true);
    bomberman_destroy.setSmooth(true);
    bomb_texture.setSmooth(true);
		 font.loadFromFile("Resources/Fonts/calibri.ttf");
		 debug.setFont(font);
		

		 debug.setPosition(200, 200);
		 RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bomberman");
	  // ,mIsMovingUp(false)  
	   window.setFramerateLimit(60);
	   //playerSprite.setOrigin(50 / 2., 50 / 2.);
	   View view(FloatRect(0, 0, window.getSize().x*1.2, window.getSize().y*1.2));
	   view.setCenter(window.getSize().x*1.2/2, window.getSize().y*1.2/2);
	   //window.setView(view);
	   background_texture.loadFromFile("Resources/Textures/Tiles.png");
	   background_texture.setRepeated(true);
	   block_texture.loadFromFile("Resources/Textures/block.PNG");
	   destroyableBlock_texture.loadFromFile("Resources/Textures/destroyable_block.PNG");
	   bomb_texture.loadFromFile("Resources/Textures/bomb.png");
	   explosion1_texture.loadFromFile("Resources/Textures/explosion1.png");
	   explosion2_texture.loadFromFile("Resources/Textures/explosion2.png");
	   explosion3_texture.loadFromFile("Resources/Textures/explosion3.png");
	   explosion4_texture.loadFromFile("Resources/Textures/explosion4.png");
	   enemy1_texture.loadFromFile("Resources/Textures/enemy1.png");
	   enemy2_texture.loadFromFile("Resources/Textures/enemy2.png");
	   enemy3_texture.loadFromFile("Resources/Textures/enemy3.png");

	   backgroundSprite.setTexture(background_texture);
	   backgroundSprite.setScale(50 / 56., 50 / 56.);
	   backgroundSprite.setTextureRect(IntRect(0, 0, 20000, 20000));
	   backgroundSprite.setPosition(-2000, -2000);
	   
	   UI.status.setFont(font);
	   leftKey = rightKey = upKey = downKey = false;

	   ifstream fin("grid.in");
	   ofstream fout("debug.out");
	   fin >> gridWidth >> gridHeight;
	   for (int i = 0; i < 1000; i++)
		  isDestroyableBlockActive[i] = false;
	   for (int i = 0; i < 1000; i++)
	   {
		  blocksSprites[i].setTexture(block_texture);
		  blocksSprites[i].setScale(50 / 59., 50 / 59.);
		  destroyableBlocksSprites[i].setTexture(destroyableBlock_texture);
		  destroyableBlocksSprites[i].setScale(50 / 76., 50 / 76.);
	   }
	   for (int i = 0; i < 100; i++)
	   {
		  bombs[i].sprite.setTexture(bomb_texture);
		  bombs[i].sprite.setScale(50 / 82., 50 / 82.);
		  bombs[i].active = false;
	   }
	   for (int i = 0; i < 100; i++)
	   {
		  explosions[i].center.setTexture(explosion1_texture);
		  explosions[i].center.setScale(50 / 78., 50 / 78.);
		  for(int j=0; j< 8; j++)
		  {
			 explosions[i].innerUp[j].setTexture(explosion1_texture);
			 explosions[i].innerUp[j].setScale(50 / 78., 50 / 78.);
		  }
		  for (int j = 0; j< 8; j++)
		  {
			 explosions[i].innerDown[j].setTexture(explosion1_texture);
			 explosions[i].innerDown[j].setScale(50 / 78., 50 / 78.);
		  }
		  for (int j = 0; j< 8; j++)
		  {
			 explosions[i].innerLeft[j].setTexture(explosion1_texture);
			 explosions[i].innerLeft[j].setScale(50 / 78., 50 / 78.);
		  }
		  for (int j = 0; j<8; j++)
		  {
			 explosions[i].innerRight[j].setTexture(explosion1_texture);
			 explosions[i].innerRight[j].setScale(50 / 78., 50 / 78.);
		  }
		  explosions[i].endUp.setTexture(explosion1_texture);
		  explosions[i].endUp.setScale(50 / 78., 50 / 78.);
		  explosions[i].endDown.setTexture(explosion1_texture);
		  explosions[i].endDown.setScale(50 / 78., 50 / 78.);
		  explosions[i].endLeft.setTexture(explosion1_texture);
		  explosions[i].endLeft.setScale(50 / 78., 50 / 78.);
		  explosions[i].endRight.setTexture(explosion1_texture);
		  explosions[i].endRight.setScale(50 / 78., 50 / 78.);
		  bombs[i].active = false;
	   }
	   for (int i = 0; i < gridHeight; i++)
		  for (int j = 0; j < gridWidth; j++)
		  {
			 fin >> grid[i][j];
			 hasPickUp[i][j] = false;
			 if (grid[i][j] == 'A')
			 {	 
				playerStartX = j * 50;
				playerStartY = i * 50;
				playerGridX = j;
				playerGridY = i;
				grid[i][j] = '0';
				
			 }
			 else if (grid[i][j] == 'U')
				blocksSprites[nrBlocks++].setPosition(j * 50, i * 50);
			 else if(grid[i][j]=='D')
			 {
				destroyableBlocksSprites[nrDestroyableBlocks].setPosition(j * 50, i * 50);
				isDestroyableBlockActive[nrDestroyableBlocks++] = true;
			 }
			 else if (grid[i][j] == '1')
			 {
				grid[i][j] = '0';
				enemy1[nrEnemy1].alive = true;
				enemy1[nrEnemy1].active = true;
				enemy1[nrEnemy1].gridX = j;
				enemy1[nrEnemy1].gridY = i;			
				enemy1[nrEnemy1].sprite.setTexture(enemy1_texture);
				enemy1[nrEnemy1].sprite.setPosition(j * 50, i * 50);
				enemy1[nrEnemy1].sprite.setScale(50 / 31., 50 / 31.);
				enemy1[nrEnemy1].destination.x = j * 50;
				enemy1[nrEnemy1++].destination.y = i * 50;
			 }
			 else if (grid[i][j] == '2')
			 {
				grid[i][j] = '0';
				enemy2[nrEnemy2].alive = true;
				enemy2[nrEnemy2].active = true;
				enemy2[nrEnemy2].gridX = j;
				enemy2[nrEnemy2].gridY = i;
				enemy2[nrEnemy2].sprite.setTexture(enemy2_texture);
				enemy2[nrEnemy2].sprite.setPosition(j * 50, i * 50);
				enemy2[nrEnemy2].sprite.setScale(50 / 24., 50 / 24.);
				enemy2[nrEnemy2].destination.x = j * 50;
				enemy2[nrEnemy2++].destination.y = i * 50;
			 }
			 else if (grid[i][j] == '3')
			 {
				grid[i][j] = '0';
				enemy3[nrEnemy3].alive = true;
				enemy3[nrEnemy3].active = true;
				enemy3[nrEnemy3].gridX = j;
				enemy3[nrEnemy3].gridY = i;
				enemy3[nrEnemy3].sprite.setTexture(enemy3_texture);
				enemy3[nrEnemy3].sprite.setPosition(j * 50, i * 50);
				enemy3[nrEnemy3].sprite.setScale(50 / 25., 50 / 25.);
				enemy3[nrEnemy3].destination.x = j * 50;
				enemy3[nrEnemy3++].destination.y = i * 50;
			 }
		  }
	   fin.close();
	   playerCreate(playerStartX, playerStartY);
	   int pX, pY;
	   for (int i = 0; i < 20; i++)
	   {	  if(i<10)
		  do
		  {
			 pX = rand() % gridWidth + 1;
			 pY= rand() % gridHeight + 1;
		  } while (grid[pY][pX]!='0' || hasPickUp[pY][pX]==true);
	   else do
		  {
			 pX = rand() % gridWidth + 1;
			 pY = rand() % gridHeight + 1;
		  } while (grid[pY][pX] != 'D' || hasPickUp[pY][pX] == true);
		  gem[i].gridX = pX;
		  gem[i].gridY = pY;
		  hasPickUp[pY][pX] = true;
		  gem[i].sprite.setTexture(gem_texture);
		  gem[i].sprite.setScale(50 / 78., 50 / 78.);
		  gem[i].sprite.setPosition(pX * 50, pY * 50);

	   }
	   for (int i = 0; i < 10; i++)
	   {
		  do
		  {
			 pX = rand() % gridWidth + 1;
			 pY = rand() % gridHeight + 1;
		  } 
		  while (grid[pY][pX] != 'D' || hasPickUp[pY][pX] == true);
		  bombPowerUp[i].gridX = pX;
		  bombPowerUp[i].gridY = pY;
		  hasPickUp[pY][pX] = true;
		  bombPowerUp[i].sprite.setTexture(bombPowerUp_texture);
		  bombPowerUp[i].sprite.setScale(50 / 100., 50 / 100.);
		  bombPowerUp[i].sprite.setPosition(pX * 50, pY * 50);
	   }
	   for (int i = 0; i < 10; i++)
	   {
		  do
		  {
			 pX = rand() % gridWidth + 1;
			 pY = rand() % gridHeight + 1;
		  } while (grid[pY][pX] != 'D' || hasPickUp[pY][pX] == true);
		  explosionPowerUp[i].gridX = pX;
		  explosionPowerUp[i].gridY = pY;
		  hasPickUp[pY][pX] = true;
		  explosionPowerUp[i].sprite.setTexture(explosionPowerUp_texture);
		  explosionPowerUp[i].sprite.setScale(50 / 100., 50 / 100.);
		  explosionPowerUp[i].sprite.setPosition(pX * 50, pY * 50);
	   }
	   for (int i = 0; i < 1; i++)
	   {
		  do
		  {
			 pX = rand() % gridWidth + 1;
			 pY = rand() % gridHeight + 1;
		  } while (grid[pY][pX] != 'D' || hasPickUp[pY][pX] == true);
		  speedPowerUp[i].gridX = pX;
		  speedPowerUp[i].gridY = pY;
		  hasPickUp[pY][pX] = true;
		  speedPowerUp[i].sprite.setTexture(speedPowerUp_texture);
		  speedPowerUp[i].sprite.setScale(50 / 100., 50 / 100.);
		  speedPowerUp[i].sprite.setPosition(pX * 50, pY * 50);
	   }
	   UI.bar.setPosition(playerStartX - 600, playerStartY - 650);
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
		  if(menu==0)
		  {
			 view.setCenter(playerPosition.x, playerPosition.y);
			 window.setView(view);		  
		  }
		  else
		  {
			 window.setView(window.getDefaultView());
		  }
		  gameLevelRender(window);
	   }
    }

//Functions

    void gameLevelRender(RenderWindow &window)
    {
	   window.clear(sf::Color::Black);
	   if(menu==0)
	   {
		  window.draw(backgroundSprite);
		  for (int i = 0; i < 20; i++)
			 if (gem[i].active)
				window.draw(gem[i].sprite);
		  for (int i = 0; i < 10; i++)
			 if (bombPowerUp[i].active)
				window.draw(bombPowerUp[i].sprite);

		  for (int i = 0; i < 10; i++)
			 if (speedPowerUp[i].active)
				window.draw(speedPowerUp[i].sprite);

		  for (int i = 0; i < 10; i++)
			 if (explosionPowerUp[i].active)
				window.draw(explosionPowerUp[i].sprite);

		  pickUpDraw();

		  for (int i = 0; i < 100; i++)
			 if (explosions[i].active)
				//drawExplosion(i);
			 {
				window.draw(explosions[i].center);
				for (int j = 0; j<explosions[i].innerUpCount; j++)
				    window.draw(explosions[i].innerUp[j]);
				for (int j = 0; j<explosions[i].innerDownCount; j++)
				    window.draw(explosions[i].innerDown[j]);
				for (int j = 0; j<explosions[i].innerLeftCount; j++)
				    window.draw(explosions[i].innerLeft[j]);
				for (int j = 0; j<explosions[i].innerRightCount; j++)
				    window.draw(explosions[i].innerRight[j]);

				if (explosions[i].isEndUp) window.draw(explosions[i].endUp);
				if (explosions[i].isEndDown)	 window.draw(explosions[i].endDown);
				if (explosions[i].isEndLeft)	 window.draw(explosions[i].endLeft);
				if (explosions[i].isEndRight) window.draw(explosions[i].endRight);

			 }
		  for (int i = 0; i < nrBlocks; i++)
		  {
			 window.draw(blocksSprites[i]);
		  } 
		  for (int i = 0; i < nrDestroyableBlocks; i++)
		  {
			 if(isDestroyableBlockActive[i])
				window.draw(destroyableBlocksSprites[i]);
		  }

		  for (int i = 0; i < 100; i++)
			 if (bombs[i].active)
				window.draw(bombs[i].sprite);


		  window.draw(playerSprite);
		  for (int i = 0; i < 10; i++)
			 if (enemy1[i].active)
				window.draw(enemy1[i].sprite);
		  for (int i = 0; i < 10; i++)
			 if (enemy2[i].active)
				window.draw(enemy2[i].sprite);
		  for (int i = 0; i < 10; i++)
			 if (enemy3[i].active)
				window.draw(enemy3[i].sprite);
		  //window.draw(movement_text);
		  //  window.draw(debug);
		  window.draw(UI.bar);
		  window.draw(UI.status);
	   }
	   else 
	   {
		  if (menu == 1)
			 window.draw(UI.title);
		  else if(menu==2)
			 window.draw(UI.pause);
		  else if (menu == 3)
		  {
			 window.draw(UI.win);
			 window.draw(UI.score);
		  }
		  else if (menu == 4)
		  {
			 window.draw(UI.lose);
			 window.draw(UI.score);
		  }
	   }
	   window.display();
	  // sf::sleep(milliseconds(100));

    }
void playerCreate(int startX, int startY)
    {
	   playerPosition.x = playerStartX;
	   playerPosition.y = playerStartY;
	   playerDestination.x = playerStartX;
	   playerDestination.y = playerStartY;
	   playerMovement.x = 0;
	   playerMovement.y = 0;
	   playerSpeed = 120;
	   isPlayerMoving = false;
	   currentTime = Time::Zero;
	   playerSprite.setScale(48 / 55., 48 / 55.);

	   playerMoveLeft.texture=bomberman_left;
	   playerMoveLeft.looped = true;
	   playerMoveLeft.nFrames = 3;
	   playerMoveLeft.duration = seconds(0.5);

	   playerIdleLeft.texture = playerMoveLeft.texture;
	   playerIdleLeft.looped = false;
	   playerIdleLeft.nFrames = 1;
	   playerIdleLeft.duration = seconds(1);

	   playerMoveRight.texture= bomberman_right;
	   playerMoveRight.looped = true;
	   playerMoveRight.nFrames = 3;
	   playerMoveRight.duration = seconds(0.5);

	   playerIdleRight.texture = playerMoveRight.texture;
	   playerIdleRight.looped = false;
	   playerIdleRight.nFrames = 1;
	   playerIdleRight.duration = seconds(1);

	   playerMoveUp.texture= bomberman_up;
	   playerMoveUp.looped = true;
	   playerMoveUp.nFrames = 3;
	   playerMoveUp.duration = seconds(0.5);

	   playerIdleUp.texture = playerMoveUp.texture;
	   playerIdleUp.looped = false;
	   playerIdleUp.nFrames = 1;
	   playerIdleUp.duration = seconds(1);

	   playerMoveDown.texture= bomberman_down;
	   playerMoveDown.looped = true;
	   playerMoveDown.nFrames = 3;
	   playerMoveDown.duration = seconds(0.5);

	   playerIdleDown.texture = playerMoveDown.texture;
	   playerIdleDown.looped = false;
	   playerIdleDown.nFrames = 1;
	   playerIdleDown.duration = seconds(1);

	   playerDying.texture=bomberman_destroy;
	   playerDying.looped = false;
	   playerDying.nFrames = 7;
	   playerDying.duration = seconds(2.5);


	   strcpy(playerDirection, "down");
	   currentPlayerAnimation = playerIdleDown;

	   playerSprite.setPosition(playerPosition.x, playerPosition.y);
	   font.loadFromFile("Resources/Fonts/calibri.ttf");
	   movement_text.setFont(font);
    }

void playerUpdate(Time deltaTime)
{
    if (isPlayerAlive)
    {   
	   playerSprite.setTexture(currentPlayerAnimation.texture);
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
	   if(isPlayerAlive)
	   {
		  if (isPlayerMoving)
			 playerSprite.move(dirX*playerSpeed*deltaTime.asSeconds(), dirY*playerSpeed*deltaTime.asSeconds());
		  movement_text.move(playerMovement*deltaTime.asSeconds());
		  playerPosition.x = playerSprite.getPosition().x;
		  playerPosition.y = playerSprite.getPosition().y;

		  if (isPlayerMoving)
			 if ((strcmp(playerDirection, "left") == 0 && playerPosition.x <= playerDestination.x) || (strcmp(playerDirection, "right") == 0 && playerPosition.x >= playerDestination.x)
				|| (strcmp(playerDirection, "up") == 0 && playerPosition.y <= playerDestination.y) || (strcmp(playerDirection, "down") == 0 && playerPosition.y >= playerDestination.y))  
			 {
				playerSprite.setPosition(playerDestination.x, playerDestination.y);
				if (strcmp(playerDirection, "left") == 0)
				{
				    playerGridX--;

				    if (!leftKey || grid[playerGridY][playerGridX - 1] == 'U' || grid[playerGridY][playerGridX - 1] == 'D' || grid[playerGridY][playerGridX - 1] == 'B')
				    {
					   isPlayerMoving = false;
					   currentPlayerAnimation = playerIdleLeft;
				    }
				    else playerDestination.x -= 50;

				}
				else if (strcmp(playerDirection, "right") == 0)
				{
				    playerGridX++;

				    if (!rightKey || grid[playerGridY][playerGridX+1] == 'U' || grid[playerGridY][playerGridX + 1] == 'D' || grid[playerGridY][playerGridX + 1] == 'B')
				    {
					   isPlayerMoving = false;
					   currentPlayerAnimation = playerIdleRight;
				    }
				    else playerDestination.x += 50;
				}
				else if (strcmp(playerDirection, "up") == 0)
				{
				    playerGridY--;

				    if (!upKey || grid[playerGridY - 1][playerGridX] == 'U' || grid[playerGridY - 1][playerGridX] == 'D'  || grid[playerGridY - 1][playerGridX] == 'B')
				    {
					   isPlayerMoving = false;
					   currentPlayerAnimation = playerIdleUp;
				    }
				    else playerDestination.y -= 50;
				}
				else if (strcmp(playerDirection, "down") == 0)
				{
				    playerGridY++;

				    if (!downKey || grid[playerGridY + 1][playerGridX] == 'U' || grid[playerGridY + 1][playerGridX] == 'D' || grid[playerGridY + 1][playerGridX] == 'B')
				    {
					   isPlayerMoving = false;
					   currentPlayerAnimation = playerIdleDown;
				    }
				    else playerDestination.y += 50;
				}

			 }

		  playerMovement.x = 0;
		  playerMovement.y = 0;
		  movement_text.setString("x=" + to_string(playerPosition.x) + " y=" + to_string(playerPosition.y) + "xd=" + to_string(playerDestination.x) + " yd=" + to_string(playerDestination.y) + "\n"
			 + to_string(playerGridY) + " " + to_string(playerGridX) + "\n");
	   }
    }
    else
    {
	   currentPlayerDyingTime += deltaTime;
	   float scaledTime = (currentPlayerDyingTime.asSeconds() / currentPlayerAnimation.duration.asSeconds());
	   int currentFrame = (int)(scaledTime*currentPlayerAnimation.nFrames) + currentPlayerAnimation.startFrameOffset;
	   playerSprite.setTextureRect(IntRect(currentFrame * 51, 0, 51, 51));
	   if (currentPlayerDyingTime > playerDyingTime)
	   {
		  lives--;
		  if(lives>=0)
		  {
			 playerReset.play();
			 currentPlayerDyingTime = Time::Zero;
			 playerSprite.setPosition(playerStartX, playerStartY);
			 playerPosition.x = playerStartX;
			 playerPosition.y = playerStartY;
			 playerGridX = playerStartX / 50;
			 playerGridY = playerStartY / 50;
			 currentPlayerAnimation = playerIdleDown;
			 playerDestination.x = playerStartX;
			 playerDestination.y = playerStartY;
			 isPlayerMoving = false;
			 isPlayerAlive = true;
		  }
		  else menu = 3;
	   }
    }

}

void gameLevelUpdate(Time deltaTime)
{
    
    if (menu == 3)
    {
	   UI.score.setString("Final score: " + to_string(score));
    }
    else if (menu == 4)
    {
	   UI.score.setString("Final score: " + to_string(score));
    }
    else if(menu==0)
    {
	   if (countEnemy1 == nrEnemy1 && countEnemy2 == nrEnemy2 && countEnemy3 == nrEnemy3 && countGems == 20)
		  menu = 3;
	   levelTime -= deltaTime;
	   playerUpdate(deltaTime);
	   bombsUpdate(deltaTime);
	   explosionsUpdate(deltaTime);
	   enemy1Update(deltaTime);
	   enemy2Update(deltaTime);
	   enemy3Update(deltaTime);
	   pickUpUpdate(deltaTime);
	   debug.setString(to_string(bombLimit));
	   UI.bar.setPosition(playerPosition.x-800, playerPosition.y - 380);
	   UI.status.setPosition(playerPosition.x - 700, playerPosition.y - 350);
	   UI.status.setString("Lives: " + to_string(lives) + "  Score: " + to_string(score) + "  Time: "+to_string((int)levelTime.asSeconds()) +"  "+ "    Balloons: " + to_string(countEnemy1) + "/" + to_string(nrEnemy1) + "  "
		  + "Stones: " + to_string(countEnemy2) + "/" + to_string(nrEnemy2) + "  " + "Raindrops: " + to_string(countEnemy3) + "/" + to_string(nrEnemy3) + "  " + "Gems: " + to_string(countGems) + "/20" + "  ");
    }
}
void handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{   if(menu==0)
{
    if (key == sf::Keyboard::Up && isPlayerAlive)
    {
	   if (isPressed)
		  upKey = true;
	   else upKey = false;
	   if (isPlayerMoving == false && grid[playerGridY - 1][playerGridX] != 'U' &&grid[playerGridY - 1][playerGridX] != 'D'  && grid[playerGridY - 1][playerGridX] != 'B')
	   {
		  isPlayerMoving = true;
		  playerDestination.y -= 50;		
		  strcpy(playerDirection, "up");
		  currentPlayerAnimation = playerMoveUp;

		  dirX = 0;
		  dirY = -1;
	   }
    }
    else if (key == sf::Keyboard::Down && isPlayerAlive)
    {
	   if (isPressed)
		  downKey = true;
	   else downKey = false;

	   if (isPlayerMoving == false && grid[playerGridY + 1][playerGridX] != 'U' && grid[playerGridY + 1][playerGridX] != 'D' && grid[playerGridY + 1][playerGridX] != 'B')
	   {
		  isPlayerMoving = true;
		  playerDestination.y += 50;
		  strcpy(playerDirection, "down");
		  currentPlayerAnimation = playerMoveDown;
		  dirX = 0;
		  dirY = 1;
		  //	}
	   }
    }
    else if (key == sf::Keyboard::Left && isPlayerAlive)
    {
	   if (isPressed)
		  leftKey = true;
	   else leftKey = false;


	   if (isPlayerMoving == false && grid[playerGridY][playerGridX - 1] != 'U' &&grid[playerGridY][playerGridX - 1] != 'D' && grid[playerGridY][playerGridX - 1] != 'B')
	   {
		  isPlayerMoving = true;
		  playerDestination.x -= 50;
		  //if (strcmp(bomberman->direction, "left") != 0)
		  //{
		  strcpy(playerDirection, "left");
		  currentPlayerAnimation = playerMoveLeft;
		  //}
		  dirX = -1;
		  dirY = 0;
	   }
    }
    else if (key == sf::Keyboard::Right && isPlayerAlive)
    {
	   if (isPressed)
		  rightKey = true;
	   else rightKey = false;

	   if ( isPlayerMoving == false && grid[playerGridY][playerGridX + 1] != 'U' &&grid[playerGridY][playerGridX + 1] != 'D'  && grid[playerGridY][playerGridX + 1] != 'B')
	   {
		  isPlayerMoving = true;
		  playerDestination.x += 50;		 
		  strcpy(playerDirection, "right");
		  currentPlayerAnimation = playerMoveRight;
		  //}
		  dirX = 1;
		  dirY = 0;
	   }
    }
    else if (key == sf::Keyboard::Space && bombCount<bombLimit && grid[playerGridY][playerGridX]!='B' && isPlayerAlive)
    {
	   placeBomb();
	   bomb.play();
    }
    else if (key == sf::Keyboard::Escape)
    {
	   menu = 2;
    }
	   

}
else if(menu==1 || menu==2)
{
    if (key == sf::Keyboard::Return)
    {
	   menu = 0;
    }
    else if (key == sf::Keyboard::Escape)
	   window.close();
}
}
void placeBomb()
{   
    int i = 0;
    while (bombs[i].active)
	   i++;
   
    bombs[i].gridX = playerGridX;
    bombs[i].gridY = playerGridY;
    bombs[i].sprite.setPosition(bombs[i].gridX *50, bombs[i].gridY*50);
    bombs[i].active = true;
    grid[playerGridY][playerGridX] = 'B';
    bombCount++;

}
void bombsUpdate(sf::Time deltaTime)
{
    for(int i=0; i<100; i++)
	   if (bombs[i].active)
	   {
		  bombs[i].currentTime+=deltaTime;

		  float scaledTime = (currentTime.asSeconds() / 0.5);
		  int currentFrame = (int)(scaledTime*4);
		  currentFrame %= 4;
		  bombs[i].sprite.setTextureRect(IntRect(currentFrame * 82, 0, 82, 82));  
		  if (bombs[i].currentTime > bombDuration || bombs[i].forcedExplode)
		  {
			 bombs[i].active = false;
			 bombs[i].forcedExplode = false;
			 bombCount--;
			 grid[bombs[i].gridY][bombs[i].gridX] = '0';
			 bombs[i].currentTime = Time::Zero;
			 explode(bombs[i].sprite.getPosition().x, bombs[i].sprite.getPosition().y);
			 
		  }
	   }
}
bool checkDirectionChange()
{
    if(upKey)
    {
	   isPlayerMoving = true;
	   playerDestination.y -= 50;	  
	   strcpy(playerDirection, "up");
	   currentPlayerAnimation = playerMoveUp;

	   dirX = 0;
	   dirY = -1;
	   return true;
	   
    }
    else if (downKey)
    {
	   isPlayerMoving = true;
	   playerDestination.y += 50;	  
	   strcpy(playerDirection, "down");
	   currentPlayerAnimation = playerMoveDown;
	   dirX = 0;
	   dirY = 1;
	   return true;

    }
    else if (leftKey)
    {
	   isPlayerMoving = true;
	   playerDestination.x -= 50;	  
	   strcpy(playerDirection, "left");
	   currentPlayerAnimation = playerMoveLeft;
	   //}
	   dirX = -1;
	   dirY = 0;
	   return true;

    }
    else if (rightKey)
    {
	   isPlayerMoving = true;
	   playerDestination.x += 50;	  
	   strcpy(playerDirection, "right");
	   currentPlayerAnimation = playerMoveRight;
	   //}
	   dirX = 1;
	   dirY = 0;
	   return true;

    }
    return false;
}
void explode(int posX, int posY)
{   
    explosion.play();
    int p = 0, i, j;
    Vector2i explosionPos;
    //explosionPos.x = 0;
    //explosionPos.y = 0;
    while (explosions[p].active)
	   p++;
    explosions[p].active = true;
    explosions[p].center.setPosition(posX, posY);
    bool ok;
    explosions[p].innerUpCount = explosions[p].innerDownCount = explosions[p].innerLeftCount = explosions[p].innerRightCount = 0;
    explosions[p].isEndLeft = explosions[p].isEndRight = explosions[p].isEndUp = explosions[p].isEndDown = true;
    for (i = 1; i <= 4; i++) {
	   ok = true;
	   for (j = 1; j <= explosionLength && ok; j++) {
		  explosionPos.x = posX;
		  explosionPos.y = posY;
		  switch (i) {
		  case 1:
		  {
			 explosionPos.y -= j * 50;
			 //debug.setString(debug.getString() + " " + to_string(explosionPos.x) + " " + to_string(explosionPos.y) + "     ");
			 if (j < explosionLength)

			 {
				explosions[p].innerUp[j-1].setPosition(explosionPos.x, explosionPos.y);
				explosions[p].innerUpCount++;
				//debug.setString("ok " + to_string(explosionPos.x) + " " + to_string(explosionPos.y));

			 }
			 else
				explosions[p].endUp.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  case 2:
		  {

			 explosionPos.x += j * 50;
			 if (j < explosionLength)

			 {
				explosions[p].innerRight[j-1].setPosition(explosionPos.x, explosionPos.y);
				explosions[p].innerRightCount++;
			 }
			 else

				explosions[p].endRight.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  case 3:
		  {
			 explosionPos.y += j * 50;
			 if (j < explosionLength)

			 {
				explosions[p].innerDown[j-1].setPosition(explosionPos.x, explosionPos.y);
				explosions[p].innerDownCount++;
			 }
			 else

				explosions[p].endDown.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  case 4:
		  {
			 explosionPos.x -= j * 50;
			 if (j < explosionLength)

			 {
				explosions[p].innerLeft[j - 1].setPosition(explosionPos.x, explosionPos.y);
				explosions[p].innerLeftCount++;
			 }
			 else

				explosions[p].endLeft.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  }
		  if (grid[explosionPos.y/50][explosionPos.x/50] == 'U' ) //zid indestructibil
			 ok = false;
		  else if (grid[explosionPos.y/50][explosionPos.x/50] == 'D') {  //zid destructibil
			 ok = false;
			 for (int q = 0; q < nrDestroyableBlocks; q++)
				if ((int)(destroyableBlocksSprites[q].getPosition().x) == explosionPos.x && (int)(destroyableBlocksSprites[q].getPosition().y) == explosionPos.y)
				{
				    isDestroyableBlockActive[q] = false;
				    grid[explosionPos.y / 50][explosionPos.x / 50]='0';
				    break;
				}

		  }
		    if (grid[explosionPos.y/50][explosionPos.x/50] == 'B') 
			 { //o alta bomba
			  for(int q=0; q<10; q++)
				 if (bombs[q].gridX == explosionPos.x / 50 && bombs[q].gridY == explosionPos.y / 50)
					bombs[q].forcedExplode = true;

			  }
	   }
	   if(!ok)
		  switch (i)
		  {
		  case 1:
			 explosions[p].isEndUp = false;
			 break;
		  case 2:
			 explosions[p].isEndRight = false;
			 break;
		  case 3:
			 explosions[p].isEndDown = false;
			 break;
		  case 4:
			 explosions[p].isEndLeft = false;
			 break;

		  }
    }
}
    

void explosionsUpdate(sf::Time deltaTime)
{
	 
    int size=0;
    for (int i = 0; i<100; i++)
	   if (explosions[i].active)
	   {
		  explosions[i].currentTime += deltaTime;
		  float scaledTime = (currentTime.asSeconds() / 1.);
		  int currentFrame = (int)(scaledTime * 8);
		  currentFrame %= 8;
		  switch (currentFrame)
		  {
		  case 0:
		 // case 8:
			 explosions[i].currentTexture = explosion1_texture;
			 size = 75;
			 changeExplosionScale(i, 75.);
			 break;
		  case 1:
		  case 7:
			 explosions[i].currentTexture = explosion2_texture;
			 changeExplosionScale(i, 77.);
			 size = 77;
			 break;
		  case 2: 
		  case 6:
			 explosions[i].currentTexture = explosion3_texture;
			 size = 78;
			 changeExplosionScale(i, 78.);
			 break;
		  case 3:
		  case 4:
		  case 5:
			 explosions[i].currentTexture = explosion4_texture;
			 changeExplosionScale(i, 80.);
			 size = 80;
			 break;
		  }
		  explosions[i].endLeft.setTexture(explosions[i].currentTexture);
		  explosions[i].endLeft.setTextureRect(IntRect(0 * size, 0, size, size));
		  if (checkIntersection(playerSprite, explosions[i].endLeft) && isPlayerAlive)
		  {
			 isPlayerAlive = false;
			 currentPlayerAnimation = playerDying;
			 playerDie.play();
		  }
		  for (int p = 0; p < nrEnemy1; p++)
			 if (checkIntersection(enemy1[p].sprite, explosions[i].endLeft))
				enemy1[p].alive = false;
		  for (int p = 0; p < nrEnemy2; p++)
			 if (checkIntersection(enemy2[p].sprite, explosions[i].endLeft))
				enemy2[p].alive = false;
		  for (int p = 0; p < nrEnemy3; p++)
			 if (checkIntersection(enemy3[p].sprite, explosions[i].endLeft))
				enemy3[p].alive = false;

		  for (int j = 0; j< explosions[i].innerLeftCount; j++)
		  {
			 explosions[i].innerLeft[j].setTexture(explosions[i].currentTexture);
			 explosions[i].innerLeft[j].setTextureRect(IntRect(1 * size, 0, size, size));
			 if (checkIntersection(playerSprite, explosions[i].innerLeft[j]) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();
			 }
			 for (int p = 0; p < nrEnemy1; p++)
				if (checkIntersection(enemy1[p].sprite, explosions[i].innerLeft[j]))
				    enemy1[p].alive = false;
			 for (int p = 0; p < nrEnemy2; p++)
				if (checkIntersection(enemy2[p].sprite, explosions[i].innerLeft[j]))
				    enemy2[p].alive = false;
			 for (int p = 0; p < nrEnemy3; p++)
				if (checkIntersection(enemy3[p].sprite, explosions[i].innerLeft[j]))
				    enemy3[p].alive = false;
		  }

		  explosions[i].center.setTexture(explosions[i].currentTexture);
		  explosions[i].center.setTextureRect(IntRect(2 * size, 0, size, size));
		  if (checkIntersection(playerSprite, explosions[i].center) && isPlayerAlive)
		  {
			 isPlayerAlive = false;
			 currentPlayerAnimation = playerDying;
			 playerDie.play();
		  }
		  for (int p = 0; p < nrEnemy1; p++)
			 if (checkIntersection(enemy1[p].sprite, explosions[i].center))
				enemy1[p].alive = false;
		  for (int p = 0; p < nrEnemy2; p++)
			 if (checkIntersection(enemy2[p].sprite, explosions[i].center))
				enemy2[p].alive = false;
		  for (int p = 0; p < nrEnemy3; p++)
			 if (checkIntersection(enemy3[p].sprite, explosions[i].center))
				enemy3[p].alive = false;

		  for (int j = 0; j< explosions[i].innerRightCount; j++)
		  {
			 explosions[i].innerRight[j].setTexture(explosions[i].currentTexture);
			 explosions[i].innerRight[j].setTextureRect(IntRect(3 * size, 0, size, size));
			 if (checkIntersection(playerSprite, explosions[i].innerRight[j]) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();
			 }
			 for (int p = 0; p < nrEnemy1; p++)
				if (checkIntersection(enemy1[p].sprite, explosions[i].innerRight[j]))
				    enemy1[p].alive = false;
			 for (int p = 0; p < nrEnemy2; p++)
				if (checkIntersection(enemy2[p].sprite, explosions[i].innerRight[j]))
				    enemy2[p].alive = false;
			 for (int p = 0; p < nrEnemy3; p++)
				if (checkIntersection(enemy3[p].sprite, explosions[i].innerRight[j]))
				    enemy3[p].alive = false;

		  }

		  explosions[i].endRight.setTexture(explosions[i].currentTexture);
		  explosions[i].endRight.setTextureRect(IntRect(4 * size, 0, size, size));
		  if (checkIntersection(playerSprite, explosions[i].endRight) && isPlayerAlive)
		  {
			 isPlayerAlive = false;
			 currentPlayerAnimation = playerDying;
			 playerDie.play();
		  }
		  for (int p = 0; p < nrEnemy1; p++)
			 if (checkIntersection(enemy1[p].sprite, explosions[i].endRight))
				enemy1[p].alive = false;
		  for (int p = 0; p < nrEnemy2; p++)
			 if (checkIntersection(enemy2[p].sprite, explosions[i].endRight))
				enemy2[p].alive = false;
		  for (int p = 0; p < nrEnemy3; p++)
			 if (checkIntersection(enemy3[p].sprite, explosions[i].endRight))
				enemy3[p].alive = false;
		  for (int j = 0; j< explosions[i].innerUpCount; j++)
		  {
			 explosions[i].innerUp[j].setTexture(explosions[i].currentTexture);
			 explosions[i].innerUp[j].setTextureRect(IntRect(5 * size, 0, size, size));
			 if (checkIntersection(playerSprite, explosions[i].innerUp[j]) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();
			 }
			 for (int p = 0; p < nrEnemy1; p++)
				if (checkIntersection(enemy1[p].sprite, explosions[i].innerUp[j]))
				    enemy1[p].alive = false;
			 for (int p = 0; p < nrEnemy2; p++)
				if (checkIntersection(enemy2[p].sprite, explosions[i].innerUp[j]))
				    enemy2[p].alive = false;
			 for (int p = 0; p < nrEnemy3; p++)
				if (checkIntersection(enemy3[p].sprite, explosions[i].innerUp[j]))
				    enemy3[p].alive = false;
		  }
		  explosions[i].endUp.setTexture(explosions[i].currentTexture);
		  explosions[i].endUp.setTextureRect(IntRect(6 * size, 0, size, size));
		  if (checkIntersection(playerSprite, explosions[i].endUp) && isPlayerAlive)
		  {
			 isPlayerAlive = false;
			 currentPlayerAnimation = playerDying;
			 playerDie.play();
		  }
		  for (int p = 0; p < nrEnemy1; p++)
			 if (checkIntersection(enemy1[p].sprite, explosions[i].endUp))
				enemy1[p].alive = false;
		  for (int p = 0; p < nrEnemy2; p++)
			 if (checkIntersection(enemy2[p].sprite, explosions[i].endUp))
				enemy2[p].alive = false;
		  for (int p = 0; p < nrEnemy3; p++)
			 if (checkIntersection(enemy3[p].sprite, explosions[i].endUp))
				enemy3[p].alive = false;
		  for (int j = 0; j< explosions[i].innerDownCount; j++)
		  {
			 explosions[i].innerDown[j].setTexture(explosions[i].currentTexture);
			 explosions[i].innerDown[j].setTextureRect(IntRect(7 * size, 0, size, size));
			 if (checkIntersection(playerSprite, explosions[i].innerDown[j]) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();
			 }
			 for (int p = 0; p < nrEnemy1; p++)
				if (checkIntersection(enemy1[p].sprite, explosions[i].innerDown[j]))
				    enemy1[p].alive = false;
			 for (int p = 0; p < nrEnemy2; p++)
				if (checkIntersection(enemy2[p].sprite, explosions[i].innerDown[j]))
				    enemy2[p].alive = false;
			 for (int p = 0; p < nrEnemy3; p++)
				if (checkIntersection(enemy3[p].sprite, explosions[i].innerDown[j]))
				    enemy3[p].alive = false;
		  }
		  explosions[i].endDown.setTexture(explosions[i].currentTexture);
		  explosions[i].endDown.setTextureRect(IntRect(8 * size, 0, size, size));
		  if (checkIntersection(playerSprite, explosions[i].endDown) && isPlayerAlive)
		  {
			 isPlayerAlive = false;
			 currentPlayerAnimation = playerDying;
			 playerDie.play();
		  }
		  for (int p = 0; p < nrEnemy1; p++)
			 if (checkIntersection(enemy1[p].sprite, explosions[i].endDown))
				enemy1[p].alive = false;
		  for (int p = 0; p < nrEnemy2; p++)
			 if (checkIntersection(enemy2[p].sprite, explosions[i].endDown))
				enemy2[p].alive = false;
		  for (int p = 0; p < nrEnemy3; p++)
			 if (checkIntersection(enemy3[p].sprite, explosions[i].endDown))
				enemy3[p].alive = false;


		  if (explosions[i].currentTime > explosionDuration)
		  {
			 explosions[i].active = false;
			 explosions[i].currentTime = Time::Zero;
		  }
		   
	   }
}

void enemy1Update(Time deltaTime)
{
  
    for (int i = 0; i < 10; i++)
	   if(enemy1[i].active)
	   {
		  if (enemy1[i].alive)
		  {
			 if (enemy1[i].sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();
			 }
			// else
			 {
				if (enemy1[i].isMoving)
				{
				    enemy1[i].sprite.move(enemy1[i].dirX*enemy1Speed*deltaTime.asSeconds(), enemy1[i].dirY*enemy1Speed*deltaTime.asSeconds());
				    if (abs(enemy1[i].sprite.getPosition().x - enemy1[i].destination.x) <= 1 && abs(enemy1[i].sprite.getPosition().y - enemy1[i].destination.y) <= 1)
				    {
					
					   enemy1[i].isMoving = false;
					   enemy1[i].sprite.setPosition(enemy1[i].destination.x, enemy1[i].destination.y);					   
					   enemy1[i].gridX = enemy1[i].sprite.getPosition().x / 50;
					   enemy1[i].gridY = enemy1[i].sprite.getPosition().y / 50;
				    }
				}
				else
				{

				    enemyValidDirections(enemy1[i].gridX, enemy1[i].gridY, validDirEnemy1);
				    if (validDirEnemy1[0] > 0)
				    {
					   enemy1[i].direction = rand() % validDirEnemy1[0] + 1;
					   //debug.setString(debug.getString() + to_string(enemy1[i].direction));
					   switch (validDirEnemy1[enemy1[i].direction])
					   {
					   case 1:
						  enemy1[i].destination.y -= 50;
						  enemy1[i].dirX = 0;
						  enemy1[i].dirY = -1;
						  enemy1[i].isMoving = true;
						  break;
					   case 2:
						  enemy1[i].destination.x += 50;
						  enemy1[i].dirX = 1;
						  enemy1[i].dirY = 0;
						  enemy1[i].isMoving = true;
						  enemy1[i].spriteFlip = false;
						  break;
					   case 3:
						  enemy1[i].destination.y += 50;
						  enemy1[i].dirX = 0;
						  enemy1[i].dirY = 1;
						  enemy1[i].isMoving = true;
						  break;
					   case 4:
						  enemy1[i].destination.x -= 50;
						  enemy1[i].dirX = -1;
						  enemy1[i].dirY = 0;
						  enemy1[i].isMoving = true;
						  enemy1[i].spriteFlip = true;
						  break;

					   }
				    }
				   // else debug.setString(debug.getString() + to_string(enemy1[i].gridY) + " " + to_string(enemy1[i].gridY) + " " + to_string(grid[enemy1[i].gridY - 1][enemy1[i].gridX]) + " " + to_string(grid[enemy1[i].gridY][enemy1[i].gridX + 1]) + " " + to_string(grid[enemy1[i].gridY + 1][enemy1[i].gridX]) + " " + to_string(grid[enemy1[i].gridY][enemy1[i].gridX - 1]) + "    ");
				}

				enemy1[i].currentTime += deltaTime;
				float scaledTime = (currentTime.asSeconds() / 2.);
				int currentFrame = (int)(scaledTime * 4);
				currentFrame %= 4;
				if (enemy1[i].spriteFlip)
				    currentFrame += 4;
				enemy1[i].sprite.setTextureRect(IntRect(currentFrame * 28, 0, 28, 28));
			 }
		  }
		  else
		  {
			 enemy1[i].dyingTime += deltaTime;
			 float scaledTime = (enemy1[i].dyingTime.asSeconds() / 2.5);
			 int currentFrame = (int)(scaledTime * 8);
			 currentFrame %= 8;
			// enemy1[i].sprite.setTextureRect(IntRect(9 * 28, 0, 28, 28));

			 if (currentFrame <= 3)
				enemy1[i].sprite.setTextureRect(IntRect(8 * 28, 0, 28, 28));
			 else switch (currentFrame)
			 {
			 case 4:
				enemy1[i].sprite.setTextureRect(IntRect(9 * 28, 0, 28, 28));
				break;
			 case 5:
				enemy1[i].sprite.setTextureRect(IntRect(10 * 28, 0, 28, 28));
				break;
			 case 6:
				enemy1[i].sprite.setTextureRect(IntRect(11 * 28, 0, 28, 28));
				break;
			 case 7:
				enemy1[i].sprite.setTextureRect(IntRect(12 * 28, 0, 28, 28));
				break;
			 default:
				enemy1[i].sprite.setTextureRect(IntRect(9 * 28, 0, 28, 28));
				break;

			 }
			 if (enemy1[i].dyingTime > seconds(2.5))
			 {
				enemy1[i].active = false;
				countEnemy1++;
				score += 300;
				enemy1[i].currentTime = Time::Zero;
				enemy1[i].dyingTime = Time::Zero;
				enemy.play();
			 }


		  }
	   }
    }

void enemyValidDirections(int gridX, int gridY, int v[5])
{
    int i=0;
    v[0] = 0;
    if (grid[gridY - 1][gridX] =='0')
    {
	   v[++i] = 1; //debug.setString(debug.getString() + "1 ");
    }
    if (grid[gridY][gridX+1] == '0')
    {
	   v[++i] = 2; //debug.setString(debug.getString() + "2 ");
    }
    if (grid[gridY + 1][gridX] == '0')
    {
	   v[++i] = 3; //debug.setString(debug.getString() + "3 ");
    }
    if (grid[gridY][gridX-1] == '0')
    {
	   v[++i] = 4; //debug.setString(debug.getString() + "4 ");
    }
    v[0] = i;
}
bool checkIntersection(Sprite a, Sprite b)
{
    if (a.getGlobalBounds().intersects(b.getGlobalBounds()))
	   return true;
    return false;
}
void enemy2Update(Time deltaTime)
{
    {
	   for (int i = 0; i < 10; i++)
		  if (enemy2[i].active)
		  {
			 
			 if (enemy2[i].alive)
			 {
				if (enemy2[i].sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()) && isPlayerAlive)
				{
				    isPlayerAlive = false;
				    currentPlayerAnimation = playerDying;
				    playerDie.play();
				}
				{
				    if (enemy2[i].isMoving)
				    {
					  // debug.setString(debug.getString() + "da ");
					  // debug.setString(to_string(enemy2[i].destination.x));
					 //  debug.setString(debug.getString() + to_string(abs(enemy2[i].sprite.getPosition().x - enemy2[i].destination.x)) +" "+ to_string(abs(enemy2[i].sprite.getPosition().y - enemy2[i].destination.y)));
					   enemy2[i].sprite.move(enemy2[i].dirX*enemy2Speed*deltaTime.asSeconds(), enemy2[i].dirY*enemy2Speed*deltaTime.asSeconds());
					   if (abs(enemy2[i].sprite.getPosition().x - enemy2[i].destination.x) <= 1 && abs(enemy2[i].sprite.getPosition().y - enemy2[i].destination.y) <= 1)
					   {
						 
						  enemy2[i].isMoving = false;
						  enemy2[i].sprite.setPosition(enemy2[i].destination.x, enemy2[i].destination.y);
						  enemy2[i].gridX = enemy2[i].sprite.getPosition().x / 50;
						  enemy2[i].gridY = enemy2[i].sprite.getPosition().y / 50;
						
					   }
				    }
				    else
				    {
					   
					   if (enemy2[i].dirX == 1 && grid[enemy2[i].gridY][enemy2[i].gridX + 1] == '0')
					   {
						  {
							 enemy2[i].isMoving = true;
							 enemy2[i].destination.x += 50;
						  }
						  //debug.setString(debug.getString() + "da ");
					   }
					   else if (enemy2[i].dirX == -1 && grid[enemy2[i].gridY][enemy2[i].gridX - 1] == '0')
					   {
						  enemy2[i].isMoving = true;
						  enemy2[i].destination.x -= 50;
					   }
					   else if (enemy2[i].dirY == 1 && grid[enemy2[i].gridY + 1][enemy2[i].gridX] == '0')
					   {
						  enemy2[i].isMoving = true;
						  enemy2[i].destination.y += 50;
					   }
					   else if (enemy2[i].dirY == -1 && grid[enemy2[i].gridY - 1][enemy2[i].gridX] == '0')
					   {
						  enemy2[i].isMoving = true;
						  enemy2[i].destination.y -= 50;
					   }
					   else
					   {
						 // debug.setString(debug.getString() + "da ");
						  enemyValidDirections(enemy2[i].gridX, enemy2[i].gridY, validDirEnemy2);
						  if (validDirEnemy2[0] > 0)
						  {
							 enemy2[i].direction = rand() % validDirEnemy2[0] + 1;
							 //debug.setString(debug.getString() + to_string(enemy1[i].direction));
							 switch (validDirEnemy2[enemy2[i].direction])
							 {
							 case 1:
								enemy2[i].destination.y -= 50;
								enemy2[i].dirX = 0;
								enemy2[i].dirY = -1;
								enemy2[i].isMoving = true;
								break;
							 case 2:
								enemy2[i].destination.x += 50;
								enemy2[i].dirX = 1;
								enemy2[i].dirY = 0;
								enemy2[i].isMoving = true;
								enemy2[i].spriteFlip = false;
								break;
							 case 3:
								enemy2[i].destination.y += 50;
								enemy2[i].dirX = 0;
								enemy2[i].dirY = 1;
								enemy2[i].isMoving = true;
								break;
							 case 4:
								enemy2[i].destination.x -= 50;
								enemy2[i].dirX = -1;
								enemy2[i].dirY = 0;
								enemy2[i].isMoving = true;
								enemy2[i].spriteFlip = true;
								break;

							 }
						  }
						  // else debug.setString(debug.getString() + to_string(enemy2[i].gridY) + " " + to_string(enemy2[i].gridY) + " " + to_string(grid[enemy2[i].gridY - 1][enemy2[i].gridX]) + " " + to_string(grid[enemy2[i].gridY][enemy2[i].gridX + 1]) + " " + to_string(grid[enemy2[i].gridY + 1][enemy2[i].gridX]) + " " + to_string(grid[enemy2[i].gridY][enemy2[i].gridX - 1]) + "    ");
					   }
				    }

				    enemy2[i].currentTime += deltaTime;
				    float scaledTime = (currentTime.asSeconds() / 0.5);
				    int currentFrame = (int)(scaledTime * 3);
				    currentFrame %= 3;
				    if (enemy2[i].spriteFlip &&currentFrame==1)
					   currentFrame += 2;
				    enemy2[i].sprite.setTextureRect(IntRect(currentFrame * 24, 0, 24, 24));
				}
			 }
			 else
			 {
				enemy2[i].dyingTime += deltaTime;
				float scaledTime = (enemy2[i].dyingTime.asSeconds() / 2.5);
				int currentFrame = (int)(scaledTime * 8);
				currentFrame %= 8;
				//enemy2[i].sprite.setTextureRect(IntRect(9 * 28, 0, 28, 28));

				if (currentFrame <= 3)
				    enemy2[i].sprite.setTextureRect(IntRect(4 * 24, 0, 24, 24));
				else switch (currentFrame)
				{
				case 4:
				    enemy2[i].sprite.setTextureRect(IntRect(5 * 24, 0, 24, 24));
				    break;
				case 5:
				    enemy2[i].sprite.setTextureRect(IntRect(6 * 24, 0, 24, 24));
				    break;
				case 6:
				    enemy2[i].sprite.setTextureRect(IntRect(7 * 24, 0, 24, 24));
				    break;
				case 7:
				    enemy2[i].sprite.setTextureRect(IntRect(8 * 24, 0, 24, 24));
				    break;
				default:
				    enemy2[i].sprite.setTextureRect(IntRect(4 * 24, 0, 24, 24));
				    break;

				}
				if (enemy2[i].dyingTime > seconds(2.5))
				{
				    enemy2[i].active = false;
				    countEnemy2++;
				    score += 400;
				    enemy2[i].currentTime = Time::Zero;
				    enemy2[i].dyingTime = Time::Zero;
				    enemy.play();
				}


			 }
		  }
    }
}
void enemy3Update(sf::Time deltaTime)
{
    for (int i = 0; i < 10; i++)
	   if (enemy3[i].active)
	   {
		  if (enemy3[i].alive)
		  {
			 if (enemy3[i].sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()) && isPlayerAlive)
			 {
				isPlayerAlive = false;
				currentPlayerAnimation = playerDying;
				playerDie.play();

			 }
			 {
				if (enemy3[i].isMoving)
				{
				    enemy3[i].sprite.move(enemy3[i].dirX*enemy3Speed*deltaTime.asSeconds(), enemy3[i].dirY*enemy3Speed*deltaTime.asSeconds());
				    if (abs(enemy3[i].sprite.getPosition().x - enemy3[i].destination.x) <= 1 && abs(enemy3[i].sprite.getPosition().y - enemy3[i].destination.y) <= 1)
				    {
					   

					   enemy3[i].isMoving = false;
					   enemy3[i].sprite.setPosition(enemy3[i].destination.x, enemy3[i].destination.y);
					   enemy3[i].gridX = enemy3[i].sprite.getPosition().x / 50;
					   enemy3[i].gridY = enemy3[i].sprite.getPosition().y / 50;
					   enemy3[i].randomWaitingTime = milliseconds(rand() % 2500);
				    }
				}
				else
				{
				    if (enemy3[i].currentWaitingTime <= enemy3[i].randomWaitingTime)
					   enemy3[i].currentWaitingTime += deltaTime;
				    else
				    {
					   if (enemy3[i].currentWaitingTime != Time::Zero)
						  enemy3[i].currentWaitingTime = Time::Zero;
					   if (enemy3[i].randomWaitingTime != Time::Zero)
						  enemy3[i].randomWaitingTime = Time::Zero;
					   enemyValidDirections(enemy3[i].gridX, enemy3[i].gridY, validDirEnemy3);
					   if (validDirEnemy3[0] > 0)
					   {
						  enemy3[i].direction = rand() % validDirEnemy3[0] + 1;
						  //debug.setString(debug.getString() + to_string(enemy1[i].direction));
						  switch (validDirEnemy3[enemy3[i].direction])
						  {
						  case 1:
							 enemy3[i].destination.y -= 50;
							 enemy3[i].dirX = 0;
							 enemy3[i].dirY = -1;
							 enemy3[i].isMoving = true;
							 break;
						  case 2:
							 enemy3[i].destination.x += 50;
							 enemy3[i].dirX = 1;
							 enemy3[i].dirY = 0;
							 enemy3[i].isMoving = true;
							 enemy3[i].spriteFlip = false;
							 break;
						  case 3:
							 enemy3[i].destination.y += 50;
							 enemy3[i].dirX = 0;
							 enemy3[i].dirY = 1;
							 enemy3[i].isMoving = true;
							 break;
						  case 4:
							 enemy3[i].destination.x -= 50;
							 enemy3[i].dirX = -1;
							 enemy3[i].dirY = 0;
							 enemy3[i].isMoving = true;
							 enemy3[i].spriteFlip = true;
							 break;

						  }
					   }
					   // else debug.setString(debug.getString() + to_string(enemy1[i].gridY) + " " + to_string(enemy1[i].gridY) + " " + to_string(grid[enemy1[i].gridY - 1][enemy1[i].gridX]) + " " + to_string(grid[enemy1[i].gridY][enemy1[i].gridX + 1]) + " " + to_string(grid[enemy1[i].gridY + 1][enemy1[i].gridX]) + " " + to_string(grid[enemy1[i].gridY][enemy1[i].gridX - 1]) + "    ");
				    }
				}

				enemy3[i].currentTime += deltaTime;
				float scaledTime = (currentTime.asSeconds() / 0.4);
				int currentFrame = (int)(scaledTime * 4);
				currentFrame %= 4;
				if (enemy3[i].spriteFlip)
				    currentFrame += 4;
				if(enemy3[i].currentWaitingTime==Time::Zero)
				    enemy3[i].sprite.setTextureRect(IntRect(currentFrame * 25, 0, 25, 25));
				else
				{
				    if(!enemy3[i].spriteFlip)
					   enemy3[i].sprite.setTextureRect(IntRect(1 * 25, 0, 25, 25));
				    else 
					   enemy3[i].sprite.setTextureRect(IntRect(5 * 25, 0, 25, 25));
				}

			 }
		  }
		  else
		  {
			 enemy3[i].dyingTime += deltaTime;
			 float scaledTime = (enemy3[i].dyingTime.asSeconds() / 2.5);
			 int currentFrame = (int)(scaledTime * 8);
			 currentFrame %= 8;
			 // enemy1[i].sprite.setTextureRect(IntRect(9 * 28, 0, 28, 28));

			 if (currentFrame <= 3)
				enemy3[i].sprite.setTextureRect(IntRect(9 * 25, 0, 25, 25));
			 else switch (currentFrame)
			 {
			 case 4:
				enemy3[i].sprite.setTextureRect(IntRect(10 * 25, 0, 25, 25));
				break;
			 case 5:
				enemy3[i].sprite.setTextureRect(IntRect(11 * 25, 0, 25, 25));
				break;
			 case 6:
				enemy3[i].sprite.setTextureRect(IntRect(12 * 25, 0, 25, 25));
				break;
			 case 7:
				enemy3[i].sprite.setTextureRect(IntRect(13 * 25, 0, 25, 25));
				break;
			 default:
				enemy3[i].sprite.setTextureRect(IntRect(9 * 25, 0, 25, 25));
				break;

			 }
			 if (enemy3[i].dyingTime > seconds(2.5))
			 {
				enemy3[i].active = false;
				countEnemy3++;
				score += 500;
				enemy3[i].currentTime = Time::Zero;
				enemy3[i].dyingTime = Time::Zero;
				enemy.play();
			 }


		  }
	   }
}
void changeExplosionScale(int i, double x)
{
    explosions[i].center.setTexture(explosion1_texture);
    explosions[i].center.setScale(50 / x, 50 / x);
    for (int j = 0; j< 8; j++)
    {
	   explosions[i].innerUp[j].setTexture(explosion1_texture);
	   explosions[i].innerUp[j].setScale(50 / x, 50 / x);
    }
    for (int j = 0; j< 8; j++)
    {
	   explosions[i].innerDown[j].setTexture(explosion1_texture);
	   explosions[i].innerDown[j].setScale(50 / x, 50 / x);
    }
    for (int j = 0; j< 8; j++)
    {
	   explosions[i].innerLeft[j].setTexture(explosion1_texture);
	   explosions[i].innerLeft[j].setScale(50 / x, 50 / x);
    }
    for (int j = 0; j<8; j++)
    {
	   explosions[i].innerRight[j].setTexture(explosion1_texture);
	   explosions[i].innerRight[j].setScale(50 / x, 50 / x);
    }
    explosions[i].endUp.setTexture(explosion1_texture);
    explosions[i].endUp.setScale(50 / x, 50 / x);
    explosions[i].endDown.setTexture(explosion1_texture);
    explosions[i].endDown.setScale(50 / x, 50 / x);
    explosions[i].endLeft.setTexture(explosion1_texture);
    explosions[i].endLeft.setScale(50 / x, 50 / x);
    explosions[i].endRight.setTexture(explosion1_texture);
    explosions[i].endRight.setScale(50 / x, 50 / x);
}
void pickUpDraw()
{
    int i;
    for (i = 0; i < 20; i++)
	   if (gem[i].active)
		  window.draw(gem[i].sprite);
    for (i = 0; i < 10; i++)
	   if (bombPowerUp[i].active)
		  window.draw(bombPowerUp[i].sprite);
    for (i = 0; i < 10; i++)
	   if (speedPowerUp[i].active)
		  window.draw(speedPowerUp[i].sprite);
    for (i = 0; i < 10; i++)
	   if (explosionPowerUp[i].active)
		  window.draw(explosionPowerUp[i].sprite);

}
void pickUpUpdate(Time deltaTime)
{
    int i;
    for (i = 0; i < 20; i++)
	   if (gem[i].active && checkIntersection(gem[i].sprite, playerSprite))
	   {
		  gem[i].active = false;
		  countGems++;
		  score += 50;
		  pickup.play();
	   }
    for (i = 0; i < 10; i++)
	   if (bombPowerUp[i].active && checkIntersection(bombPowerUp[i].sprite, playerSprite))
	   {
		  bombPowerUp[i].active = false;
		  bombCount++;
		  score += 50;
		  bombLimit++;
		  pickup.play();
	   }
    for (i = 0; i < 10; i++)
	   if (explosionPowerUp[i].active && checkIntersection(explosionPowerUp[i].sprite, playerSprite))
	   {
		  explosionPowerUp[i].active = false;
		  score += 50;
		  explosionLength++;
		  pickup.play();
	   }
    for (i = 0; i < 10; i++)
	   if (speedPowerUp[i].active && checkIntersection(speedPowerUp[i].sprite, playerSprite))
	   {
		  speedPowerUp[i].active = false;
		  score += 50;
		  playerSpeed += 10;
		  pickup.play();
	   }
    

}
