#include "SFML/Graphics.hpp"
#include "GameLevel.h"
#include <fstream>

using namespace std;
using namespace sf;

int playerGridX, playerGridY;
Vector2i playerPosition;
Vector2i playerDestination;
Vector2f playerMovement;
int playerSpeed;
int lives = 3;


struct enemy1
{
    Sprite sprite;
    bool active = false;
    int gridX, gridY;
    Time currentTime = Time::Zero;
}enemy1[10];

Texture  bomberman_up, bomberman_down, bomberman_left, bomberman_right;
Texture bomb_texture;
Sprite playerSprite;
RenderWindow window;
int windowWidth=1200, windowHeight=600;

View view, currentView;
Texture background_texture, block_texture, destroyableBlock_texture;
Texture enemy1_texture, enemy2_texture, enemy3_texture;
Sprite backgroundSprite, blocksSprites[100], destroyableBlocksSprites[100];
int nrBlocks = 0, nrDestroyableBlocks = 0;
int nrEnemy1=-1;
int playerStartX, playerStartY;
char grid[100][100];
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

Animation currentPlayerAnimation, playerMoveLeft, playerIdleLeft, playerMoveRight, playerIdleRight, playerMoveUp, playerIdleUp, playerMoveDown, playerIdleDown;
Animation bombAnimation;

bool isPlayerMoving;
char playerDirection[6];

Time currentTime;
Text movement_text, text, debug;
Font font;

bool isDestroyableBlockActive[100];
int bombLimit = 2, bombCount = 0;
Time bombDuration = seconds(1);
struct bomb
{
    Sprite sprite;
    int gridX, gridY;
    bool active=false;
    Time currentTime=Time::Zero;

}bombs[10];

struct explosion
{
    Sprite center;
    Sprite endUp, endDown, endLeft, endRight;
    Sprite innerUp, innerDown, innerLeft, innerRight;
    bool active = false;
    Time currentTime = Time::Zero;
    Texture currentTexture;
}explosions[10];

Texture explosion1_texture, explosion2_texture, explosion3_texture, explosion4_texture;
int explosionLength = 4;
Time explosionDuration = seconds(1);

//Functia principala
void gameLevel()
{
		 font.loadFromFile("Resources/Fonts/calibri.ttf");
		 debug.setFont(font);
		 debug.setPosition(200, 200);
		 
		 RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bomberman");
	  // ,mIsMovingUp(false)  
	   window.setFramerateLimit(60);
	   //playerSprite.setOrigin(50 / 2., 50 / 2.);
	   View view(FloatRect(0, 0, window.getSize().x, window.getSize().y));
	   view.setCenter(playerSprite.getOrigin());
	   //window.setView(view);
	   background_texture.loadFromFile("Resources/Textures/Tiles.png");
	   block_texture.loadFromFile("Resources/Textures/block.PNG");
	   destroyableBlock_texture.loadFromFile("Resources/Textures/destroyable_block.PNG");
	   bomb_texture.loadFromFile("Resources/Textures/bomb.png");
	   explosion1_texture.loadFromFile("Resources/Textures/explosion1.png");
	   explosion2_texture.loadFromFile("Resources/Textures/explosion2.png");
	   explosion3_texture.loadFromFile("Resources/Textures/explosion3.png");
	   explosion4_texture.loadFromFile("Resources/Textures/explosion4.png");
	   enemy1_texture.loadFromFile("Resources/Textures/enemy1.png");

	   backgroundSprite.setTexture(background_texture);
	   backgroundSprite.setScale((float)50 / 56, (float)50 / 56);
	   leftKey = rightKey = upKey = downKey = false;
	   ifstream fin("grid.in");
	   ofstream fout("debug.out");
	   fin >> gridWidth >> gridHeight;
	   for (int i = 0; i < 100; i++)
		  isDestroyableBlockActive[i] = false;
	   for (int i = 0; i < 100; i++)
	   {
		  blocksSprites[i].setTexture(block_texture);
		  blocksSprites[i].setScale(50 / 59., 50 / 59.);
		  destroyableBlocksSprites[i].setTexture(destroyableBlock_texture);
		  destroyableBlocksSprites[i].setScale(50 / 76., 50 / 76.);
	   }
	   for (int i = 0; i < 10; i++)
	   {
		  bombs[i].sprite.setTexture(bomb_texture);
		  bombs[i].sprite.setScale(50 / 82., 50 / 82.);
		  bombs[i].active = false;
	   }
	   for (int i = 0; i < 10; i++)
	   {
		  explosions[i].center.setTexture(explosion1_texture);
		  explosions[i].center.setScale(50 / 75., 50 / 75.);
		  explosions[i].innerUp.setTexture(explosion1_texture);
		  explosions[i].innerUp.setScale(50 / 75., 50 / 75.);
		  explosions[i].innerDown.setTexture(explosion1_texture);
		  explosions[i].innerDown.setScale(50 / 75., 50 / 75.);
		  explosions[i].innerLeft.setTexture(explosion1_texture);
		  explosions[i].innerLeft.setScale(50 / 75., 50 / 75.);
		  explosions[i].innerRight.setTexture(explosion1_texture);
		  explosions[i].innerRight.setScale(50 / 75., 50 / 75.);
		  explosions[i].endUp.setTexture(explosion1_texture);
		  explosions[i].endUp.setScale(50 / 75., 50 / 75.);
		  explosions[i].endDown.setTexture(explosion1_texture);
		  explosions[i].endDown.setScale(50 / 75., 50 / 75.);
		  explosions[i].endLeft.setTexture(explosion1_texture);
		  explosions[i].endLeft.setScale(50 / 75., 50 / 75.);
		  explosions[i].endRight.setTexture(explosion1_texture);
		  explosions[i].endRight.setScale(50 / 75., 50 / 75.);
		  bombs[i].active = false;
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
			 else if (grid[i][j] == 'U')
				blocksSprites[nrBlocks++].setPosition(j * 50, i * 50);
			 else if(grid[i][j]=='D')
			 {
				destroyableBlocksSprites[nrDestroyableBlocks].setPosition(j * 50, i * 50);
				isDestroyableBlockActive[nrDestroyableBlocks++] = true;
			 }
			 else if (grid[i][j] == '1')
			 {
				enemy1[++nrEnemy1].active = true;
				enemy1[nrEnemy1].gridX = j;
				enemy1[nrEnemy1].gridY = i;
				enemy1[nrEnemy1].sprite.setTexture(enemy1_texture);
				enemy1[nrEnemy1].sprite.setPosition(j * 50, i * 50);
				enemy1[nrEnemy1].sprite.setScale(50 / 28., 50 / 28.);
			 }
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
		  gameLevelRender(window);
	   }
    }

//Functions

    void gameLevelRender(RenderWindow &window)
    {
	   window.clear(sf::Color::Black);
	   window.draw(backgroundSprite);
	   for (int i = 0; i < 100; i++)
	   {
		  window.draw(blocksSprites[i]);
		  if(isDestroyableBlockActive[i])
		  window.draw(destroyableBlocksSprites[i]);
	   }
	   for (int i = 0; i < 10; i++)
		  if (bombs[i].active)
			 window.draw(bombs[i].sprite);
	   window.draw(playerSprite);
	   for (int i = 0; i < 10; i++)
		  if (enemy1[i].active)
			 window.draw(enemy1[i].sprite);
	   for (int i = 0; i < 10; i++)
		  if (explosions[i].active)
			 //drawExplosion(i);
		  {
			 window.draw(explosions[i].center);
			 window.draw(explosions[i].innerUp);
			 window.draw(explosions[i].innerDown);
			 window.draw(explosions[i].innerLeft);
			 window.draw(explosions[i].innerRight);

			 window.draw(explosions[i].endUp);
			 window.draw(explosions[i].endDown);
			 window.draw(explosions[i].endLeft);
			 window.draw(explosions[i].endRight);
			 
		  }  
	  //window.draw(movement_text);
	  //window.draw(debug);
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

	   playerMoveLeft.texture.loadFromFile("Resources/Textures/bomberman_left.png");
	   playerMoveLeft.looped = true;
	   playerMoveLeft.nFrames = 3;
	   playerMoveLeft.duration = seconds(0.5);

	   playerIdleLeft.texture = playerMoveLeft.texture;
	   playerIdleLeft.looped = false;
	   playerIdleLeft.nFrames = 1;
	   playerIdleLeft.duration = seconds(1);

	   playerMoveRight.texture.loadFromFile("Resources/Textures/bomberman_right.png");
	   playerMoveRight.looped = true;
	   playerMoveRight.nFrames = 3;
	   playerMoveRight.duration = seconds(0.5);

	   playerIdleRight.texture = playerMoveRight.texture;
	   playerIdleRight.looped = false;
	   playerIdleRight.nFrames = 1;
	   playerIdleRight.duration = seconds(1);

	   playerMoveUp.texture.loadFromFile("Resources/Textures/bomberman_up.png");
	   playerMoveUp.looped = true;
	   playerMoveUp.nFrames = 3;
	   playerMoveUp.duration = seconds(0.5);

	   playerIdleUp.texture = playerMoveUp.texture;
	   playerIdleUp.looped = false;
	   playerIdleUp.nFrames = 1;
	   playerIdleUp.duration = seconds(1);

	   playerMoveDown.texture.loadFromFile("Resources/Textures/bomberman_down.png");
	   playerMoveDown.looped = true;
	   playerMoveDown.nFrames = 3;
	   playerMoveDown.duration = seconds(0.5);

	   playerIdleDown.texture = playerMoveDown.texture;
	   playerIdleDown.looped = false;
	   playerIdleDown.nFrames = 1;
	   playerIdleDown.duration = seconds(1);

	   strcpy(playerDirection, "down");
	   currentPlayerAnimation = playerIdleDown;

	   playerSprite.setPosition(playerPosition.x, playerPosition.y);
	   font.loadFromFile("Resources/Fonts/calibri.ttf");
	   movement_text.setFont(font);
    }

void playerUpdate(Time deltaTime)
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

void gameLevelUpdate(Time deltaTime)
{
    playerUpdate(deltaTime);
    bombsUpdate(deltaTime);
    explosionsUpdate(deltaTime);
    enemy1Update(deltaTime);
    
}
void handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::Up)
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
    else if (key == sf::Keyboard::Down)
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
    else if (key == sf::Keyboard::Left)
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
    else if (key == sf::Keyboard::Right)
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
    else if (key == sf::Keyboard::Space && bombCount<bombLimit && grid[playerGridY][playerGridX]!='B')
	   placeBomb();
    else if (key == sf::Keyboard::Escape)
     window.close();
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
    for(int i=0; i<10; i++)
	   if (bombs[i].active)
	   {
		  bombs[i].currentTime+=deltaTime;

		  float scaledTime = (currentTime.asSeconds() / 0.5);
		  int currentFrame = (int)(scaledTime*4);
		  currentFrame %= 4;
		  bombs[i].sprite.setTextureRect(IntRect(currentFrame * 82, 0, 82, 82));  
		  if (bombs[i].currentTime > bombDuration)
		  {
			 bombs[i].active = false;
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
    int p = 0, i, j;
    Vector2i explosionPos;
    explosionPos.x = 0;
    explosionPos.y = 0;
    while (explosions[p].active)
	   p++;
    explosions[p].active = true;
    explosions[p].center.setPosition(posX, posY);
    bool ok;
    for (i = 1; i <= 4; i++) {
	   ok = true;
	   for (j = 1; j <= explosionLength; j++) {
		  explosionPos.x = posX;
		  explosionPos.y = posY;
		  switch (i) {
		  case 1:
		  {
			 explosionPos.y -= j * 50;
			 //debug.setString(debug.getString() + " " + to_string(explosionPos.x) + " " + to_string(explosionPos.y) + "     ");
			 if (j < explosionLength)

			 {
				explosions[p].innerUp.setPosition(explosionPos.x, explosionPos.y);
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

				explosions[p].innerRight.setPosition(explosionPos.x, explosionPos.y);
			 else

				explosions[p].endRight.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  case 3:
		  {
			 explosionPos.y += j * 50;
			 if (j < explosionLength)

				explosions[p].innerDown.setPosition(explosionPos.x, explosionPos.y);
			 else

				explosions[p].endDown.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  case 4:
		  {
			 explosionPos.x -= j * 50;
			 if (j < explosionLength)

				explosions[p].innerLeft.setPosition(explosionPos.x, explosionPos.y);
			 else

				explosions[p].endLeft.setPosition(explosionPos.x, explosionPos.y);
			 break;
		  }
		  }
		  if (grid[explosionPos.y/50][explosionPos.x/50] == 'U') //zid indestructibil
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
				 if (bombs[i].gridX == explosionPos.x / 50 && bombs[i].gridY == explosionPos.y / 50)
				 {
					bombs[i].active = false;
					explode(bombs[i].sprite.getPosition().x, bombs[i].sprite.getPosition().y);
					bombCount--;
					grid[bombs[i].gridY][bombs[i].gridX] = '0';
					bombs[i].currentTime = Time::Zero;
				 }

			  }
		    if (abs(playerSprite.getPosition().x - explosionPos.x) < 50 || abs(playerSprite.getPosition().y - explosionPos.y)<50)
		    {
			   playerSprite.setPosition(playerStartX, playerStartY);
			   playerGridX = playerStartX / 50;
			   playerGridY = playerStartY / 50;
			   playerPosition.x = playerStartX;
			   playerPosition.y = playerStartY;
			   playerDestination.x = playerStartX;
			   playerDestination.y = playerStartY;
			   playerMovement.x = 0;
			   playerMovement.y = 0;
			   playerSpeed = 200;
			   isPlayerMoving = false;
			   currentTime = Time::Zero;
			   strcpy(playerDirection, "down");
			   currentPlayerAnimation = playerIdleDown;			
		    }
		  
	   }
    }
}
    

void explosionsUpdate(sf::Time deltaTime)
{
	 
    int size;
    for (int i = 0; i<10; i++)
	   if (explosions[i].active)
	   {
		  explosions[i].currentTime += deltaTime;
		  float scaledTime = (currentTime.asSeconds() / 0.5);
		  int currentFrame = (int)(scaledTime * 4);
		  currentFrame %= 4;
		  if (currentFrame == 0)
		  {
			 explosions[i].currentTexture = explosion1_texture;
			 size = 75;
		  }
		  else  if (currentFrame == 1)
		  {
			 explosions[i].currentTexture = explosion2_texture;
			 size = 77;
		  }
		  else   if (currentFrame == 2)
		  {
			 explosions[i].currentTexture = explosion3_texture;
			 size = 78;
		  }
		  else  if (currentFrame == 3)
		  {
			 explosions[i].currentTexture = explosion4_texture;
			 size = 80;
		  }
		  explosions[i].endLeft.setTexture(explosions[i].currentTexture);
		  explosions[i].endLeft.setTextureRect(IntRect(0 * size, 0, size, size));
		  explosions[i].innerLeft.setTexture(explosions[i].currentTexture);
		  explosions[i].innerLeft.setTextureRect(IntRect(1 * size, 0, size, size));
		  explosions[i].center.setTexture(explosions[i].currentTexture);
		  explosions[i].center.setTextureRect(IntRect(2 * size, 0, size, size));
		  explosions[i].innerRight.setTexture(explosions[i].currentTexture);
		  explosions[i].innerRight.setTextureRect(IntRect(3 * size, 0, size, size));
		  explosions[i].endRight.setTexture(explosions[i].currentTexture);
		  explosions[i].endRight.setTextureRect(IntRect(4 * size, 0, size, size));
		  explosions[i].innerUp.setTexture(explosions[i].currentTexture);
		  explosions[i].innerUp.setTextureRect(IntRect(5 * size, 0, size, size));
		  explosions[i].endUp.setTexture(explosions[i].currentTexture);
		  explosions[i].endUp.setTextureRect(IntRect(6 * size, 0, size, size));
		  explosions[i].innerDown.setTexture(explosions[i].currentTexture);
		  explosions[i].innerDown.setTextureRect(IntRect(7 * size, 0, size, size));
		  explosions[i].endDown.setTexture(explosions[i].currentTexture);
		  explosions[i].endDown.setTextureRect(IntRect(8 * size, 0, size, size));
		  if (explosions[i].currentTime > explosionDuration)
			 explosions[i].active = false;
	   }
}
void drawExplosion(int i)
{
    window.draw(explosions[i].endLeft);
    window.draw(explosions[i].innerLeft);
    window.draw(explosions[i].center);
    window.draw(explosions[i].innerRight);
    window.draw(explosions[i].endRight);
    window.draw(explosions[i].innerUp);
    window.draw(explosions[i].endUp);
    window.draw(explosions[i].innerDown);
    window.draw(explosions[i].endDown);
}
void enemy1Update(Time deltaTime)
{
    for(int i=0; i<10; i++)
	   if (enemy1[i].active)
	   {
		  enemy1[i].currentTime += deltaTime;
		  float scaledTime = (currentTime.asSeconds() / 2.);
		  int currentFrame = (int)(scaledTime * 13);
		  currentFrame %= 13;
		  enemy1[i].sprite.setTextureRect(IntRect(currentFrame * 28, 0, 28, 28));
	   }
}