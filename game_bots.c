#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("Bot chose: Horizontal at (%d, %d)\n", row, col);
    } else {
        printf("Bot chose: Vertical at (%d, %d)\n", row, col);
    }
    
    return newScore - oldScore;
}

// Helper function to copy board state.
void copy_state(int srcHor[5][6], int srcVer[5][6], char srcBoxes[4][5],
                int destHor[5][6], int destVer[5][6], char destBoxes[4][5]) {
    memcpy(destHor, srcHor, sizeof(int) * 5 * 6);
    memcpy(destVer, srcVer, sizeof(int) * 5 * 6);
    memcpy(destBoxes, srcBoxes, sizeof(char) * 4 * 5);
}

// Recursive function that simulates extra moves from a given state.
// Returns the maximum additional boxes that can be captured.
int simulate_chain(int hor[5][6], int ver[5][6], char boxes[4][5], char bot) {
    int maxChain = 0;
    int moveFound = 0;
    
    // Try every possible move.
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            // Try horizontal move if valid.
            if (col < 5 && hor[row][col] == 0) {
                int tempHor[5][6], tempVer[5][6];
                char tempBoxes[4][5];
                copy_state(hor, ver, boxes, tempHor, tempVer, tempBoxes);
                
                int oldScore = countBoxes(bot, tempBoxes);
                tempHor[row][col] = 1;
                checksquare(row, col, bot, tempHor, tempVer, tempBoxes);
                int newScore = countBoxes(bot, tempBoxes);
                int gain = newScore - oldScore;
                
                if (gain > 0) {
                    moveFound = 1;
                    // Earned boxes give an extra turn: simulate further chain moves.
                    int chainGain = gain + simulate_chain(tempHor, tempVer, tempBoxes, bot);
                    if (chainGain > maxChain)
                        maxChain = chainGain;
                }
            }
            // Try vertical move if valid.
            if (row < 4 && ver[row][col] == 0) {
                int tempHor[5][6], tempVer[5][6];
                char tempBoxes[4][5];
                copy_state(hor, ver, boxes, tempHor, tempVer, tempBoxes);
                
                int oldScore = countBoxes(bot, tempBoxes);
                tempVer[row][col] = 1;
                checksquare(row, col, bot, tempHor, tempVer, tempBoxes);
                int newScore = countBoxes(bot, tempBoxes);
                int gain = newScore - oldScore;
                
                if (gain > 0) {
                    moveFound = 1;
                    int chainGain = gain + simulate_chain(tempHor, tempVer, tempBoxes, bot);
                    if (chainGain > maxChain)
                        maxChain = chainGain;
                }
            }
        }
    }
    
    // If no move yields a box immediately, no chain exists.
    if (!moveFound)
        return 0;
    return maxChain;
}

// Modified medium bot: tries to maximize chain moves.
int medium_bot(int hor[5][6], int ver[5][6], char boxes[4][5], char bot) {
    int bestChain = 0;
    int bestRow = -1, bestCol = -1, bestIsHorizontal = -1;
    
    // Evaluate all valid moves.
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            // Try horizontal moves.
            if (col < 5 && hor[row][col] == 0) {
                int tempHor[5][6], tempVer[5][6];
                char tempBoxes[4][5];
                copy_state(hor, ver, boxes, tempHor, tempVer, tempBoxes);
                
                int oldScore = countBoxes(bot, tempBoxes);
                tempHor[row][col] = 1;
                checksquare(row, col, bot, tempHor, tempVer, tempBoxes);
                int newScore = countBoxes(bot, tempBoxes);
                int gain = newScore - oldScore;
                
                // If this move gains boxes, simulate chain moves.
                if (gain > 0) {
                    int chainGain = gain + simulate_chain(tempHor, tempVer, tempBoxes, bot);
                    if (chainGain > bestChain) {
                        bestChain = chainGain;
                        bestRow = row;
                        bestCol = col;
                        bestIsHorizontal = 1;
                    }
                }
            }
            
            // Try vertical moves.
            if (row < 4 && ver[row][col] == 0) {
                int tempHor[5][6], tempVer[5][6];
                char tempBoxes[4][5];
                copy_state(hor, ver, boxes, tempHor, tempVer, tempBoxes);
                
                int oldScore = countBoxes(bot, tempBoxes);
                tempVer[row][col] = 1;
                checksquare(row, col, bot, tempHor, tempVer, tempBoxes);
                int newScore = countBoxes(bot, tempBoxes);
                int gain = newScore - oldScore;
                
                if (gain > 0) {
                    int chainGain = gain + simulate_chain(tempHor, tempVer, tempBoxes, bot);
                    if (chainGain > bestChain) {
                        bestChain = chainGain;
                        bestRow = row;
                        bestCol = col;
                        bestIsHorizontal = 0;
                    }
                }
            }
        }
    }
    
    // If a move with a positive chain gain was found, execute it.
    if (bestChain > 0) {
        int oldScore = countBoxes(bot, boxes);
        if (bestIsHorizontal) {
            hor[bestRow][bestCol] = 1;
            checksquare(bestRow, bestCol, bot, hor, ver, boxes);
            printf("Bot chose: Horizontal at (%d, %d)", bestRow, bestCol);
        } else {
            ver[bestRow][bestCol] = 1;
            checksquare(bestRow, bestCol, bot, hor, ver, boxes);
            printf("Bot chose: Vertical at (%d, %d)", bestRow, bestCol);
        }
        int newScore = countBoxes(bot, boxes);
        return newScore - oldScore;
    }
    
    // Otherwise, fallback to a random move (like the easy bot).
    int row, col, isHorizontal;
    int valid_move = 0;
    while (!valid_move) {
        row = rand() % 5;
        col = rand() % 6;
        isHorizontal = rand() % 2;
        if (isHorizontal && col < 5 && hor[row][col] == 0) {
            int oldScore = countBoxes(bot, boxes);
            hor[row][col] = 1;
            checksquare(row, col, bot, hor, ver, boxes);
            int newScore = countBoxes(bot, boxes);
            valid_move = 1;
            printf("Bot chose: Horizontal at (%d, %d)\n", row, col);
            return newScore - oldScore;
        }
        else if (!isHorizontal && row < 4 && ver[row][col] == 0) {
            int oldScore = countBoxes(bot, boxes);
            ver[row][col] = 1;
            checksquare(row, col, bot, hor, ver, boxes);
            int newScore = countBoxes(bot, boxes);
            valid_move = 1;
            printf("Bot chose: Vertical at (%d, %d)\n", row, col);
            return newScore - oldScore;
        }
    }
    return 0; // Should never reach here.
}