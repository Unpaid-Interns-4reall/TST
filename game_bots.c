#include <stdio.h>
#include <stdlib.h>

int bot_move(int hor[5][6], int ver[5][6], char boxes[4][5], char bot) {
    int row, col, isHorizontal;
    int valid_move = 0;
    int oldScore, newScore;

    // Loop until a valid move is found
    while (!valid_move) {
        row = rand() % 5;
        col = rand() % 6;
        isHorizontal = rand() % 2;

        if (isHorizontal && col < 5 && hor[row][col] == 0) {
            oldScore = countBoxes(bot, boxes);
            hor[row][col] = 1;
            checksquare(row, col, bot, hor, ver, boxes);
            newScore = countBoxes(bot, boxes);
            valid_move = 1;
        }
        else if (!isHorizontal && row < 4 && ver[row][col] == 0) {
            oldScore = countBoxes(bot, boxes);
            ver[row][col] = 1;
            checksquare(row, col, bot, hor, ver, boxes);
            newScore = countBoxes(bot, boxes);
            valid_move = 1;
        }
    }

    if (isHorizontal) {
        printf("Bot chose: Horizontal at (%d, %d)\n", row, col);
    } else {
        printf("Bot chose: Vertical at (%d, %d)\n", row, col);
    }

    return newScore - oldScore; // Return how many boxes were completed
}