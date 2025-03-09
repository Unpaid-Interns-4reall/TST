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
