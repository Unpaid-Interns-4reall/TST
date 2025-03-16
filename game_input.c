#include <stdio.h>
#include <stdlib.h>

// Function to get a valid move from the current player.
// Returns 1 if input is valid; otherwise returns 0.
int getPlayerMove(int *r1, int *c1, int *r2, int *c2, char currentPlayer) {
    printf("Player %c's turn. Enter the row and column of the first dot (e.g., 0 0) and second dot: ", currentPlayer);
    if (scanf("%d %d %d %d", r1, c1, r2, c2) != 4) {
        printf("Invalid input. Please try again.\n");
        while(getchar() != '\n'); // clear input buffer
        return 0;
    }
    // Validate dot coordinates.
    if (*r1 < 0 || *r1 > 4 || *r2 < 0 || *r2 > 4 ||
        *c1 < 0 || *c1 > 5 || *c2 < 0 || *c2 > 5) {
        printf("Invalid dot coordinates. Try again.\n");
        return 0;
    }
    return 1;
}