#include <stdio.h>

#define ROWS 4
#define COLUMNS 5

// 1) checksquare function
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
