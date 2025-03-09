void checksquare(int row, int col, char player,
                 int horizontaledges[ROWS + 1][COLUMNS - 1],
                 int verticaledges[ROWS - 1][COLUMNS + 1],
                 char boxes[ROWS][COLUMNS])
{

    int completed = 0;

    // check if it is horizontal
    if (row < ROWS + 1 && col < COLUMNS - 1 && horizontaledges[row][col] == 1)
    {

        // check box below
        if (row < ROWS && horizontaledges[row + 1][col] && verticaledges[row][col] && verticaledges[row][col + 1])
        {
            boxes[row][col] = player;
            completed = 1;
        }

        // check box above
        if (row > 0 && horizontaledges[row - 1][col] && verticaledges[row - 1][col] && verticaledges[row - 1][col + 1])
        {
            boxes[row - 1][col] = player;
            completed = 1;
        }
    }
    // check if it is vertical
    if (row < ROWS - 1 && col < COLUMNS + 1 && verticaledges[row][col] == 1)
    {

        // check box to the right
        if (col < COLUMNS - 1 && verticaledges[row][col + 1] && horizontaledges[row][col + 1] && horizontaledges[row + 1][col])
        {
            boxes[row][col] = player;
            completed = 1;
        }
        // check box to the left
        if (col > 0 && verticaledges[row][col - 1] && horizontaledges[row][col - 1] && horizontaledges[row + 1][col - 1])
        {
            boxes[row][col - 1] = player;
            completed = 1;
        }
    }
    if (completed)
    {
        printf("Player %c completed a square!", player);
    }
}
}