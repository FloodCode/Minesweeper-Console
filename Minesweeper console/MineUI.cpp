#include "MineUI.h"

void MineUI::Start()
{
	mainMenu();
}

void MineUI::mainMenu()
{
	bool repeat = true;
	while (repeat)
	{
		system("cls");
		std::cout << "Minesweeper v" << MINE_VER << std::endl;
		std::cout << "Menu:" << std::endl;
		std::cout << "\t1. New game" << std::endl;
		std::cout << "\t0. Exit" << std::endl;
		int answer = readInt("> ", 0, 1);
		switch (answer)
		{
		case 1:
			newGame();
			break;
		case 0:
			std::cout << "Goodbye!" << std::endl;
			system("pause");
			repeat = false;
			break;
		}
	}
}

void MineUI::newGame()
{
	system("cls");
	std::cout << "Dificulty:" << std::endl;
	std::cout << "\t1. Beginner" << std::endl;
	std::cout << "\t2. Intermediate" << std::endl;
	std::cout << "\t3. Expert" << std::endl;
	std::cout << "\t4. Custom" << std::endl;
	int answer = readInt("> ", 1, 4);
	switch (answer)
	{
	case 1:
		minesweeper = Minesweeper(9, 9, 10);
		break;
	case 2:
		minesweeper = Minesweeper(16, 16, 40);
		break;
	case 3:
		minesweeper = Minesweeper(16, 30, 99);
		break;
	case 4:
		system("cls");
		int w, h, mines;
		std::cout << "Width:" << std::endl;
		w = readInt("> ", 4, 30);
		std::cout << "Height:" << std::endl;
		h = readInt("> ", 4, 30);
		std::cout << "Mines count:" << std::endl;
		mines = readInt("> ", 1, int(w * h * 0.9));
		minesweeper = Minesweeper(h, w, mines);
		break;
	}
	runGame();
}

void MineUI::runGame()
{
	GameState gameState;
	while ((gameState = minesweeper.getGameState()) == GameState::running)
	{
		system("cls");
		std::cout << minesweeper.getString();
		std::cout << "1. Open cell" << std::endl;
		std::cout << "2. Flag/unflag cell" << std::endl;
		int answer = readInt("> ", 1, 2);
		int col = readInt("x: ", 1, minesweeper.getWidth());
		int row = readInt("y: ", 1, minesweeper.getHeight());
		if (answer == 1)
		{
			minesweeper.openCell(row - 1, col - 1);
		}
		else
		{
			minesweeper.flagCell(row - 1, col - 1);
		}
	}
	system("cls");
	switch (gameState)
	{
	case GameState::loose:
		std::cout << "You loose!" << std::endl;
		break;
	case GameState::win:
		std::cout << "You won!" << std::endl;
		break;
	}
	system("pause");
}

int MineUI::readInt(const char *mask, int min, int max)
{
	while (true)
	{
		std::cout << mask;
		std::string answer;
		std::getline(std::cin, answer);
		int answerInt = toInt(answer);
		if (answerInt >= min && answerInt <= max)
		{
			return answerInt;
		}
		std::cout << "Wrong input!" << std::endl;
	}
}

int MineUI::toInt(std::string str)
{
	if (str.length() >= 10) return -1;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] < 48 || str[i] > 57) return -1;
	}
	long int result = strtol(str.c_str(), 0, 10);
	return int(result);
}