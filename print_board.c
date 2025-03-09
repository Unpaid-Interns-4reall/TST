void printBoard(int ver[5][6], int hor[5][6], char boxes[4][5])
{
    // Column headers
    printf("   ");
    for (int j = 0; j < 6; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < 5; i++) 
    {
        // Row header
        printf("%d  ", i);

        // Print dots + horizontal edges
        for (int j = 0; j < 6; j++) {
            printf(".");  // the dot
            // horizontal edge?
            if (j < 5 && hor[i][j] == 1) {
                printf("-");
            } else if (j < 5) {
                printf(" ");
            }
        }
        printf("\n");

        // Print vertical edges + boxes (except the last row of dots)
        if (i < 4) {
            printf("   "); 
            for (int j = 0; j < 6; j++) {
                if (ver[i][j] == 1) {
                    printf("|");
                } else {
                    printf(" ");
                }
                if (j < 5) {
                    // print the box char or space
                    if (boxes[i][j] == 0) {
                        printf(" ");
                    } else {
                        printf("%c", boxes[i][j]);
                    }
                }
            }
            printf("\n");
        }
    }
}