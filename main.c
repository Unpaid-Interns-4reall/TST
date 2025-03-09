#include <stdio.h>
#include "count.c"
#include "print_board.c"
#include "checksquare.c"

#define ROWS 4
#define COLUMNS 5

// 1) Main function: Two-player loop
int main()
{
    // Arrays to track edges
    int ver[5][6] = {0};
    int hor[5][6] = {0};

    // Boxes array to store which player (A/B) claimed each box
    char boxes[4][5] = {0};

    // Total boxes on the board is 4*5 = 20
    int totalBoxes = ROWS * COLUMNS;
    int completedBoxes = 0;

    // Game starts with Player A
    char currentPlayer = 'A';

    // Keep going until all boxes are claimed
    while (completedBoxes < totalBoxes)
    {
        // 1) Print current board
        printBoard(ver, hor, boxes);

        // 2) Print current scores
        printf("Player A score: %d\n", countBoxes('A', boxes));
        printf("Player B score: %d\n", countBoxes('B', boxes));
        printf("****************\n");

        // 3) Prompt current player for input
        printf("Player %c's turn. Enter the row and column of the first dot (e.g., 0 0) and second dot: ", currentPlayer);
        int r1, c1, r2, c2;
        if (scanf("%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
            // Bad input (not 4 integers)
            printf("Invalid input. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }

        // Validate that the dots are within the correct range
        // Dots range: row in [0..4], col in [0..5]
        if (r1 < 0 || r1 > 4 || r2 < 0 || r2 > 4 || 
            c1 < 0 || c1 > 5 || c2 < 0 || c2 > 5)
        {
            printf("Invalid dot coordinates. Try again.\n");
            continue;
        }

        // 4) Check if the move is horizontal or vertical
        //    and if it’s exactly between adjacent dots
        if (r1 == r2 && (c1 == c2 + 1 || c1 + 1 == c2)) 
        {
            // Horizontal move
            int col = (c1 < c2) ? c1 : c2;
            // Check if this edge is already taken
            if (hor[r1][col] == 1) {
                printf("That line is already taken. Try again.\n");
                continue;
            }

            // Mark the edge
            hor[r1][col] = 1;

            // Check if a box was completed
            int oldScore = countBoxes(currentPlayer, boxes);
            checksquare(r1, col, currentPlayer, hor, ver, boxes);
            int newScore = countBoxes(currentPlayer, boxes);

            // If no new box, switch players
            if (newScore == oldScore) {
                currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
            }
        }
        else if (c1 == c2 && (r1 == r2 + 1 || r1 + 1 == r2)) 
        {
            // Vertical move
            int row = (r1 < r2) ? r1 : r2;
            // Check if this edge is already taken
            if (ver[row][c1] == 1) {
                printf("That line is already taken. Try again.\n");
                continue;
            }

            // Mark the edge
            ver[row][c1] = 1;

            // Check if a box was completed
            int oldScore = countBoxes(currentPlayer, boxes);
            checksquare(row, c1, currentPlayer, hor, ver, boxes);
            int newScore = countBoxes(currentPlayer, boxes);

            // If no new box, switch players
            if (newScore == oldScore) {
                currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
            }
        }
        else
        {
            printf("Invalid move! Dots must be adjacent horizontally or vertically.\n");
            continue;
        }

        // 5) Update how many boxes are claimed so far
        completedBoxes = countBoxes('A', boxes) + countBoxes('B', boxes);
    }

    // Once all boxes are claimed, print final board
    printBoard(ver, hor, boxes);

    // Final scores
    int finalScoreA = countBoxes('A', boxes);
    int finalScoreB = countBoxes('B', boxes);

    printf("Player A score: %d\n", finalScoreA);
    printf("Player B score: %d\n", finalScoreB);

    // Determine winner
    if (finalScoreA > finalScoreB) {
        printf("Player A wins!\n");
    } else if (finalScoreB > finalScoreA) {
        printf("Player B wins!\n");
    } else {
        printf("It's a tie!\n");
    }

    return 0;
}