#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

typedef struct {
    int width;
    int height;
    bool *grid;
} GolBoard;

GolBoard *create_board(int width, int height) {
    GolBoard *board = malloc(sizeof(GolBoard));
    board->width = width;
    board->height = height;
    board->grid = malloc(width * height * sizeof(bool));
    memset(board->grid, 0, width * height);
    return board;
}

inline void set_cell(GolBoard *board, int x, int y, bool state) {
    if (x >= board->width) {
        x -= board->width;
    }else if (x < 0) {
        x += board->width;
    }

    if (y >= board->height) {
        y -= board->height;
    }else if (y < 0) {
        y += board->height;
    }

    board->grid[y * board->width + x] = state;
}

inline bool get_cell(GolBoard *board, int x, int y) {
    if (x >= board->width) {
        x -= board->width;
    }else if (x < 0) {
        x += board->width;
    }

    if (y >= board->height) {
        y -= board->height;
    }else if (y < 0) {
        y += board->height;
    }

    return board->grid[y * board->width + x];
}

void board_step(GolBoard *board, GolBoard *newBoard) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            int neighbors = 0;

            neighbors += get_cell(board, x, y-1);
            neighbors += get_cell(board, x, y+1);
            neighbors += get_cell(board, x-1, y);
            neighbors += get_cell(board, x+1, y);
            neighbors += get_cell(board, x+1, y+1);
            neighbors += get_cell(board, x-1, y-1);
            neighbors += get_cell(board, x+1, y-1);
            neighbors += get_cell(board, x-1, y+1);

            if (neighbors < 2) {
                set_cell(newBoard, x, y, false);
            } else if (neighbors > 3) {
                set_cell(newBoard, x, y, false);
            } else if (neighbors == 3) {
                set_cell(newBoard, x, y, true);
            } else {
                bool current = get_cell(board, x, y);
                set_cell(newBoard, x, y, current);
            }
        }
    }
}

void board_swap(GolBoard *board1, GolBoard *board2) {
    bool *b1Grid = board1->grid;
    board1->grid = board2->grid;
    board2->grid = b1Grid;

    GolBoard *b1 = board1;
    board1 = board2;
    board2 = b1;
}

void display_board(GolBoard *board) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            bool cell = get_cell(board, x, y);
            char ch = ' ';
            if (cell) {
                ch = '#';
            }

            mvaddch(y, x, ch);
        }
    }
}

int main() {
    initscr();
    
    GolBoard *board = create_board(COLS, LINES);
    GolBoard *newBoard = create_board(COLS, LINES);

    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (rand() % 10 < 5) {
                set_cell(board, x, y, false);
            } else {
                set_cell(board, x, y, true);
            }
        }
    }
    
    while (true) {
        board_step(board, newBoard);
        board_swap(board, newBoard);
        display_board(board);
        refresh();
        usleep(100000);
    }

    endwin();
    return 0;
}
