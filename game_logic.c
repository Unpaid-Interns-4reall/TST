#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"

int countBoxes(char player, char boxes[ROWS][COLUMNS]) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (boxes[i][j] == player) {
                count++;
            }
        }
    }
    return count;
}

void checksquare(int row, int col, char player, 
                 int hor[5][6],
                 int ver[5][6],
                 char boxes[4][5])
{
    int completed = 0;

    // If this was a horizontal edge
    if ((row >= 0 && row < 5) && (col >= 0 && col < 5) && hor[row][col] == 1)
    {
        // Check box below
        if (row < 4
            && boxes[row][col] == 0  
            && hor[row+1][col] == 1 
            && ver[row][col] == 1 
            && ver[row][col+1] == 1)
        {
            boxes[row][col] = player;
            completed = 1;
        }
        // Check box above
        if (row > 0 
            && boxes[row-1][col] == 0  
            && hor[row-1][col] == 1
            && ver[row-1][col] == 1
            && ver[row-1][col+1] == 1)
        {
            boxes[row-1][col] = player;
            completed = 1;
        }
    }
        
    // If this was a vertical edge
    if (row >= 0 && row < 4 && col >= 0 && col < 6 && ver[row][col] == 1)
    {
        // Check box to the right
        if (col < 5
            && boxes[row][col] == 0  
            && ver[row][col+1] == 1
            && hor[row][col] == 1
            && hor[row+1][col] == 1)
        {
            boxes[row][col] = player;
            completed = 1;
        }
        // Check box to the left
        if (col > 0
            && boxes[row][col-1] == 0
            && ver[row][col-1] == 1
            && hor[row][col-1] == 1
            && hor[row+1][col-1] == 1)
        {
            boxes[row][col-1] = player;
            completed = 1;
        }
    }

    if (completed == 1)
    {
        printf("Player %c completed a square!\n", player);
    }
}

int processMove(int r1, int c1, int r2, int c2, char currentPlayer, int hor[][6], int ver[][6], char boxes[][5]) {
    if (r1 == r2 && (c1 == c2 + 1 || c1 + 1 == c2)) {
        // Horizontal move.
        int col = (c1 < c2) ? c1 : c2;
        if (hor[r1][col] == 1) {
            printf("That line is already taken. Try again.\n");
            return -1;
        }
        hor[r1][col] = 1;
        int oldScore = countBoxes(currentPlayer, boxes);
        checksquare(r1, col, currentPlayer, hor, ver, boxes);
        int newScore = countBoxes(currentPlayer, boxes);
        return (newScore - oldScore);
    }
    else if (c1 == c2 && (r1 == r2 + 1 || r1 + 1 == r2)) {
        // Vertical move.
        int row = (r1 < r2) ? r1 : r2;
        if (ver[row][c1] == 1) {
            printf("That line is already taken. Try again.\n");
            return -1;
        }
        ver[row][c1] = 1;
        int oldScore = countBoxes(currentPlayer, boxes);
        checksquare(row, c1, currentPlayer, hor, ver, boxes);
        int newScore = countBoxes(currentPlayer, boxes);
        return (newScore - oldScore);
    }
    else {
        printf("Invalid move! Dots must be adjacent horizontally or vertically.\n");
        return -1;
    }
}