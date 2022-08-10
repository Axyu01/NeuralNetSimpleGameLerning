#include <iostream>

//#include <ctime>

#include "simpleGame.h"
Player::Player()
	{
		game = new Game();
		restart();
	}
Player::Player(Game* g)
	{
		game = g;
		restart();
	}
void Player::restart()
	{
		positionX = 0;
		positionY = 0;
		health = 30;
		for (unsigned short y = 0; y < GAME_CONST::fovSize; y++)
		{
			for (unsigned short x = 0; x < GAME_CONST::fovSize; x++)
				fov[y][x] = game->map[(positionY + y) % GAME_CONST::mapSize][(positionX + x) % GAME_CONST::mapSize];
		}
	}
void Player::up()
	{
		positionY--;
		game->update(this);

	}
void Player::down()
	{
		positionY++;
		game->update(this);
	}
void Player::left()
	{
		positionX--;
		game->update(this);
	}
void Player::right()
	{
		positionX++;
		game->update(this);
	}
void Player::viewStats()
	{
		std::cout << "X:" << positionX << std::endl;
		std::cout << "Y:" << positionY << std::endl;
		std::cout << "HP:" << health << std::endl;
	}
void Player::viewFov()
	{
		std::cout << std::endl;
		for (unsigned short y = 0; y < GAME_CONST::fovSize; y++)
		{
			for (unsigned short x = 0; x < GAME_CONST::fovSize; x++)
				std::cout << fov[y][x];
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
void Player::update(bool foodEaten)
	{
		if (foodEaten)
		{
			health += 10;
			if (health > 100)
			{
				health = 100;
			}
		}
		else
		{
			health--;
			if (health < 0)
			{
				game->restart();
				restart();
			}
		}
		for (unsigned short y = 0; y < GAME_CONST::fovSize; y++)
		{
			for (unsigned short x = 0; x < GAME_CONST::fovSize; x++)
				fov[y][x] = game->map[(positionY + y + GAME_CONST::mapSize - GAME_CONST::fovSizer) % GAME_CONST::mapSize]
				[(positionX + x + GAME_CONST::mapSize - GAME_CONST::fovSizer) % GAME_CONST::mapSize];
		}
	}
void Game::spawnFood()
{
	srand(time(NULL));
	bool lookFor = true;
	short x = rand() % GAME_CONST::mapSize;
	short y = rand() % GAME_CONST::mapSize;
	while (lookFor)
	{
		if (map[y][x] == GAME_CONST::TILE)
		{
			map[y][x] = GAME_CONST::FOOD;
			lookFor = false;
		}
		else
		{
			x = rand() % GAME_CONST::mapSize;
			y = rand() % GAME_CONST::mapSize;
		}
	}
}
void Game::restart()
{
	srand(time(NULL));
	for (unsigned short y = 0; y < GAME_CONST::mapSize; y++)
	{
		for (unsigned short x = 0; x < GAME_CONST::mapSize; x++)
		{
			map[y][x] = GAME_CONST::TILE;
		}
	}
	for (unsigned short i = 0; i < GAME_CONST::mapSize; i++)
	{
		spawnFood();
	}
}
Game::Game()
{
	this->restart();
}
void Game::viewGame()
{
	std::cout << std::endl;
	for (unsigned short y = 0; y < GAME_CONST::mapSize; y++)
	{
		for (unsigned short x = 0; x < GAME_CONST::mapSize; x++)
			std::cout << map[y][x];
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void Game::update(Player* player)
{
	player->positionX = (GAME_CONST::mapSize + player->positionX) % GAME_CONST::mapSize;
	player->positionY = (GAME_CONST::mapSize + player->positionY) % GAME_CONST::mapSize;
	int positionX = player->positionX % GAME_CONST::mapSize;
	int positionY = player->positionY % GAME_CONST::mapSize;

	//clearing previus PLAYER marker
	if (map[(positionY + 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize][positionX] == GAME_CONST::PLAYER)
	{
		map[(positionY + 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize][positionX] = GAME_CONST::TILE;
	}
	else if (map[(positionY - 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize][positionX] == GAME_CONST::PLAYER)
	{
		map[(positionY - 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize][positionX] = GAME_CONST::TILE;
	}
	else if (map[positionY][(positionX + 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize] == GAME_CONST::PLAYER)
	{
		map[positionY][(positionX + 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize] = GAME_CONST::TILE;
	}
	else if (map[positionY][(positionX - 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize] == GAME_CONST::PLAYER)
	{
		map[positionY][(positionX - 1 + GAME_CONST::mapSize) % GAME_CONST::mapSize] = GAME_CONST::TILE;
	}

	if (map[positionY][positionX] == GAME_CONST::FOOD)
	{
		map[positionY][positionX] = GAME_CONST::PLAYER;
		spawnFood();
		player->update(true);
	}
	else
	{
		map[positionY][positionX] = GAME_CONST::PLAYER;
		player->update(false);
	}
}