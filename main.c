#include <stdio.h>
#include "board.c"

void main() {

	const char easy[9][9] = {
		" 9   23  ",
		"  2 7148 ",
		"5148  7  ",
		"   2   43",
		" 29 6 85 ",
		"8    5   ",
		"4 3  9  8",
		" 6518 93 ",
		"  14   6 "
	};

	const char hard[9][9] = {
		"2      7 ",
		"  6  8 5 ",
		"    9 6  ",
		"13 2 6  5",
		"  5   7  ",
		"4  5 7 83",
		"  1 5    ",
		" 4 7  5  ",
		" 7      9",
	};

	loadBoard(hard);
	printBoard();
	solveBoard();
	printBoard();
	printPossible();
}
