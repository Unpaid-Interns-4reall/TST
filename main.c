// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define ROWS      4
#define COLS      5
#define MAX_DEPTH 5

#define HUMAN 'A'
#define AI    'B'

// ─────────────────────────────────────────────────────────────────────────────
// Move struct
typedef struct {
    int row, col;
    int isHorizontal;  // 1 = horizontal, 0 = vertical
} Move;

// ─────────────────────────────────────────────────────────────────────────────
// Prototypes: Game logic
int  countBoxes(char player, char boxes[ROWS][COLS]);
int  checkSquare(int row, int col,
                 char player,
                 int hor[ROWS+1][COLS],
                 int ver[ROWS][COLS+1],
                 char boxes[ROWS][COLS]);
int  applyMove(int r1, int c1, int r2, int c2,
               char player,
               int hor[ROWS+1][COLS],
               int ver[ROWS][COLS+1],
               char boxes[ROWS][COLS]);
int  isGameOver(char boxes[ROWS][COLS]);

// Prototypes: I/O
int  getPlayerMove(int *r1, int *c1, int *r2, int *c2);
void printBoard(int ver[ROWS][COLS+1],
                int hor[ROWS+1][COLS],
                char boxes[ROWS][COLS]);

// Prototypes: Easy & Random bot
Move getRandomMove(int hor[ROWS+1][COLS],
                   int ver[ROWS][COLS+1]);
int  easy_bot(int hor[ROWS+1][COLS],
              int ver[ROWS][COLS+1],
              char boxes[ROWS][COLS],
              char bot);

// Prototypes: Medium bot
void copy_state(int srcHor[ROWS+1][COLS],
                int srcVer[ROWS][COLS+1],
                char srcBoxes[ROWS][COLS],
                int dstHor[ROWS+1][COLS],
                int dstVer[ROWS][COLS+1],
                char dstBoxes[ROWS][COLS]);
int  simulate_chain(int hor[ROWS+1][COLS],
                    int ver[ROWS][COLS+1],
                    char boxes[ROWS][COLS],
                    char bot);
void *worker_medium(void *arg);
int  medium_bot(int hor[ROWS+1][COLS],
                int ver[ROWS][COLS+1],
                char boxes[ROWS][COLS],
                char bot);
// ─────────────────────────────────────────────────────────────────────────────
// Globals for medium_bot threading
static pthread_mutex_t best_mutex = PTHREAD_MUTEX_INITIALIZER;
static int bestChain, bestRow, bestCol, bestIsHorizontal;

// ─────────────────────────────────────────────────────────────────────────────
// Count boxes owned by `player`
int countBoxes(char player, char boxes[ROWS][COLS]) {
    int cnt = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (boxes[i][j] == player) cnt++;
    return cnt;
}

// Check if adding an edge completes any boxes
int checkSquare(int row, int col, char player,
                int hor[ROWS+1][COLS],
                int ver[ROWS][COLS+1],
                char boxes[ROWS][COLS]) {
    int gained = 0;
    // horizontal
    if (hor[row][col]) {
        if (row < ROWS &&
            boxes[row][col] == 0 &&
            hor[row+1][col] && ver[row][col] && ver[row][col+1]) {
            boxes[row][col] = player;
            gained++;
        }
        if (row > 0 &&
            boxes[row-1][col] == 0 &&
            hor[row-1][col] && ver[row-1][col] && ver[row-1][col+1]) {
            boxes[row-1][col] = player;
            gained++;
        }
    }
    // vertical
    if (ver[row][col]) {
        if (col < COLS &&
            boxes[row][col] == 0 &&
            ver[row][col+1] && hor[row][col] && hor[row+1][col]) {
            boxes[row][col] = player;
            gained++;
        }
        if (col > 0 &&
            boxes[row][col-1] == 0 &&
            ver[row][col-1] && hor[row][col-1] && hor[row+1][col-1]) {
            boxes[row][col-1] = player;
            gained++;
        }
    }
    return gained;
}

// Apply a move; return gained boxes or -1 if invalid
int applyMove(int r1, int c1, int r2, int c2, char player,
              int hor[ROWS+1][COLS],
              int ver[ROWS][COLS+1],
              char boxes[ROWS][COLS]) {
    if (r1 == r2 && abs(c1 - c2) == 1) {
        int col = c1 < c2 ? c1 : c2;
        if (hor[r1][col]) return -1;
        hor[r1][col] = 1;
        return checkSquare(r1, col, player, hor, ver, boxes);
    }
    if (c1 == c2 && abs(r1 - r2) == 1) {
        int row = r1 < r2 ? r1 : r2;
        if (ver[row][c1]) return -1;
        ver[row][c1] = 1;
        return checkSquare(row, c1, player, hor, ver, boxes);
    }
    return -1;
}

// Check if game over
int isGameOver(char boxes[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (boxes[i][j] == 0) return 0;
    return 1;
}

// Prompt human
int getPlayerMove(int *r1, int *c1, int *r2, int *c2) {
    printf("Enter r1 c1 r2 c2:\n");
    return scanf("%d %d %d %d", r1, c1, r2, c2) == 4;
}

// Print board
void printBoard(int ver[ROWS][COLS+1],
                int hor[ROWS+1][COLS],
                char boxes[ROWS][COLS]) {
    printf("   ");
    for (int j = 0; j <= COLS; j++) printf("%d ", j);
    printf("\n");
    for (int i = 0; i <= ROWS; i++) {
        printf("%d  ", i);
        for (int j = 0; j < COLS; j++) {
            printf(".");
            printf(hor[i][j] ? "-" : " ");
        }
        printf(".\n");
        if (i < ROWS) {
            printf("   ");
            for (int j = 0; j <= COLS; j++) {
                printf(ver[i][j] ? "|" : " ");
                if (j < COLS) {
                    char c = boxes[i][j];
                    printf(c ? "%c" : " ", c);
                }
            }
            printf("\n");
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// EASY BOT: random valid move
Move getRandomMove(int hor[ROWS+1][COLS],
                   int ver[ROWS][COLS+1]) {
    Move moves[(ROWS+1)*COLS + ROWS*(COLS+1)];
    int cnt = 0;
    for (int r = 0; r <= ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (!hor[r][c]) moves[cnt++] = (Move){r,c,1};
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c <= COLS; c++)
            if (!ver[r][c]) moves[cnt++] = (Move){r,c,0};
    return moves[rand() % cnt];
}

int easy_bot(int hor[ROWS+1][COLS],
             int ver[ROWS][COLS+1],
             char boxes[ROWS][COLS],
             char bot) {
    Move m = getRandomMove(hor, ver);
    int gain = applyMove(
        m.row, m.col,
        m.isHorizontal ? m.row : m.row+1,
        m.isHorizontal ? m.col+1 : m.col,
        bot, hor, ver, boxes
    );
    printf("Easy Bot chose: %s at (%d,%d)\n",
           m.isHorizontal ? "H" : "V", m.row, m.col);
    return gain;
}

// ─────────────────────────────────────────────────────────────────────────────
// MEDIUM BOT: chain simulation + threads

void copy_state(int srcHor[ROWS+1][COLS],
                int srcVer[ROWS][COLS+1],
                char srcBoxes[ROWS][COLS],
                int dstHor[ROWS+1][COLS],
                int dstVer[ROWS][COLS+1],
                char dstBoxes[ROWS][COLS]) {
    memcpy(dstHor, srcHor, sizeof(int)*(ROWS+1)*COLS);
    memcpy(dstVer, srcVer, sizeof(int)*ROWS*(COLS+1));
    memcpy(dstBoxes, srcBoxes, sizeof(char)*ROWS*COLS);
}

int simulate_chain(int hor[ROWS+1][COLS],
                   int ver[ROWS][COLS+1],
                   char boxes[ROWS][COLS],
                   char bot) {
    int maxChain = 0, found = 0;
    // horizontal
    for (int r = 0; r <= ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        if (!hor[r][c]) {
          int h2[ROWS+1][COLS], v2[ROWS][COLS+1]; char b2[ROWS][COLS];
          copy_state(hor,ver,boxes,h2,v2,b2);
          int old = countBoxes(bot,b2);
          h2[r][c]=1; checkSquare(r,c,bot,h2,v2,b2);
          int gain=countBoxes(bot,b2)-old;
          if(gain>0){
            found=1;
            int chain=gain+simulate_chain(h2,v2,b2,bot);
            if(chain>maxChain) maxChain=chain;
          }
        }
      }
    }
    // vertical
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c <= COLS; c++) {
        if (!ver[r][c]) {
          int h2[ROWS+1][COLS], v2[ROWS][COLS+1]; char b2[ROWS][COLS];
          copy_state(hor,ver,boxes,h2,v2,b2);
          int old = countBoxes(bot,b2);
          v2[r][c]=1; checkSquare(r,c,bot,h2,v2,b2);
          int gain=countBoxes(bot,b2)-old;
          if(gain>0){
            found=1;
            int chain=gain+simulate_chain(h2,v2,b2,bot);
            if(chain>maxChain) maxChain=chain;
          }
        }
      }
    }
    return found ? maxChain : 0;
}

typedef struct {
    int row,col,isHorizontal;
    int hor[ROWS+1][COLS], ver[ROWS][COLS+1];
    char boxes[ROWS][COLS];
    char bot;
} ThreadArgs;

void *worker_medium(void *arg) {
    ThreadArgs *a = arg;
    int h2[ROWS+1][COLS], v2[ROWS][COLS+1];
    char b2[ROWS][COLS];
    copy_state(a->hor, a->ver, a->boxes, h2, v2, b2);
    int old = countBoxes(a->bot,b2);
    if(a->isHorizontal) h2[a->row][a->col]=1;
    else                v2[a->row][a->col]=1;
    checkSquare(a->row,a->col,a->bot,h2,v2,b2);
    int gain=countBoxes(a->bot,b2)-old;
    int chainGain = gain>0 ? gain + simulate_chain(h2,v2,b2,a->bot) : 0;

    pthread_mutex_lock(&best_mutex);
      if (chainGain > bestChain) {
        bestChain        = chainGain;
        bestRow          = a->row;
        bestCol          = a->col;
        bestIsHorizontal = a->isHorizontal;
      }
    pthread_mutex_unlock(&best_mutex);

    free(a);
    return NULL;
}

int medium_bot(int hor[ROWS+1][COLS],
               int ver[ROWS][COLS+1],
               char boxes[ROWS][COLS],
               char bot) {
    pthread_t th[(ROWS+1)*COLS + ROWS*(COLS+1)];
    int tc=0;
    bestChain=0; bestRow=bestCol=bestIsHorizontal=-1;

    // spawn threads
    for(int r=0;r<=ROWS;r++){
      for(int c=0;c<COLS;c++){
        if(!hor[r][c]){
          ThreadArgs *a = malloc(sizeof *a);
          a->row=r; a->col=c; a->isHorizontal=1; a->bot=bot;
          copy_state(hor,ver,boxes,a->hor,a->ver,a->boxes);
          pthread_create(&th[tc++],NULL,worker_medium,a);
        }
      }
    }
    for(int r=0;r<ROWS;r++){
      for(int c=0;c<=COLS;c++){
        if(!ver[r][c]){
          ThreadArgs *a = malloc(sizeof *a);
          a->row=r; a->col=c; a->isHorizontal=0; a->bot=bot;
          copy_state(hor,ver,boxes,a->hor,a->ver,a->boxes);
          pthread_create(&th[tc++],NULL,worker_medium,a);
        }
      }
    }
    // join
    for(int i=0;i<tc;i++) pthread_join(th[i],NULL);

    // apply bestChain move if found
    if(bestChain>0){
      int old=countBoxes(bot,boxes);
      if(bestIsHorizontal){
        hor[bestRow][bestCol]=1;
        printf("Medium Bot H at (%d,%d)\n",bestRow,bestCol);
      } else {
        ver[bestRow][bestCol]=1;
        printf("Medium Bot V at (%d,%d)\n",bestRow,bestCol);
      }
      checkSquare(bestRow,bestCol,bot,hor,ver,boxes);
      return countBoxes(bot,boxes)-old;
    }
    // fallback
    return easy_bot(hor,ver,boxes,bot);
}
// ─────────────────────────────────────────────────────────────────────────────
// MAIN
int main() {
  srand((unsigned)time(NULL));

  int hor[ROWS+1][COLS]   = {{0}};
  int ver[ROWS][COLS+1]   = {{0}};
  char boxes[ROWS][COLS]  = {{0}};
  char player             = HUMAN;
  int mode;

  printf("Select game mode:\n");
  printf("  1. Two-player\n");
  printf("  2. Easy Bot\n");
  printf("  3. Medium Bot\n");
  printf("  4. Hard Bot\n");
  printf("Enter choice: ");
  if (scanf("%d", &mode) != 1 || mode < 1 || mode > 4) {
      mode = 1;
  }

  // If Hard‑Bot mode, invoke the external program and quit
  if (mode == 4) {
      system("./Hard_Bot");
      return 0;
  }

  while (!isGameOver(boxes)) {
      printBoard(ver, hor, boxes);

      if (mode == 1 || (mode > 1 && player == HUMAN)) {
          printf("Player %c's turn.\n", player);
          int r1, c1, r2, c2;
          if (!getPlayerMove(&r1, &c1, &r2, &c2)) continue;
          int gain = applyMove(r1, c1, r2, c2,
                               player, hor, ver, boxes);
          if (gain < 0) {
              printf("Invalid move!\n");
              continue;
          }
          if (gain == 0) {
              if (mode == 1) player = (player == HUMAN ? AI : HUMAN);
              else           player = AI;
          }
      } else {
          int gain;
          if (mode == 2) {
              gain = easy_bot(hor, ver, boxes, AI);
          } else {  // mode == 3
              gain = medium_bot(hor, ver, boxes, AI);
          }
          if (gain == 0) {
              player = HUMAN;
          }
      }
  }

  printBoard(ver, hor, boxes);
  int a = countBoxes(HUMAN, boxes);
  int b = countBoxes(AI,    boxes);
  printf("Final Score A=%d B=%d\n", a, b);
  if (a > b)      printf("Player A wins!\n");
  else if (b > a) printf("Player B wins!\n");
  else            printf("It's a tie!\n");

  return 0;
}