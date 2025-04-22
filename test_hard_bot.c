#include <assert.h>
#include <stdio.h>

// Rename main in Hard_Bot.c to avoid conflict
#define main dont_use_main
#include "Hard_Bot.c"
#undef main

// Test that initBoard sets everything to zero and player to A (0)
void test_initBoard() {
    initBoard();
    // Check edges and counts
    for(int i = 0; i <= ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            assert(hedge[i][j] == 0);
        }
    }
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j <= COLS; j++) {
            assert(vedge[i][j] == 0);
        }
    }
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            assert(boxCnt[i][j] == 0);
            assert(boxOwner[i][j] == 0);
        }
    }
    // Scores and player
    assert(score[0] == 0);
    assert(score[1] == 0);
    assert(player == 0);
    printf("✔ test_initBoard\n");
}

// Test that hardBotMove claims a three-sided box
void test_hardBotCaptures3Edge() {
    initBoard();
    // Manually create three edges around box (0,0)
    setHedge(0, 0);  // top
    setHedge(1, 0);  // bottom
    setVedge(0, 0);  // left
    assert(boxCnt[0][0] == 3);

    int prevScoreB = score[1];
    // It's B's turn after the three sets, so hardBotMove should take the 4th edge
    hardBotMove();

    // After move, boxCnt should be 4 and boxOwner 'B'
    assert(boxCnt[0][0] == 4);
    assert(boxOwner[0][0] == 'B');
    assert(score[1] == prevScoreB + 1);
    printf("✔ test_hardBotCaptures3Edge\n");
}

int main() {
    test_initBoard();
    test_hardBotCaptures3Edge();
    printf("All Hard Bot tests passed!\n");
    return 0;
}
