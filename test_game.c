// test_game.c
#include <assert.h>
#include <stdio.h>
#define main dont_use_main
#include "main.c"
#undef main

void test_countBoxes() {
    char boxes[ROWS][COLS] = {{0}};
    boxes[0][0] = HUMAN;
    boxes[1][2] = AI;
    assert(countBoxes(HUMAN, boxes) == 1);
    assert(countBoxes(AI, boxes)    == 1);
    printf("✔ test_countBoxes\n");
}

void test_checkSquare() {
    int hor[ROWS+1][COLS]   = {{0}};
    int ver[ROWS][COLS+1]   = {{0}};
    char boxes[ROWS][COLS]  = {{0}};
    // complete the top‐left box
    hor[0][0] = hor[1][0] = 1;
    ver[0][0] = ver[0][1] = 1;
    int gain = checkSquare(0,0, HUMAN, hor, ver, boxes);
    assert(gain == 1);
    assert(boxes[0][0] == HUMAN);
    printf("✔ test_checkSquare\n");
}

void test_applyMove() {
    int hor[ROWS+1][COLS]   = {{0}};
    int ver[ROWS][COLS+1]   = {{0}};
    char boxes[ROWS][COLS]  = {{0}};
    // valid horizontal
    int g1 = applyMove(0,0, 0,1, HUMAN, hor, ver, boxes);
    assert(g1 == 0);
    assert(hor[0][0] == 1);
    // invalid diagonal
    assert(applyMove(0,0, 1,1, HUMAN, hor, ver, boxes) == -1);
    printf("✔ test_applyMove\n");
}

void test_isGameOver() {
    char boxes[ROWS][COLS];
    for(int i=0;i<ROWS;i++)
      for(int j=0;j<COLS;j++)
        boxes[i][j] = HUMAN;
    assert(isGameOver(boxes) == 1);
    boxes[2][3] = 0;
    assert(isGameOver(boxes) == 0);
    printf("✔ test_isGameOver\n");
}

int main() {
    test_countBoxes();
    test_checkSquare();
    test_applyMove();
    test_isGameOver();
    printf("All logic tests passed!\n");
    return 0;
}