#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_logic.c"
#include "game_display.c"
#include "game_input.c"
#include "game_bots.c"

#define ROWS 4
#define COLUMNS 5

int main(void) {
    srand(time(NULL));

    int gameMode = 0;
    int botEnabled = 0;
    int difficulty = 1; // 1 = Easy, 2 = Medium

    printf("Choose game mode:\n");
    printf("1. Two Player Mode\n");
    printf("2. Play Against Bot\n");
    printf("Enter your choice: ");
    if (scanf("%d", &gameMode) != 1) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    if (gameMode == 2) {
        botEnabled = 1;
        printf("Choose bot difficulty:\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("Enter your choice: ");
        if (scanf("%d", &difficulty) != 1) {
            printf("Invalid input. Defaulting to Easy.\n");
            difficulty = 1;
        }
    }

    int hor[5][6] = {0};
    int ver[5][6] = {0};
    char boxes[4][5] = {0};
    int totalBoxes = ROWS * COLUMNS;
    int completedBoxes = 0;
    char currentPlayer = 'A';
    int r1, c1, r2, c2;

    while (completedBoxes < totalBoxes) {
        displayStatus(ver, hor, boxes);

        int diff = 0;  
        if (botEnabled && currentPlayer == 'B') {
            if (difficulty == 1) {
                diff = easy_bot(hor, ver, boxes, currentPlayer);
            } else if (difficulty == 2) {
                diff = medium_bot(hor, ver, boxes, currentPlayer);
            }
        } else {
            if (!getPlayerMove(&r1, &c1, &r2, &c2, currentPlayer)) {
                continue; 
            }
            diff = processMove(r1, c1, r2, c2, currentPlayer, hor, ver, boxes);
            if (diff < 0) {
                continue;
            }
        }

        completedBoxes = countBoxes('A', boxes) + countBoxes('B', boxes);

        if (diff == 0) {
            currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
        }
    }

    displayStatus(ver, hor, boxes);
    printFinalResult(boxes);
    return 0;
}