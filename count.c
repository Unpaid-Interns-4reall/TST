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
}// nahhal was here
