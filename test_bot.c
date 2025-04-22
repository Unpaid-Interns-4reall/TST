// test_bot.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Rename main in main.c to avoid conflict
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

void test_medium_bot() {
    int hor[ROWS+1][COLS] = {{0}};
    int ver[ROWS][COLS+1] = {{0}};
    char boxes[ROWS][COLS] = {{0}};
    // Set up two-box scenario at (0,0) and (0,1): all edges except the middle vertical
    // Top/bottom horizontals for both boxes
    hor[0][0] = hor[1][0] = 1;
    hor[0][1] = hor[1][1] = 1;
    // Left vertical of first box and right vertical of second box
    ver[0][0] = 1;
    ver[0][2] = 1;
    // Now only ver[0][1] is open to complete both boxes
    int gain = medium_bot(hor, ver, boxes, AI);
    // Should capture both boxes
    assert(gain == 2);
    assert(boxes[0][0] == AI && boxes[0][1] == AI);
    printf("✔ test_medium_bot\n");
}

int main() {
    test_getRandomMove();
    test_easy_bot();
    test_medium_bot();
    printf("All bot tests passed!\n");
    return 0;
}