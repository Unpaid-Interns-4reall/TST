
/*  
Hard_Bot.c
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 
 /* ---------- board dimensions ---------- */
 #define ROWS     4            /* # box rows   */
 #define COLS     5            /* # box cols   */
 #define DOT_ROWS (ROWS+1)     /* # dot rows   */
 #define DOT_COLS (COLS+1)     /* # dot cols   */
 
 /* ---------- state arrays ---------- */
 static int  hedge[DOT_ROWS][COLS];
 static int  vedge[ROWS][DOT_COLS];
 static int  boxCnt[ROWS][COLS];
 static char boxOwner[ROWS][COLS];
 static int  score[2];
 static int  player;
 
 /* ---------- helpers shared with AI ---------- */
 static int u, v, x, y, zz, count, loop;
 
 /* ---------- function prototypes ---------- */
 static void initBoard(void);
 static void drawBoard(void);
 static void humanMove(void);
 static void hardBotMove(void);
 
 static void setHedge(int i, int j);
 static void setVedge(int i, int j);
 static void checkH(int i, int j);
 static void checkV(int i, int j);
 
 static void takeEdge(int zz_, int x_, int y_);
 static void takesafe3s(void);
 static int  sides3(void);
 static void takeall3s(void);
 static int  sides01(void);
 static int  safeHedge(int i, int j);
 static int  safeVedge(int i, int j);
 static int  randHedge(int i, int j);
 static int  randVedge(int i, int j);
 static int  singleton(void);
 static int  doubleton(void);
 static int  ldub(int i, int j);
 static int  rdub(int i, int j);
 static int  udub(int i, int j);
 static int  ddub(int i, int j);
 static void sac(int i, int j);
 static void incount(int k, int i, int j);
 static void outcount(int k, int i, int j);
 static void takeallbut(int x_, int y_);
 static int  sides3not(int x_, int y_);
 static void takebox(int i, int j);
 static void makeanymove(void);
 
 /* ---------- main ---------- */
 int main(void) {
    srand((unsigned)time(NULL));
    initBoard();

    while (score[0] + score[1] < ROWS * COLS) {
        drawBoard();
        // exact prompt as in your screenshot:
        printf(
            "Player %c's turn. Enter the row and column of the first dot "
            "(e.g., A0 -> 0 0) and second dot: ",
            player ? 'B' : 'A'
        );
        if (player == 0)
            humanMove();
        else
            hardBotMove();
    }

    drawBoard();
    // after final drawBoard (which already shows stars & scores), show result:
    if (score[0] > score[1])      puts("A wins!");
    else if (score[1] > score[0]) puts("B wins!");
    else                           puts("It's a tie!");
    return 0;
}
 /* ---------- initialisation ---------- */
 static void initBoard(void)
 {
     memset(hedge,    0, sizeof hedge);
     memset(vedge,    0, sizeof vedge);
     memset(boxCnt,   0, sizeof boxCnt);
     memset(boxOwner, 0, sizeof boxOwner);
     score[0] = score[1] = 0;
     player = 0;
 }
 
 /* ---------- fixed console drawing ---------- */
 static void drawBoard(void)
 {
     // Column headers
     printf("   ");
     for (int j = 0; j <= COLS; j++) {
         printf("%d ", j);
     }
     printf("\n");
 
     // Grid rows
     for (int i = 0; i <= ROWS; i++) {
         // Row header + dots + horizontal edges
         printf("%d  ", i);
         for (int j = 0; j < COLS; j++) {
             printf(".%c", hedge[i][j] ? '-' : ' ');
         }
         printf(".\n");
 
         // Vertical edges + box owners (skip after last dot‐row)
         if (i < ROWS) {
             printf("   ");
             for (int j = 0; j <= COLS; j++) {
                 printf("%c", vedge[i][j] ? '|' : ' ');
                 if (j < COLS) {
                     printf("%c", boxOwner[i][j] ? boxOwner[i][j] : ' ');
                 }
             }
             printf("\n");
         }
     }
 
     // Score and current player
     printf("\nScore  A:%d  B:%d   [%c’s turn]\n\n",
            score[0], score[1], player ? 'B' : 'A');
 }
 /* ---------- human move ---------- */
 static void humanMove(void)
 {
     int r1, c1, r2, c2;
     while (1) {
         printf("Enter move (r1 c1 r2 c2): ");
         if (scanf("%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
             puts("Bad input; try again.");
             while (getchar() != '\n');
             continue;
         }
         int dr = abs(r1 - r2), dc = abs(c1 - c2);
         if (!((dr == 0 && dc == 1) || (dr == 1 && dc == 0))) {
             puts("Points must be adjacent.");
             continue;
         }
         if (dr == 0) {  /* horizontal */
             int r = r1, c = (c1 < c2 ? c1 : c2);
             if (r < 0 || r >= DOT_ROWS || c < 0 || c >= COLS || hedge[r][c]) {
                 puts("Invalid or taken."); continue;
             }
             setHedge(r, c);
         } else {       /* vertical */
             int r = (r1 < r2 ? r1 : r2), c = c1;
             if (r < 0 || r >= ROWS || c < 0 || c >= DOT_COLS || vedge[r][c]) {
                 puts("Invalid or taken."); continue;
             }
             setVedge(r, c);
         }
         break;
     }
 }
 
 /* ---------- edge setters & scoring ---------- */
 static void setHedge(int i, int j)
 {
     hedge[i][j] = 1;
     if (i > 0)     boxCnt[i-1][j]++;
     if (i < ROWS)  boxCnt[i][j]++;
     checkH(i, j);
     player = 1 - player;
 }
 
 static void setVedge(int i, int j)
 {
     vedge[i][j] = 1;
     if (j > 0)     boxCnt[i][j-1]++;
     if (j < COLS)  boxCnt[i][j]++;
     checkV(i, j);
     player = 1 - player;
 }
 
 static void checkH(int i, int j)
 {
     int hit = 0;
     if (i > 0 && boxCnt[i-1][j] == 4 && !boxOwner[i-1][j]) {
         boxOwner[i-1][j] = player ? 'B' : 'A';
         score[player]++; hit = 1;
     }
     if (i < ROWS && boxCnt[i][j] == 4 && !boxOwner[i][j]) {
         boxOwner[i][j] = player ? 'B' : 'A';
         score[player]++; hit = 1;
     }
     if (hit) player = 1 - player;
 }
 
 static void checkV(int i, int j)
 {
     int hit = 0;
     if (j > 0 && boxCnt[i][j-1] == 4 && !boxOwner[i][j-1]) {
         boxOwner[i][j-1] = player ? 'B' : 'A';
         score[player]++; hit = 1;
     }
     if (j < COLS && boxCnt[i][j] == 4 && !boxOwner[i][j]) {
         boxOwner[i][j] = player ? 'B' : 'A';
         score[player]++; hit = 1;
     }
     if (hit) player = 1 - player;
 }
 
 /* ---------- universal helper ---------- */
 static void takeEdge(int zz_, int x_, int y_)
 {
     if (zz_ > 1) setVedge(x_, y_);
     else          setHedge(x_, y_);
 }
 /* ---------- AI main ---------- */
 static void hardBotMove(void)
 {
     takesafe3s();
     if (sides3()) {
         if (sides01()) {
             takeall3s();
             takeEdge(zz, x, y);
         } else {
             sac(u, v);
         }
     } else if (sides01())        takeEdge(zz, x, y);
     else if (singleton())        takeEdge(zz, x, y);
     else if (doubleton())        takeEdge(zz, x, y);
     else                         makeanymove();
 }
 
 /* ---------- AI helper implementations ---------- */
 static void takesafe3s(void)
 {
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j < COLS; ++j)
         if (boxCnt[i][j] == 3) {
             if (!vedge[i][j]) {
                 if (j == 0 || boxCnt[i][j-1] != 2) setVedge(i, j);
             } else if (!hedge[i][j]) {
                 if (i == 0 || boxCnt[i-1][j] != 2) setHedge(i, j);
             } else if (!vedge[i][j+1]) {
                 if (j == COLS-1 || boxCnt[i][j+1] != 2) setVedge(i, j+1);
             } else if (!hedge[i+1][j]) {
                 if (i == ROWS-1 || boxCnt[i+1][j] != 2) setHedge(i+1, j);
             }
         }
 }
 
 static int sides3(void)
 {
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j < COLS; ++j)
         if (boxCnt[i][j] == 3) { u = i; v = j; return 1; }
     return 0;
 }
 
 static void takeall3s(void)
 {
     while (sides3()) takebox(u, v);
 }
 
 static int safeHedge(int i, int j)
 {
     if (!hedge[i][j]) {
         if (i == 0)         return boxCnt[i][j] < 2;
         else if (i == ROWS) return boxCnt[i-1][j] < 2;
         else                return boxCnt[i][j] < 2 && boxCnt[i-1][j] < 2;
     }
     return 0;
 }
 
 static int safeVedge(int i, int j)
 {
     if (!vedge[i][j]) {
         if (j == 0)         return boxCnt[i][j] < 2;
         else if (j == COLS) return boxCnt[i][j-1] < 2;
         else                return boxCnt[i][j] < 2 && boxCnt[i][j-1] < 2;
     }
     return 0;
 }
 
 static int randHedge(int i, int j)
 {
     x = i; y = j;
     do {
         if (safeHedge(x, y)) return 1;
         if (++y == COLS) { y = 0; if (++x > ROWS) x = 0; }
     } while (x != i || y != j);
     return 0;
 }
 
 static int randVedge(int i, int j)
 {
     x = i; y = j;
     do {
         if (safeVedge(x, y)) return 1;
         if (++y > COLS) { y = 0; if (++x == ROWS) x = 0; }
     } while (x != i || y != j);
     return 0;
 }
 
 static int sides01(void)
 {
     zz = (rand() & 1) ? 1 : 2;
     int i = rand() % ROWS;
     int j = rand() % COLS;
     if (zz == 1) {
         if (randHedge(i, j)) return 1;
         zz = 2;
         if (randVedge(i, j)) return 1;
     } else {
         if (randVedge(i, j)) return 1;
         zz = 1;
         if (randHedge(i, j)) return 1;
     }
     return 0;
 }
 
 static int singleton(void)
 {
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j < COLS; ++j)
         if (boxCnt[i][j] == 2) {
             int numb = 0;
             if (!hedge[i][j] && (i < 1 || boxCnt[i-1][j] < 2)) ++numb;
             zz = 2;
             if (!vedge[i][j] && (j < 1 || boxCnt[i][j-1] < 2) && ++numb > 1) {
                 x = i; y = j; return 1;
             }
             if (!vedge[i][j+1] && (j+1 == COLS || boxCnt[i][j+1] < 2) && ++numb > 1) {
                 x = i; y = j+1; return 1;
             }
             zz = 1;
             if (!hedge[i+1][j] && (i+1 == ROWS || boxCnt[i+1][j] < 2) && ++numb > 1) {
                 x = i+1; y = j; return 1;
             }
         }
     return 0;
 }
 
 static int ldub(int i, int j)
 {
     if (!vedge[i][j])       return (j < 1 || boxCnt[i][j-1] < 2);
     else if (!hedge[i][j])  return (i < 1 || boxCnt[i-1][j] < 2);
     else                    return (i == ROWS-1 || boxCnt[i+1][j] < 2);
 }
 
 static int rdub(int i, int j)
 {
     if (!vedge[i][j+1])     return (j+1 == COLS || boxCnt[i][j+1] < 2);
     else if (!hedge[i][j])  return (i < 1 || boxCnt[i-1][j] < 2);
     else                    return (i+1 == ROWS || boxCnt[i+1][j] < 2);
 }
 
 static int udub(int i, int j)
 {
     if (!hedge[i][j])       return (i < 1 || boxCnt[i-1][j] < 2);
     else if (!vedge[i][j])  return (j < 1 || boxCnt[i][j-1] < 2);
     else                    return (j == COLS-1 || boxCnt[i][j+1] < 2);
 }
 
 static int ddub(int i, int j)
 {
     if (!hedge[i+1][j])     return (i == ROWS-1 || boxCnt[i+1][j] < 2);
     else if (!vedge[i][j])  return (j < 1 || boxCnt[i][j-1] < 2);
     else                    return (j == COLS-1 || boxCnt[i][j+1] < 2);
 }
 
 static int doubleton(void)
 {
     zz = 2;
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j < COLS-1; ++j)
         if (boxCnt[i][j] == 2 && boxCnt[i][j+1] == 2 && !vedge[i][j+1])
             if (ldub(i, j) && rdub(i, j+1)) { x = i; y = j+1; return 1; }
 
     zz = 1;
     for (int j = 0; j < COLS; ++j)
     for (int i = 0; i < ROWS-1; ++i)
         if (boxCnt[i][j] == 2 && boxCnt[i+1][j] == 2 && !hedge[i+1][j])
             if (udub(i, j) && ddub(i+1, j)) { x = i+1; y = j; return 1; }
     return 0;
 }
 
 static void incount(int k, int i, int j)
 {
     count++;
     if (k != 1 && !vedge[i][j] && j > 0) {
         if (boxCnt[i][j-1] > 2) { count++; loop = 1; }
         else if (boxCnt[i][j-1] > 1) incount(3, i, j-1);
     } else if (k != 2 && !hedge[i][j] && i > 0) {
         if (boxCnt[i-1][j] > 2) { count++; loop = 1; }
         else if (boxCnt[i-1][j] > 1) incount(4, i-1, j);
     } else if (k != 3 && !vedge[i][j+1] && j < COLS-1) {
         if (boxCnt[i][j+1] > 2) { count++; loop = 1; }
         else if (boxCnt[i][j+1] > 1) incount(1, i, j+1);
     } else if (k != 4 && !hedge[i+1][j] && i < ROWS-1) {
         if (boxCnt[i+1][j] > 2) { count++; loop = 1; }
         else if (boxCnt[i+1][j] > 1) incount(2, i+1, j);
     }
 }
 
 static void sac(int i, int j)
 {
     count = 0; loop = 0;
     incount(0, i, j);
     if (!loop) takeallbut(i, j);
     if (count + score[0] + score[1] == ROWS * COLS) takeall3s();
     else {
         if (loop) count -= 2;
         outcount(0, i, j);
     }
 }
 
 static void takeallbut(int x_, int y_)
 {
     while (sides3not(x_, y_)) takebox(u, v);
 }
 
 static int sides3not(int x_, int y_)
 {
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j < COLS; ++j)
         if (boxCnt[i][j] == 3 && (i != x_ || j != y_)) { u = i; v = j; return 1; }
     return 0;
 }
 
 static void takebox(int i, int j)
 {
     if (!hedge[i][j])        setHedge(i, j);
     else if (!vedge[i][j])   setVedge(i, j);
     else if (!hedge[i+1][j]) setHedge(i+1, j);
     else                     setVedge(i, j+1);
 }
 
 static void outcount(int k, int i, int j)
 {
     if (count <= 0) return;
     if (k != 1 && !vedge[i][j]) {
         if (count != 2) setVedge(i, j);
         count--; outcount(3, i, j-1);
     } else if (k != 2 && !hedge[i][j]) {
         if (count != 2) setHedge(i, j);
         count--; outcount(4, i-1, j);
     } else if (k != 3 && !vedge[i][j+1]) {
         if (count != 2) setVedge(i, j+1);
         count--; outcount(1, i, j+1);
     } else if (k != 4 && !hedge[i+1][j]) {
         if (count != 2) setHedge(i+1, j);
         count--; outcount(2, i+1, j);
     }
 }
 
 static void makeanymove(void)
 {
     for (int i = 0; i <= ROWS; ++i)
     for (int j = 0; j < COLS; ++j)
         if (!hedge[i][j]) { setHedge(i, j); return; }
 
     for (int i = 0; i < ROWS; ++i)
     for (int j = 0; j <= COLS; ++j)
         if (!vedge[i][j]) { setVedge(i, j); return; }
 }