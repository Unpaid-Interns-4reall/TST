#include <stdio.h>
#include <stdlib.h>
#include "game_bots.h"
#include "game_logic.h"

// Easy bot: makes a random valid move.
int easy_bot(int hor[5][6], int ver[5][6], char boxes[4][5], char bot) {
    int row, col, isHorizontal;
    int valid_move = 0;
    int oldScore, newScore;
    
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
        printf("Easy bot chose: Horizontal at (%d, %d)\n", row, col);
    } else {
        printf("Easy bot chose: Vertical at (%d, %d)\n", row, col);
    }

    return newScore - oldScore;
}

// Medium bot: first checks for a move that completes a box.
// If none is found, it falls back to a random move.
int medium_bot(int hor[5][6], int ver[5][6], char boxes[4][5], char bot) {
    int tempHor[5][6], tempVer[5][6];
    char tempBoxes[4][5];
    int oldScore, newScore;
    
    // Check horizontal moves.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (hor[i][j] == 0) {
                for (int a = 0; a < 5; a++) {
                    for (int b = 0; b < 6; b++) {
                        tempHor[a][b] = hor[a][b];
                        tempVer[a][b] = ver[a][b];
                    }
                }
                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 5; b++) {
                        tempBoxes[a][b] = boxes[a][b];
                    }
                }
                oldScore = countBoxes(bot, tempBoxes);
                tempHor[i][j] = 1;
                checksquare(i, j, bot, tempHor, tempVer, tempBoxes);
                newScore = countBoxes(bot, tempBoxes);
                if (newScore - oldScore > 0) {
                    hor[i][j] = 1;
                    checksquare(i, j, bot, hor, ver, boxes);
                    printf("Medium bot chose: Horizontal at (%d, %d)\n", i, j);
                    return newScore - oldScore;
                }
            }
        }
    }
    
    // Check vertical moves.
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            if (ver[i][j] == 0) {
                for (int a = 0; a < 5; a++) {
                    for (int b = 0; b < 6; b++) {
                        tempHor[a][b] = hor[a][b];
                        tempVer[a][b] = ver[a][b];
                    }
                }
                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 5; b++) {
                        tempBoxes[a][b] = boxes[a][b];
                    }
                }
                oldScore = countBoxes(bot, tempBoxes);
                tempVer[i][j] = 1;
                checksquare(i, j, bot, tempHor, tempVer, tempBoxes);
                newScore = countBoxes(bot, tempBoxes);
                if (newScore - oldScore > 0) {
                    ver[i][j] = 1;
                    checksquare(i, j, bot, hor, ver, boxes);
                    printf("Medium bot chose: Vertical at (%d, %d)\n", i, j);
                    return newScore - oldScore;
                }
            }
        }
    }

    return easy_bot(hor, ver, boxes, bot);
}
