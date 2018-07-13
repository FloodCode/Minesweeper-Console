#include "minesweeper.h"

Cell::Cell()
{
	this->type = CellType::empty;
	this->state = CellState::closed;
}

Minesweeper::Point::Point(int row, int col)
{
	this->row = row;
	this->col = col;
}

bool Minesweeper::Point::operator==(const Point point)
{
	return (this->row == point.row && this->col == point.col);
}

Minesweeper::Minesweeper()
{
	newGame(16, 16, 40);
}

Minesweeper::Minesweeper(int rows, int cols, int mines)
{
	newGame(rows, cols, mines);
}

Minesweeper::~Minesweeper()
{
	clear();
}

Minesweeper &Minesweeper::operator=(const Minesweeper &sourceObj)
{
	newGame(sourceObj.rows, sourceObj.cols, sourceObj.minesTotal);
	return *this;
}

// Initialize object attributes with pre-validation
void Minesweeper::newGame(int rows, int cols, int mines)
{
	this->rows = -1;
	this->cols = -1;
	clear();
	this->openedCells.clear();
	this->gameState = GameState::newgame;
	this->rows = rows < 1 ? 4 : rows > 30 ? 30 : rows;
	this->cols = cols < 1 ? 4 : cols > 30 ? 30 : cols;
	this->minesTotal = mines < 1 ? 1 : mines > (rows * cols * 0.9) ? int(rows * cols * 0.9) : mines;
	this->cellsCount = rows * cols;
	this->openedCount = 0;
	this->flagsCount = 0;
	this->minefield = 0;
	this->timeStart = 0;

	generateMinefield(-1, -1);
}

// Allocate memory for minefield.
// Place given amount of mines.
// Generate hints for every cell excluding mines.
void Minesweeper::generateMinefield(int excludeRow, int excludeCol)
{
	if (minefield == 0)
	{
		minefield = new Cell*[rows];
		for (int i = 0; i < rows; i++)
		{
			minefield[i] = new Cell[cols];
		}
	}

	srand(time(0));
	int minesCount = 0, rowN, colN;
	while (minesCount != minesTotal)
	{
		rowN = rand() % rows;
		colN = rand() % cols;
		if (rowN == excludeRow && colN == excludeCol)
		{
			continue;
		}
		if (minefield[rowN][colN].type != CellType::mine)
		{
			minefield[rowN][colN].type = CellType::mine;
			minesCount++;
		}
	}

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (minefield[row][col].type != CellType::mine)
			{
				minefield[row][col].type = getHint(Point(row, col));
			}
		}
	}
}

// Returns true if given cell is mine
bool Minesweeper::isMine(Point crd)
{
	if (crd.col < 0 || crd.col >= cols || crd.row < 0 || crd.row >= rows)
	{
		return false;
	}
	return (minefield[crd.row][crd.col].type == CellType::mine);
}

// Returns true if openCells vector contains given cell
bool Minesweeper::isInOpened(Point crd)
{
	for (auto p = openedCells.begin(); p != openedCells.end(); p++) {
		if (*p == crd) return true;
	}

	return false;
}

// Opens given cell
void Minesweeper::openCell(int row, int col)
{
	if (gameState == GameState::newgame)
	{
		if (minefield[row][col].type == CellType::mine)
		{
			generateMinefield(row, col);
		}
		gameState = GameState::running;
		timeStart = clock();
	}

	Cell *cell = &minefield[row][col];

	if (cell->state == CellState::flagged)
	{
		return;
	}

	if (cell->type == CellType::mine)
	{
		defeat();
	}
	else if (cell->type == CellType::empty)
	{
		std::vector<Point> queue;
		queue.push_back(Point(row, col));
		while (queue.size() > 0)
		{
			std::vector<Point> newQueue;
			for (size_t i = 0; i < queue.size(); i++)
			{
				if (!isInOpened(queue[i]))
				{
					std::vector<Point> near = openSquare(queue[i]);
					openedCells.push_back(queue[i]);
					for (auto p = near.begin(); p != near.end(); p++) {
						if (!isInOpened(*p)) newQueue.push_back(*p);
					}
				}
			}
			queue = newQueue;
		}
	}
	else
	{
		forceOpen(Point(row, col));
	}
}

// Returns hint for given cell
CellType Minesweeper::getHint(Point crd)
{
	int result = 0;
	if (isMine(Point(crd.row + 1, crd.col + 1))) result++;
	if (isMine(Point(crd.row + 1, crd.col - 1))) result++;
	if (isMine(Point(crd.row - 1, crd.col + 1))) result++;
	if (isMine(Point(crd.row - 1, crd.col - 1))) result++;
	if (isMine(Point(crd.row, crd.col + 1))) result++;
	if (isMine(Point(crd.row, crd.col - 1))) result++;
	if (isMine(Point(crd.row + 1, crd.col))) result++;
	if (isMine(Point(crd.row - 1, crd.col))) result++;
	return CellType(result);
}

// Opens every neighbour of given cell
std::vector<Minesweeper::Point> Minesweeper::openSquare(Point crd)
{
	std::vector<Point> result;
	forceOpen(crd);
	if (forceOpen(Point(crd.row - 1, crd.col - 1)))
		result.push_back(Point(crd.row - 1, crd.col - 1));
	if (forceOpen(Point(crd.row, crd.col - 1)))
		result.push_back(Point(crd.row, crd.col - 1));
	if (forceOpen(Point(crd.row - 1, crd.col)))
		result.push_back(Point(crd.row - 1, crd.col));
	if (forceOpen(Point(crd.row + 1, crd.col + 1)))
		result.push_back(Point(crd.row + 1, crd.col + 1));
	if (forceOpen(Point(crd.row, crd.col + 1)))
		result.push_back(Point(crd.row, crd.col + 1));
	if (forceOpen(Point(crd.row + 1, crd.col)))
		result.push_back(Point(crd.row + 1, crd.col));
	if (forceOpen(Point(crd.row - 1, crd.col + 1)))
		result.push_back(Point(crd.row - 1, crd.col + 1));
	if (forceOpen(Point(crd.row + 1, crd.col - 1)))
		result.push_back(Point(crd.row + 1, crd.col - 1));
	return result;
}

bool Minesweeper::cellExist(Point crd)
{
	return (crd.row < rows && crd.row >= 0 && crd.col < cols && crd.col >= 0);
}

void Minesweeper::flagCell(int row, int col)
{
	if (gameState == GameState::running)
	{
		Cell *cell = &minefield[row][col];
		switch (cell->state)
		{
		case CellState::closed:
			cell->state = flagged;
			flagsCount++;
			break;
		case CellState::flagged:
			cell->state = closed;
			flagsCount--;
			break;
		}
	}
}

bool Minesweeper::forceOpen(Point crd)
{
	if (cellExist(crd) &&
		minefield[crd.row][crd.col].state == CellState::closed &&
		(gameState == GameState::running || gameState == GameState::newgame))
	{
		minefield[crd.row][crd.col].state = CellState::opened;
		this->openedCount++;
		if (minesTotal == cellsCount - openedCount)
		{
			victory();
		}
		return minefield[crd.row][crd.col].type == CellType::empty;
	}
	return false;
}

void Minesweeper::victory()
{
	gameState = GameState::win;
	flagsCount = 0;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (minefield[row][col].type == CellType::mine)
			{
				minefield[row][col].state = CellState::flagged;
			}
		}
	}
}

void Minesweeper::defeat()
{
	gameState = GameState::loose;
	flagsCount = 0;
	openedCells.clear();

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (minefield[row][col].type == CellType::mine)
			{
				minefield[row][col].state = CellState::opened;
			}
		}
	}
}

bool Minesweeper::cellExist(int row, int col)
{
	return cellExist(Point(row, col));
}

int Minesweeper::getCols()
{
	return cols;
}

int Minesweeper::getRows()
{
	return rows;
}

// Returns amount of seconds since first cell was opened
int Minesweeper::getTime()
{
	return timeStart == 0 ? 0 : int((clock() - timeStart) / CLOCKS_PER_SEC);
}

int Minesweeper::getMinesLeft()
{
	return minesTotal - flagsCount;
}

Cell **Minesweeper::getMinefield()
{
	return minefield;
}

GameState Minesweeper::getGameState()
{
	return gameState;
}

// Returns ASCII representation of minefield
std::string Minesweeper::getString()
{
	std::stringstream result;
	Cell current;
	char cellSymbol;
	result << "    ";
	for (int col = 0; col < cols; col++)
	{
		result << col + 1;
		if (col + 1 < 10)
		{
			result << ' ';
		}
	}
	result << std::endl << std::endl;
	for (int row = 0; row < rows; row++)
	{
		result << row + 1;
		if (row + 1 < 10)
		{
			 result << "   ";
		}
		else
		{
			result << "  ";
		}
		for (int col = 0; col < cols; col++)
		{
			current = minefield[row][col];
			switch (current.state)
			{
			case CellState::closed: cellSymbol = '-'; break;
			case CellState::flagged: cellSymbol = 'X'; break;
			case CellState::opened:
				switch (current.type)
				{
				case CellType::empty: cellSymbol = ' '; break;
				case CellType::mine: cellSymbol = '*'; break;
				case CellType::n1: cellSymbol = '1'; break;
				case CellType::n2: cellSymbol = '2'; break;
				case CellType::n3: cellSymbol = '3'; break;
				case CellType::n4: cellSymbol = '4'; break;
				case CellType::n5: cellSymbol = '5'; break;
				case CellType::n6: cellSymbol = '6'; break;
				case CellType::n7: cellSymbol = '7'; break;
				case CellType::n8: cellSymbol = '8'; break;
				}
				break;
			}
			result << cellSymbol << ' ';
		}
		result << std::endl;
	}
	result << "Time: " << getTime() << "s" << std::endl;
	result << "Mines: " << getMinesLeft() << std::endl;
	return result.str();
}

// Frees memory allocated for minefield
void Minesweeper::clear()
{
	if (rows != -1 && cols != -1)
	{
		for (int row = 0; row < rows; row++)
		{
			delete[]minefield[row];
		}
		delete[]minefield;
	}
}