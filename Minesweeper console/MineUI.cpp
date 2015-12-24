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
		w = readInt("Width: ", 4, 30);
		h = readInt("Height: ", 4, 30);
		mines = readInt("Mines: ", 1, int(w * h * 0.9));
		minesweeper = Minesweeper(h, w, mines);
		break;
	}
	cursorRow = 0;
	cursorCol = 0;
	runGame();
}

void MineUI::runGame()
{
	bool exit = false;
	render();
	do
	{
		int keyCode = getch();
		switch (keyCode)
		{
		case 72:
			// Up arrow
			if (cursorRow > 0) cursorRow--;
			break;
		case 80:
			// Down arrow
			if (cursorRow < minesweeper.getRows() - 1) cursorRow++;
			break;
		case 75:
			// Left arrow
			if (cursorCol > 0) cursorCol--;
			break;
		case 77:
			// Right arrow
			if (cursorCol < minesweeper.getCols() - 1) cursorCol++;
			break;
		case 13:
			// Enter
			minesweeper.openCell(cursorRow, cursorCol);
			if (minesweeper.getGameState() == GameState::loose || minesweeper.getGameState() == GameState::win)
			{
				exit = true;
			}
			break;
		case 27:
			// Esc
			exit = true;
			break;
		case 83:
			minesweeper.flagCell(cursorRow, cursorCol);
			break;
		default:
			break;
		}
		render();
	} while (!exit);
	switch (minesweeper.getGameState())
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

void MineUI::render()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Cell **minefield = minesweeper.getMinefield();
	Cell current;
	char cellSymbol;
	int color;
	system("cls");
	for (int row = 0; row < minesweeper.getRows(); row++)
	{
		for (int col = 0; col < minesweeper.getCols(); col++)
		{
			current = minefield[row][col];
			color = 7;
			switch (current.state)
			{
			case CellState::closed: cellSymbol = '-'; break;
			case CellState::flagged: cellSymbol = 'F'; color = 127; break;
			case CellState::opened:
				switch (current.type)
				{
				case CellType::empty: cellSymbol = ' '; color = 112; break;
				case CellType::mine: cellSymbol = '*'; color = 124; break;
				case CellType::n1: cellSymbol = '1'; color = 121; break;
				case CellType::n2: cellSymbol = '2'; color = 114; break;
				case CellType::n3: cellSymbol = '3'; color = 124; break;
				case CellType::n4: cellSymbol = '4'; color = 113; break;
				case CellType::n5: cellSymbol = '5'; color = 116; break;
				case CellType::n6: cellSymbol = '6'; color = 115; break;
				case CellType::n7: cellSymbol = '7'; color = 112; break;
				case CellType::n8: cellSymbol = '8'; color = 120; break;
				}
				break;
			}
			SetConsoleTextAttribute(hConsole, color);
			if (cursorRow != row || cursorCol != col)
			{
				std::cout << ' ' << cellSymbol << ' ';
			}
			else
			{
				std::cout << '<' << cellSymbol << '>';
			}
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Time: " << minesweeper.getTime() << "s" << std::endl;
	std::cout << "Mines: " << minesweeper.getMinesLeft() << std::endl;
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