// test_bot.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define main dont_use_main
#include "main.c"
#undef main

void test_getRandomMove() {
    int hor[ROWS+1][COLS] = {{0}};
    int ver[ROWS][COLS+1] = {{0}};
    srand(0);
    Move m = getRandomMove(hor, ver);
    // should be within bounds and a valid orientation
    assert(m.row >= 0 && m.row <= ROWS);
    assert(m.isHorizontal == 0 || m.isHorizontal == 1);
    printf("✔ test_getRandomMove\n");
}

void test_easy_bot() {
    int hor[ROWS+1][COLS] = {{0}};
    int ver[ROWS][COLS+1] = {{0}};
    char boxes[ROWS][COLS] = {{0}};
    srand(0);
    int gain = easy_bot(hor, ver, boxes, AI);
    // gain should be 0 or 1 (at most one box from a random move)
    assert(gain == 0 || gain == 1);
    printf("✔ test_easy_bot\n");
}

int main() {
    test_getRandomMove();
    test_easy_bot();
    printf("All bot tests passed!\n");
    return 0;
}