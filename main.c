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
    int difficulty = 1; // 1=Easy, 2=Medium, 3=Hard

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
        printf("3. Hard\n");
        printf("Enter your choice: ");
        if (scanf("%d", &difficulty) != 1) {
            printf("Invalid input. Defaulting to Easy.\n");
            difficulty = 1;
        }
        if (difficulty != 1) {
            printf("Medium and Hard difficulties will be added soon. Defaulting to Easy mode.\n");
            difficulty = 1;
        }
    }

    // Game arrays
    int hor[5][6] = {0};
    int ver[5][6] = {0};
    char boxes[4][5] = {0};
    int totalBoxes = ROWS * COLUMNS;
    int completedBoxes = 0;
    char currentPlayer = 'A';

    // Variables for human moves
    int r1, c1, r2, c2;

    while (completedBoxes < totalBoxes) {
        displayStatus(ver, hor, boxes);

        int diff = 0;  // how many boxes were completed on this turn
        if (botEnabled && currentPlayer == 'B') {
            // Bot's turn
            diff = bot_move(hor, ver, boxes, currentPlayer);
            if (diff < 0) {
                // If you ever decide the bot_move can return -1 for invalid,
                // handle that here. Right now, it won't.
                continue;
            }
        } else {
            // Human player's turn
            if (!getPlayerMove(&r1, &c1, &r2, &c2, currentPlayer)) {
                continue;  // invalid input
            }
            diff = processMove(r1, c1, r2, c2, currentPlayer, hor, ver, boxes);
            if (diff < 0) {
                // invalid move
                continue;
            }
        }

        // Update completed boxes
        completedBoxes = countBoxes('A', boxes) + countBoxes('B', boxes);

        // Only switch players if no box was completed
        if (diff == 0) {
            currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
        }
    }

    // Game is over
    displayStatus(ver, hor, boxes);
    printFinalResult(boxes);
    return 0;
}