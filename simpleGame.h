#pragma once
namespace GAME_CONST
{
	const unsigned short mapSize = 20;
	const unsigned short fovSizer = 2;
	const unsigned short fovSize = fovSizer * 2 + 1;
	const char FOOD = '@';//jedzenie
	const char PLAYER = '&';//gracz
	const char TILE = '.';//kafelek
}
class Player;
class Game
{
private:
	void spawnFood();
public:
	Game();
	char map[GAME_CONST::mapSize][GAME_CONST::mapSize];
	void restart();
	void viewGame();
	void update(Player*);
};
class Player
{
	Game* game;
public:
	int health;
	int positionX;
	int positionY;
	char fov[GAME_CONST::fovSize][GAME_CONST::fovSize];
	Player();
	Player(Game*);
	void restart();
	void up();
	void down();
	void left();
	void right();
	void viewStats();
	void viewFov();
	void update(bool foodEaten);
	
};