#ifndef MINEUI_H
#define MINEUI_H
#define MINE_VER "0.0.3"
#include <iostream>
#include <string>
#include "minesweeper.h"
class MineUI
{
private:
	int cursorRow, cursorCol;
	Minesweeper minesweeper;
	void mainMenu();
	void newGame();
	void runGame();
	void render();
	int readInt(const char *mask, int min, int max);
	int toInt(std::string str);
public:
	void Start();
};
#endif // !MINEUI_H
