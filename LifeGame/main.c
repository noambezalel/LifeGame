#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>


#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10
#define BOARD_REFRESH_RATE 1

void init_board_cells(int board[BOARD_HEIGHT][BOARD_WIDTH]);
void process_board(int board[BOARD_HEIGHT][BOARD_WIDTH], int tmp_board[BOARD_HEIGHT][BOARD_WIDTH], int refresh_delay);
void print_board(int board[BOARD_HEIGHT][BOARD_WIDTH]);
int is_valid_bounds(int y, int x);
int should_cell_live(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x);
int calculate_neighbors(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x);
int board_count_live_cells(int board[BOARD_HEIGHT][BOARD_WIDTH]);

int main(void) {
	int size_of_board = sizeof(int[BOARD_HEIGHT][BOARD_WIDTH]);
	int (*board)[BOARD_WIDTH] = malloc(size_of_board);
	int (*tmp_board)[BOARD_WIDTH] = malloc(size_of_board);
	if (!board || !tmp_board) {
		printf("Couldn't allocate board.");
		return -1;
	}

	// zeroing boards
	memset(board, 0, size_of_board);
	memset(tmp_board, 0, size_of_board);

	init_board_cells(board);

	process_board(board, tmp_board, BOARD_REFRESH_RATE);

	// Enf of use, Freeing from memory
	free(board);
	free(tmp_board);

	return 0;
}

void init_board_cells(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
	char capture;
	int x = 0;
	int y = 0;
	board[y][x] = 2;

	int prev_x = 0;
	int prev_y = 0;

	while (1) {
		print_board(board);
		printf("Move using wasd, press SPACE to active\\deactive a cell, press ENTER to start the game.\n");

		if (_kbhit()) {
			capture = _getch();

			// previous cell position
			prev_x = x;
			prev_y = y;

			if (capture == ' ') {
				if (board[y][x] == 0) board[y][x] = 1;
				else if (board[y][x] == 1) board[y][x] = 0;
				else if (board[y][x] == 2) board[y][x] = 3;
				else if (board[y][x] == 3) board[y][x] = 2;

				system("cls");
				continue;
			}
			else if (is_valid_bounds(y, x - 1) && capture == 'a') {
				x--;
			}
			else if (is_valid_bounds(y, x + 1) && capture == 'd') {
				x++;
			}
			else if (is_valid_bounds(y - 1, x) && capture == 'w') {
				y--;
			}
			else if (is_valid_bounds(y + 1, x) && capture == 's') {
				y++;
			}

			// handle current cell
			if (board[y][x] == 0) {
				board[y][x] = 2;
			}
			if (board[y][x] == 1) {
				board[y][x] = 3;
			}
			// handle previous cell
			if (board[prev_y][prev_x] == 2) {
				board[prev_y][prev_x] = 0;
			}
			if (board[prev_y][prev_x] == 3) {
				board[prev_y][prev_x] = 1;
			}

			if (capture == 13) {
				system("cls");
				return;
			}
		}

		Sleep(100);
		system("cls");
	}
}

void process_board(int board[BOARD_HEIGHT][BOARD_WIDTH], int tmp_board[BOARD_HEIGHT][BOARD_WIDTH], int refresh_delay) {
	do {
		print_board(board);

		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				tmp_board[i][j] = should_cell_live(board, i, j);
			}
		}

		memcpy(board, tmp_board, sizeof(int[BOARD_HEIGHT][BOARD_WIDTH]));

		Sleep(refresh_delay * 1000);
		system("cls");
	} while (board_count_live_cells(board));

	print_board(board);
}

void print_board(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			int node = board[i][j];
			if (node == 1) {
				printf("%c", '#');
			}
			else if (node == 2) {
				printf("%c", '?');
			}
			else if (node == 3) {
				printf("%c", '$');
			}
			else {
				printf("%c", '.');
			}
		}
		printf("\n");
	}
}

int is_valid_bounds(int y, int x) {
	// Bounds check
	if ((BOARD_HEIGHT <= y || y < 0) || (BOARD_WIDTH <= x || x < 0)) {
		return 0;
	}

	return 1;
}

int should_cell_live(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x) {
	if (!is_valid_bounds(y, x)) {
		printf("Failed at function should_cell_live, at bounds check.");
		exit(-1);
	}

	int cell = board[y][x];
	int neighbors = calculate_neighbors(board, y, x);

	/*
		Node lives when:
		Alive && (neighbors == 2 || neighbors == 3)
		Dead && neighbors == 3
	*/
	if (cell && (neighbors == 2 || neighbors == 3)) {
		return 1;
	}
	if (!cell && neighbors == 3) {
		return 1;
	}

	/*
		Node dies when:
		Alive && neighbors < 2
		Alive && neighbors > 3
	*/
	if (cell && (neighbors < 2 || 3 < neighbors)) {
		return 0;
	}

	return 0;
}

int calculate_neighbors(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x) {
	if (!is_valid_bounds(y, x)) {
		printf("Failed at function calculate_neighbors, at bounds check.");
		exit(-1);
	}

	int neighbor_counter = 0;
	int cell;
	int tmp_y, tmp_x;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!i && !j) {
				continue;
			}

			tmp_y = y + i;
			tmp_x = x + j;


			cell = board[tmp_y][tmp_x];

			if (is_valid_bounds(tmp_y, tmp_x) && cell) {
				neighbor_counter++;
			}
		}
	}

	return neighbor_counter;
}

int board_count_live_cells(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int counter = 0;

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			int node = board[i][j];

			if (node) {
				counter++;
			}
		}
	}

	return counter;
}