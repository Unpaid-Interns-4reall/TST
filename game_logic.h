#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define ROWS 4
#define COLUMNS 5

int countBoxes(char player, char boxes[ROWS][COLUMNS]);
void checksquare(int row, int col, char player, int hor[5][6], int ver[5][6], char boxes[4][5]);
int processMove(int r1, int c1, int r2, int c2, char currentPlayer, int hor[][6], int ver[][6], char boxes[][5]);

#endif