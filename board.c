#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define NUM1  (1UL << 0)
#define NUM2  (1UL << 1)
#define NUM3  (1UL << 2)
#define NUM4  (1UL << 3)
#define NUM5  (1UL << 4)
#define NUM6  (1UL << 5)
#define NUM7  (1UL << 6)
#define NUM8  (1UL << 7)
#define NUM9  (1UL << 8)

typedef uint16_t board_t[9][9];
board_t Board;

const uint16_t states[10] = {
	NUM1 | NUM2 | NUM3 | NUM4 | NUM5 | NUM6 | NUM7 | NUM8 | NUM9, // all possible
	NUM1, // 1 possible
	NUM2, // 2 possible
	NUM3, // etc
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9
};

void clearBoard(board_t b) {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			b[r][c] = states[0];
		}
	}
}

void loadBoard(const char numbers[9][9]) {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			Board[r][c] = states[0];
			char n = numbers[r][c];
			if (n >= '1' && n <= '9') {
				Board[r][c] = states[n - '0'];
			}
		}
	}
}

void printBoard() {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			char sep = ' ';
			if (r % 3 == 2)
				sep = '_';
			
			for (int s = 1; s <= 9; s++) {
				uint16_t state = states[s];
				if (Board[r][c] == state) {
					putchar(s + '0');
					break;
				}
				if (s == 9)
					putchar(sep);
			}

			if (c % 3 == 2) {
				putchar('|');
			} else {
				putchar(sep);
			}
		}
		putchar('\n');
	}
}

void printPossible() {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			int count = 0;
			for (int s = 1; s <= 9; s++) {
				if ((Board[r][c] & states[s]) > 0) {
					printf("%d", s);
					count++;
				}
			}

			for (int i = count; i < 10; i++) {
				putchar(' ');
			}
		}
		putchar('\n');
	}
}

int cellSolved(board_t b, int r, int c) {
	for (int s = 1; s <= 9; s++) {
		if (b[r][c] == states[s]) {
			return 1;
		}
	}
	return 0;
}

int boardSolved(board_t b) {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			if (!cellSolved(b, r, c))
				return 0;
		}
	}
	return 1;
}

int boardValidSolution(board_t b) {
	for (int r = 0; r < 9; r++) {
		uint16_t found = 0;
		for (int c = 0; c < 9; c++) {
			if (!cellSolved(b, r, c))
				return 0;

			if (found & b[r][c])
				return 0;

			found |= b[r][c];
		}
	}
	for (int c = 0; c < 9; c++) {
		uint16_t found = 0;
		for (int r = 0; r < 9; r++) {
			if (found & b[r][c])
				return 0;

			found |= b[r][c];
		}
	}
	return 1;
}

int reduceBoard(board_t board) {
	int changes = 0;
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {

			for (int s = 1; s <= 9; s++) {
				uint16_t state = states[s];
				if (board[row][col] == state) { // solved

					// reduce row
					for (int c = 0; c < 9; c++) {
						if (c != col && (board[row][c] & state) > 0) {
							board[row][c] &= ~state;
							changes++;
						}
					}

					// reduce column
					for (int r = 0; r < 9; r++) {
						if (r != row && (board[r][col] & state) > 0) {
							board[r][col] &= ~state;
							changes++;
						}
					}

					// reduce block
					int blockRow = (row / 3) * 3;
					int blockCol = (col / 3) * 3;
					for (int r = blockRow; r < blockRow + 3; r++) {
						for (int c = blockCol; c < blockCol + 3; c++) {
							if ((r != row || c != col) && (board[r][c] & state)) {
								board[r][c] &= ~state;
								changes++;
							}
						}
					}

					break;
				}
			}
		}
	}
	return changes;
}

int solveSingles(board_t board) {
	int changes = 0;
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (cellSolved(board, row, col))
				continue;
			
			// each state
			for (int s = 1; s <= 9; s++) {
				uint16_t state = states[s];
				if ((board[row][col] & state) > 0) { // cell has state
					int num = 0;

					// check row
					for (int c = 0; c < 9; c++) {
						if (c != col && (board[row][c] & state)) {
							num++;
							break;
						}
					}
					if (num == 0) {
						board[row][col] = state;
						changes++;
					} else {

						// check column
						num = 0;
						for (int r = 0; r < 9; r++) {
							if (r != row && (board[r][col] & state)) {
								num++;
								break;
							}
						}
						if (num == 0) {
							board[row][col] = state;
							changes++;
						} else {

							// check block
							num = 0;
							int blockRow = (row / 3) * 3;
							int blockCol = (col / 3) * 3;
							for (int r = blockRow; r < blockRow + 3; r++) {
								for (int c = blockCol; c < blockCol + 3; c++) {
									if ((r != row || c != col) && (board[r][c] & state)) {
										num++;
										break;
									}
								}
							}
							if (num == 0) {
								board[row][col] = state;
								changes++;
							}
						}
					}
				}
			}
		}
	}
	return changes;
}

int alignments(board_t board) {
	int changes = 0;
	for (int blockRow = 0; blockRow < 3; blockRow++) {
		for (int blockCol = 0; blockCol < 3; blockCol++) {

			for (int s = 1; s <= 9; s++) {
				uint16_t state = states[s];
				int rowCount[3] = {0, 0, 0};
				int colCount[3] = {0, 0, 0};

				for (int row = blockRow * 3, r = 0; r < 3; r++, row++) {
					for (int col = blockCol * 3, c = 0; c < 3; c++, col++) {
						if (board[row][col] & state) {
							rowCount[r]++;
							colCount[c]++;
						}
					}
				}

				const int skip[3][2] = {
					{1, 2},
					{0, 2},
					{0, 1},
				};

				// reduce unique columns
				for (int c = 0; c < 3; c++) {
					if (colCount[skip[c][0]] == 0 && colCount[skip[c][1]] == 0) {
						for (int br = 0; br < 3; br++) {
							if (br != blockRow) {
								int col = blockCol*3 + c;
								for (int row = br*3, r = 0; r < 3; r++, row++) {
									if (board[row][col] & state) {
										board[row][col] &= ~state;
										changes++;
									}
								}
							}
						}
					}
				}

				// reduce unique rows
				for (int r = 0; r < 3; r++) {
					if (rowCount[skip[r][0]] == 0 && rowCount[skip[r][1]] == 0) {
						for (int bc = 0; bc < 3; bc++) {
							if (bc != blockCol) {
								int row = blockRow*3 + r;
								for (int col = bc*3, c = 0; c < 3; c++, col++) {
									if (board[row][col] & state) {
										board[row][col] &= ~state;
										changes++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return changes;
}

void systematicPass(board_t b) {
	int changes = 1;
	while (changes > 0) {
		changes = 0;
		changes += reduceBoard(b);
		changes += solveSingles(b);
		changes += alignments(b);
	}
	printf("pass complete\n");
}



void solveBoard() {
	systematicPass(Board);
	if (boardSolved(Board)) {
		return;
	}

	printf("trying possibilites\n");
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			if (cellSolved(Board, r, c))
				continue;

			// now we have an unsolved cell
			for (int s = 1; s <= 9; s++) {
				uint16_t state = states[s];
				if (Board[r][c] & state) {

					board_t newBoard;
					memcpy(newBoard, Board, sizeof(board_t));
					newBoard[r][c] = state;
					systematicPass(newBoard);
					if (boardSolved(newBoard)) {
						if (boardValidSolution(newBoard)) {
							printf("valid solution found\n");
							memcpy(Board, newBoard, sizeof(board_t));
							return;
						} else {
							printf("solution invalid\n");
						}
					} else {
						printf("solution not found\n");
					}
				}
			}

		}
	}
}
