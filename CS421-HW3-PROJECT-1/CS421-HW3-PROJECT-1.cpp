// CS 421 HW3: Multithreaded Sudoku Validator
// Guan Ce Looa

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// Structure to pass parameters to each thread
typedef struct {
	int row;
	int column;
	int (*board)[9];
} parameters;

// 9 threads to check each column
// 9 threads to check each row
// 9 threads to check each 3x3 subgrid
// 27 threads total
const int threadConstant = 27;
// Global array to keep track of each thread result
int validator[threadConstant] = { 0 };

void checkColumn(void *p);
void checkRow(void *p);
void checkGrid(void *p);

int main()
{
	// Valid board
	int sudokuValid[9][9] = {
		{6, 2, 4, 5, 3, 9, 1, 8, 7},
		{5, 1, 9, 7, 2, 8, 6, 3, 4},
		{8, 3, 7, 6, 1, 4, 2, 9, 5},
		{1, 4, 3, 8, 6, 5, 7, 2, 9},
		{9, 5, 8, 2, 4, 7, 3, 6, 1},
		{7, 6, 2, 3, 9, 1, 4, 5, 8},
		{3, 7, 1, 9, 5, 6, 8, 4, 2},
		{4, 9, 6, 1, 8, 2, 5, 7, 3},
		{2, 8, 5, 4, 7, 3, 9, 1, 6}
	};

	/* Invalid board
	int sudokuInvalid[9][9] = {
		{6, 2, 4, 0, 3, 9, 1, 8, 0},
		{5, 0, 9, 7, 2, 8, 6, 3, 4},
		{8, 3, 7, 6, 0, 4, 2, 9, 5},
		{1, 4, 3, 8, 6, 5, 0, 2, 9},
		{9, 5, 0, 2, 4, 7, 3, 6, 1},
		{7, 6, 2, 3, 9, 1, 4, 5, 8},
		{0, 7, 1, 9, 5, 6, 8, 0, 2},
		{4, 9, 6, 1, 8, 2, 5, 7, 3},
		{2, 8, 5, 4, 7, 0, 9, 1, 6}
	};
	*/
	
	// Initialize array of threads
	thread threadArray[threadConstant];
	int threadCount = 0;

	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			if (x == 0) // Create threads to check columns
			{
				parameters *dataColumn = (parameters *)malloc(sizeof(parameters));
				dataColumn->row = x;
				dataColumn->column = y;
				dataColumn->board = sudokuValid; // Swappable with sudokuInvalid for invalid test case
				threadArray[threadCount++] = thread(checkColumn, dataColumn);
			}
			if (y == 0) // Create threads to check rows
			{
				parameters *dataRow = (parameters *)malloc(sizeof(parameters));
				dataRow->row = x;
				dataRow->column = y;
				dataRow->board = sudokuValid; // Swappable with sudokuInvalid for invalid test case
				threadArray[threadCount++] = thread(checkRow, dataRow);
			}
			if (x % 3 == 0 && y % 3 == 0) // Create threads to check Grids
			{
				parameters *dataGrid = (parameters *)malloc(sizeof(parameters));
				dataGrid->row = x;
				dataGrid->column = y;
				dataGrid->board = sudokuValid; // Swappable with sudokuInvalid for invalid test case
				threadArray[threadCount++] = thread(checkGrid, dataGrid);
			}
		}
	}

	// Wait for each thread to complete tasks
	for (int i = 0; i < threadConstant; i++)
	{
		threadArray[i].join();
		// cout << "Waiting on thread " << i + 1 << "..." << endl; For debugging, ignore
	}

	// Validate sudoku board
	bool solution = true;
	cout << "Validate array. 1 = OK, 0 = NOT OK: " << endl;
	for (int i = 0; i < threadConstant; i++)
	{
		cout << validator[i] << " ";
		if (validator[i] == 0)
		{
			solution = false;
		}
	}
	cout << endl;

	// Print results
	if (solution)
	{
		cout << "Solution valid." << endl;
	}
	else
	{
		cout << "Solution invalid." << endl;
	}

	cin.get();
	return 0;
}

void checkColumn(void *p)
{
	parameters *data = (parameters *)p;
	int c = data->column;
	int subvalidator[10] = { 0 }; // Sub-array to check for repeating numbers
	bool subvalidatorBool = true;
	for (int x = 0; x < 9; x++)
	{
		int value = data->board[x][c];
		if (subvalidator[value] != 0)
		{
			subvalidatorBool = false;
		}
		else
		{
			subvalidator[value] = 1;
		}
	}

	if (subvalidatorBool)
	{
		validator[c] = 1;
	}
}

void checkRow(void *p)
{
	parameters *data = (parameters *)p;
	int r = data->row;
	int subvalidator[10] = { 0 }; // Sub-array to check for repeating numbers
	bool subvalidatorBool = true;
	for (int y = 0; y < 9; y++)
	{
		int value = data->board[r][y];
		if (subvalidator[value] != 0)
		{
			subvalidatorBool = false;
		}
		else
		{
			subvalidator[value] = 1;
		}
	}

	if (subvalidatorBool)
	{
		validator[r + 9] = 1;
	}
}

void checkGrid(void *p)
{
	parameters *data = (parameters *)p;
	int r = data->row;
	int c = data->column;
	int subvalidator[10] = { 0 }; // Sub-array to check for repeating numbers
	bool subvalidatorBool = true;
	for (int x = r; x < r; ++x)
	{
		for (int y = c; y < c; ++y)
		{
			int value = data->board[x][y];
			if (subvalidator[value] != 0)
			{
				subvalidatorBool = false;
			}
			else
			{
				subvalidator[value] = 1;
			}
		}
	}

	if (subvalidatorBool)
	{
		validator[(r + c / 3) + 18] = 1;
	}
}