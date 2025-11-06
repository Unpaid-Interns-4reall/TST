# Dots and Boxes (4x5)

Console Dots and Boxes written in C. The board has 4 rows by 5 columns of boxes (20 total), which corresponds to a 5x6 grid of dots. Draw edges between adjacent dots to complete boxes. When you complete a box, you score it and take another turn.

This build supports four modes:
- 1: Two-player (hotseat)
- 2: Easy bot (random valid moves)
- 3: Medium bot (chain-aware heuristic)
- 4: Hard bot (separate program `Hard_Bot`)

## Board & Rules

- Dots are addressed by zero-based coordinates: rows `0..4` and columns `0..5`.
- A move is two adjacent dots either horizontally or vertically: `r1 c1 r2 c2`.
- Completing a box grants it to the current player and they play again.
- Game ends when all 20 boxes are owned; highest score wins.

## Build

Build both the main game and the hard bot program in the project root.

- Windows (MinGW):
  - `gcc -O2 -std=c11 main.c -o main.exe`
  - `gcc -O2 -std=c11 Hard_Bot.c -o Hard_Bot.exe`

- Linux/macOS:
  - `gcc -O2 -std=c11 main.c -o main`
  - `gcc -O2 -std=c11 Hard_Bot.c -o Hard_Bot`

Notes:
- No pthreads are required; the medium bot runs sequentially.
- Mode 4 launches the external `Hard_Bot` binary in the same folder (`Hard_Bot.exe` on Windows, `Hard_Bot` on Unix-like systems).

## Run

- Windows: `./main.exe`
- Linux/macOS: `./main`

Select a mode (1–4) when prompted.

## Input Format

On your turn, enter four integers separated by spaces:

`r1 c1 r2 c2`

Examples:
- `0 0 0 1` draws a horizontal edge between `(0,0)` and `(0,1)`.
- `1 3 2 3` draws a vertical edge between `(1,3)` and `(2,3)`.

The two dots must be adjacent and within range; attempting an invalid or already-drawn edge will be rejected and you can enter another move.

## Example Prompt

`Player A's turn. Enter the row and column of the first dot (e.g., 0 0) and second dot: 0 0 0 1`

## Contributing

1. Fork the repo and create a feature branch.
2. Make focused changes with clear commit messages.
3. Open a Pull Request describing the change and testing.

## License

MIT License.

