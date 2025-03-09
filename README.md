# Dots and Boxes (4×5)

A simple two-player Dots and Boxes game implemented in C. This version uses a 4×5 grid of boxes (5×6 dots), and players take turns drawing lines between adjacent dots. Whenever a player completes a box, they score a point and take another turn.

## How It Works

1. **Board Setup**  
   - There are 5 rows and 6 columns of dots, forming 4 rows of boxes and 5 columns of boxes (total of 20 boxes).
   - Each box can be claimed by drawing its four surrounding edges.

2. **Gameplay**  
   - Two players, **Player A** and **Player B**, alternate turns.
   - On your turn, you pick two adjacent dots (either horizontally or vertically) and draw a line between them.
   - If you complete a box by drawing its last edge, you immediately take another turn.
   - The game ends once all 20 boxes are claimed.

3. **Scoring**  
   - Each claimed box is worth **1 point**.
   - Your score is the number of boxes you have claimed.
   - The player with the most boxes at the end wins; if both have the same number, it’s a tie.

## Requirements

- A C compiler (e.g., `gcc` or `clang`).

## How to Compile

1. Clone or download this repository.
2. Open a terminal in the project directory.
3. Compile the code with:
   ```bash
   gcc -o dots_and_boxes main.c
4. You should now have an executable called dots_and_boxes (on Windows, it might be dots_and_boxes.exe).

## How to Run

Run the compiled executable:
```bash
./dots_and_boxes
```
## How to Play

1. When the program starts, it displays the board and each player's score.

2. You’ll be prompted to enter four integers, for example:
   0 0 0 1
   These represent two adjacent dots:
   - `0 0` = row 0, column 0  
   - `0 1` = row 0, column 1  
   (which are horizontally adjacent)

3. Ensure the dots are **next to each other** either horizontally or vertically. If you pick an invalid pair (not adjacent, out of range, or already connected), you’ll be asked to try again.

4. If your line completes a box, you’ll see:
   Player A completed a square! And you immediately take another turn.

5. Play continues until all 20 boxes are claimed.
## Example Turn

```vbnet
Player A's turn. Enter the row and column of the first dot (e.g., 0 0) and second dot: 0 0 0 1
This means Player A is drawing a horizontal line between dot(0,0) and dot(0,1).
If that completes a box, Player A goes again; otherwise, it’s Player B’s turn.
```
## Contributing

If you'd like to improve or extend this project:
1. Fork this repository.
2. Make your changes.
3. Open a Pull Request describing your updates.

## License

This project is licensed under the MIT License.  
Feel free to use, modify, and distribute it as you see fit.
