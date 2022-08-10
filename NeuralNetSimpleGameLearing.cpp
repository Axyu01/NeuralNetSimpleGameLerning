// NeuralNetSimpleGameLearing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <ctime>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <conio.h>
#include "simpleGame.h"
#include "neuralNets.h"
int modulo(int num)
{
	if (num < 0)
		return -num;
	else
		return num;
};
int main()
{
	/*const unsigned int howManyIterations = 100;
	bool a;
	bool b;
	srand(time(NULL));
	while(true)
	{
		if (rand() % 2 == 1)
			a = true;
		else
			a = false;
		if (rand() % 2 == 1)
			b = true;
		else
			b = false;
		std::cout << "a:" << a << " b:" << b<<std::endl;
		std::cout << "------------" << std::endl;
		_getch();
	}*/
	//std::cout << "a:" << allA << " b:" << allB << " allTrues:" << allTrues << "/" << howManyIterations;
	//return 0;
	/*MPList<int> list;
	list.addKey(1);
	ist.addKey(2);
	list.addKey(3);
	list.resetPointerKey();
	std::cout <<list.getPtrKey()<< std::endl;
	list.incrementPointer();
	std::cout << list.getPtrKey() << std::endl;
	list.incrementPointer();
	std::cout << list.getPtrKey() << std::endl;
	list.incrementPointer();*/
	Game* game = new Game();
	Player* player = new Player(game);
	LayerNet* neuralNet=new LayerNet(GAME_CONST::fovSize* GAME_CONST::fovSize,0, GAME_CONST::fovSize * GAME_CONST::fovSize,4,0.00001);
	
	char choice='w';
	srand(time(NULL));
	float inputArray[GAME_CONST::fovSize * GAME_CONST::fovSize];
	float desiredOutput[4];
	while(choice=='w'|| choice == 's' || choice == 'a' || choice == 'd')
	{
		
		choice = _getch();
		system("cls");
		for(unsigned int i = 0;i<GAME_CONST::fovSize;i++)
			for (unsigned int j = 0; j < GAME_CONST::fovSize; j++)
			{
				if(player->fov[i][j]==GAME_CONST::FOOD)
					inputArray[i * GAME_CONST::fovSize + j] = 1;
				else
					inputArray[i * GAME_CONST::fovSize + j] = 0;
			}
		switch (choice)
		{
		case 'w':
			{
			player->up();
			desiredOutput[0] = 1;
			desiredOutput[1] = 0;
			desiredOutput[2] = 0;
			desiredOutput[3] = 0;
			}
		break;
		case 's':
		{
			player->down();
			desiredOutput[0] = 0;
			desiredOutput[1] = 1;
			desiredOutput[2] = 0;
			desiredOutput[3] = 0;
		}
		break;
		case 'a':
		{
			player->left();
			desiredOutput[0] = 0;
			desiredOutput[1] = 0;
			desiredOutput[2] = 1;
			desiredOutput[3] = 0;
		}
		break;
		case 'd':
		{
			player->right();
			desiredOutput[0] = 0;
			desiredOutput[1] = 0;
			desiredOutput[2] = 0;
			desiredOutput[3] = 1;
		}
		break;
		default:
			break;
		}
		//game->viewGame();
		player->viewStats();
		player->viewFov();
		neuralNet->setInput(inputArray);
		neuralNet->calculateNet();
		std::cout << "ErrorFunction: " << neuralNet->lernByExample(desiredOutput) << std::endl;
		neuralNet->giveOutput(desiredOutput);
		std::cout << "Up:" << desiredOutput[0] << std::endl;
		std::cout << "Down:" << desiredOutput[1] << std::endl;
		std::cout << "Left:" << desiredOutput[2] << std::endl;
		std::cout << "Right:" << desiredOutput[3] << std::endl;
	}
	if (choice == 'r')
	{
		while (true)
		{
			{
				int random = rand() % 4;
				switch (random)
				{
				case 0:
					choice = 'w';
					break;
				case 1:
					choice = 's';
					break;
				case 2:
					choice = 'a';
					break;
				case 3:
					choice = 'd';
					break;
				}
				Sleep(400);
			}
			system("cls");
			switch (choice)
			{
			case 'w':
			{
				player->up();
			}
			break;
			case 's':
			{
				player->down();
			}
			break;
			case 'a':
			{
				player->left();
			}
			break;
			case 'd':
			{
				player->right();
			}
			break;
			default:
				break;
			}
			game->viewGame();
			player->viewStats();
			player->viewFov();
		}
	}
	if (choice == 'l')
	{
		for (unsigned int sample=0; sample < 1000000; sample++)
		{
			if (sample % 10000==0)
			{
				std::cout << 100*sample/1000000<<"% done" << std::endl;
			}
			int xoffset= GAME_CONST::fovSizer;
			int yoffset = GAME_CONST::fovSizer;

			for (int i = 0; i < GAME_CONST::fovSize; i++)
				for (int j = 0; j < GAME_CONST::fovSize; j++)
				{
					if (player->fov[i][j] == GAME_CONST::FOOD)
					{
						inputArray[i * GAME_CONST::fovSize + j] = 1;
						if (modulo(xoffset) + modulo(yoffset) > modulo(i- GAME_CONST::fovSizer) + modulo(j- GAME_CONST::fovSizer))
						{
							yoffset = i- GAME_CONST::fovSizer;
							xoffset = j- GAME_CONST::fovSizer;
						}
					}
					else
						inputArray[i * GAME_CONST::fovSize + j] = 0;
				}
			//::cout << "xoff: " << xoffset << "yoff: " << yoffset << std::endl;
			for (int k = 0; k < 4; k++)
				desiredOutput[k] = 0;
			if (modulo(xoffset) < modulo(yoffset))
			{
				if (yoffset > 0)
				{
					desiredOutput[1] = 1;
					//choice = 's';
				}
				else
				{
					desiredOutput[0] = 1;
					//choice = 'w';
				}
			}
			else
			{
				if (xoffset > 0)
				{
					desiredOutput[3] = 1;
					//choice = 'd';
				}
				else
				{
					desiredOutput[2] = 1;
					//choice = 'a';
				}
			}
			neuralNet->setInput(inputArray);
			neuralNet->calculateNet();
			neuralNet->lernByExample(desiredOutput);
			{
				unsigned int biggest = 0;

				for (unsigned int i = 1; i < 4; i++)
				{
					if (desiredOutput[biggest] < desiredOutput[i])
						biggest = i;
				}
				switch (biggest)
				{
				case 0:
					choice = 'w';
					break;
				case 1:
					choice = 's';
					break;
				case 2:
					choice = 'a';
					break;
				case 3:
					choice = 'd';
					break;
				}
			}
			switch (choice)
			{
			case 'w':
			{
				player->up();
			}
			break;
			case 's':
			{
				player->down();
			}
			break;
			case 'a':
			{
				player->left();
			}
			break;
			case 'd':
			{
				player->right();
			}
			break;
			default:
				break;
			}
		}
		while (true)
		{
			for (unsigned int i = 0; i < GAME_CONST::fovSize; i++)
				for (unsigned int j = 0; j < GAME_CONST::fovSize; j++)
				{
					if (player->fov[i][j] == GAME_CONST::FOOD)
						inputArray[i * GAME_CONST::fovSize + j] = 1;
					else
						inputArray[i * GAME_CONST::fovSize + j] = 0;
				}
			neuralNet->setInput(inputArray);
			neuralNet->calculateNet();
			neuralNet->giveOutput(desiredOutput);
			std::cout << "Up:" << desiredOutput[0] << std::endl;
			std::cout << "Down:" << desiredOutput[1] << std::endl;
			std::cout << "Left:" << desiredOutput[2] << std::endl;
			std::cout << "Right:" << desiredOutput[3] << std::endl;
			{
				unsigned int biggest = 0;

				for (unsigned int i = 1; i < 4; i++)
				{
					if (desiredOutput[biggest] < desiredOutput[i])
						biggest = i;
				}
				switch (biggest)
				{
				case 0:
					choice = 'w';
					break;
				case 1:
					choice = 's';
					break;
				case 2:
					choice = 'a';
					break;
				case 3:
					choice = 'd';
					break;
				}
				Sleep(150);
			}
			system("cls");
			switch (choice)
			{
			case 'w':
			{
				player->up();
			}
			break;
			case 's':
			{
				player->down();
			}
			break;
			case 'a':
			{
				player->left();
			}
			break;
			case 'd':
			{
				player->right();
			}
			break;
			default:
				break;
			}
			game->viewGame();
			player->viewStats();
			player->viewFov();
		}
	}
	if (choice == 'h')
	{
		while (true)
		{
			{
					int way = 0;
					int xoffset = GAME_CONST::fovSizer+1;
					int yoffset = GAME_CONST::fovSizer+1;

					for (int i = 0; i < GAME_CONST::fovSize; i++)
						for (int j = 0; j < GAME_CONST::fovSize; j++)
						{
							if (player->fov[i][j] == GAME_CONST::FOOD)
							{
								if (modulo(xoffset) + modulo(yoffset) > modulo(i - (int)GAME_CONST::fovSizer) + modulo(j - (int)GAME_CONST::fovSizer))
								{
									yoffset = i - (int)GAME_CONST::fovSizer;
									xoffset = j - (int)GAME_CONST::fovSizer;
								}
							}
						}
					std::cout << "xoff: " << xoffset << "yoff: " << yoffset <<std::endl;
					if (modulo(xoffset) < modulo(yoffset))
					{
						if (yoffset > 0)
						{
							way = 1;
						}
						else
						{
							way = 0;
						}
					}
					else
					{
						if (xoffset > 0)
						{
							way = 3;
						}
						else
						{
							way = 2;
						}
					}
					switch (way)
					{
					case 0:
						choice = 'w';
						break;
					case 1:
						choice = 's';
						break;
					case 2:
						choice = 'a';
						break;
					case 3:
						choice = 'd';
						break;
					}
					Sleep(400);
					//_getch();
				}
				system("cls");
				switch (choice)
				{
				case 'w':
				{
					player->up();
				}
				break;
				case 's':
				{
					player->down();
				}
				break;
				case 'a':
				{
					player->left();
				}
				break;
				case 'd':
				{
					player->right();
				}
				break;
				default:
					break;
				}
				game->viewGame();
				player->viewStats();
				player->viewFov();
		}
	}
	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
