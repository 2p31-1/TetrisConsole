void gotoxy(int x, int y);
void pstr(int x, int y, char* s);
void pint(int x, int y, int i);
void hideCursor();
void titleScreen();
void initialize();
void boardInit();
void gameStart();
void gameOver();
void blockLoop();
int eraseLine();
void printBoard();
void eraseBlock();
void printBlock();
void blockWrite(int c);
int checkCollision(int r);

int block_i[4][4][4],
    block_l[4][4][4],
    block_j[4][4][4],
    block_o[4][4][4],
    block_s[4][4][4],
    block_t[4][4][4],
    block_z[4][4][4];
