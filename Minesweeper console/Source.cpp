#include <iostream>
#include "Minesweeper.h"

using namespace std;

Minesweeper minesweeper(10, 10, 30);

int gameMenu()
{
	cout << "1. Open cell" << endl;
	cout << "2. Flag cell" << endl;
	int answer = -1;
	while (answer != 1 && answer != 2)
	{
		cin >> answer;
	}
	return answer;
}

int main()
{
	int answer;
	cout << minesweeper.getString();
	while (true)
	{
		answer = gameMenu();
		int row, col;
		cout << "x: ";
		cin >> col;
		cout << "y: ";
		cin >> row;
		if (minesweeper.cellExist(row - 1, col - 1))
		{
			if (answer == 1)
			{
				minesweeper.openCell(row - 1, col - 1);
			}
			else if (answer == 2)
			{
				minesweeper.flagCell(row - 1, col - 1);
			}
		}
		system("cls");
		cout << minesweeper.getString();
	}
	system("pause");
	return 0;
}