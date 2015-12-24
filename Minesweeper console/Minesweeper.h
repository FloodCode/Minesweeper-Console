#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
enum GameState
{
	newgame,
	win,
	loose,
	running
};
enum CellType
{
	empty = 0,
	n1 = 1,
	n2 = 2,
	n3 = 3,
	n4 = 4,
	n5 = 5,
	n6 = 6,
	n7 = 7,
	n8 = 8,
	mine = 9
};
enum CellState
{
	opened,
	closed,
	flagged
};
struct Cell
{
	Cell();
	CellType type;
	CellState state;
};
class Minesweeper
{
private:
	struct Point
	{
		Point(int row, int col);
		bool operator==(const Point point);
		int col;
		int row;
	};

	int rows, cols, minesTotal, openedCount, flagsCount, cellsCount;
	time_t timeStart;
	GameState gameState;
	std::vector<Point> openedCells;
	Cell **minefield;

	void victory();
	void defeat();
	void generateMinefield(int excludeRow, int excludeCol);
	bool forceOpen(Point crd);
	bool isMine(Point crd);
	bool isInOpened(Point crd);
	bool cellExist(Point crd);
	CellType getHint(Point crd);
	std::vector<Point> openSquare(Point crd);
public:
	Minesweeper();
	Minesweeper(int rows, int cols, int mines);
	~Minesweeper();
	Minesweeper &operator=(const Minesweeper &sourceObj);
	void newGame(int rows, int cols, int mines);
	void openCell(int row, int col);
	void flagCell(int row, int col);
	void clear();
	bool cellExist(int row, int col);
	int getCols();
	int getRows();
	int getTime();
	int getMinesLeft();
	Cell **getMinefield();
	GameState getGameState();
	std::string getString();
};
#endif // !MINESWEEPER_H