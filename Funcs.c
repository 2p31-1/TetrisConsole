#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Funcs.h"
#include "Leaderboard.h"

//#define DEBUG_SHOWBLOCKVALUES

int board[21][10],
    buffer[20][10],
    blockX,
    blockY,
    (*block)[4][4],
    blockKind,
    r,
    printFlag,
    keyLeft,
    keyRight,
    keyDown,
    keyRotate,
    keySuperDown,
    fallDelay,
    moveDelay,
    downDelay,
    rotateDelay,
    superfallDelay,
    fallTime,
    moveTime,
    downTime,
    rotateTime,
    superfallTime,
    currentTime,
    score,
    level,
    nextScore,
	difficulty,
	combo,
	maxcombo,
	falled;
//????
int typeValue[7];
void gotoxy(int x, int y)
{
    COORD Pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void pstr(int x, int y, char* s) {
    gotoxy(x, y);
    printf(s);
}

void pint(int x, int y, int i) {
    gotoxy(x, y);
    printf("%d", i);
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = 0;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void titleScreen() {

    system("cls");
    pstr(34, 10, "T E T R I S");
    pstr(34, 14, "<S> : Start");
    pstr(34, 16, "<EASY>HARD ");
	pstr(34, 17, " <A>   <D>");
	pstr(33, 18, "<L>eaderboard");
//	pstr(32, 20 ,"<B>lack <W>hite");
	gotoxy(20, 25);
	printf(" BuILd %s %s", __DATE__, __TIME__);
	difficulty=0;
	while (1)
        if (GetAsyncKeyState(83) & 0x8000 || GetAsyncKeyState(28) & 0x8000) {
            boardInit();
			initialize();
            return gameStart();
        }else if(GetAsyncKeyState(65) & 0x8000 || GetAsyncKeyState(0x25) & 0x8000) {
			pstr(34, 16, "<EASY>HARD ");
			difficulty=0;
		}else if(GetAsyncKeyState(68) & 0x8000 || GetAsyncKeyState(0x27) & 0x8000) {
			pstr(34, 16, " EASY<HARD>");
			difficulty=1;
		}else if(GetAsyncKeyState(76) & 0x8000) {
			return ShowLeaderboard();
		}
/*		if(GetAsyncKeySate(66)&0x8000){
			system("color 0f");
			return titleScreen();
		}else if(GetAsyncKeyState(87)&0x8000){
			system("color f0");
			return titleScreen();
		}*/
}

void initialize() {

    srand(time(NULL));
    hideCursor();

    keyLeft = 65;       // A
    keyRight = 68;      // D
    keyDown = 83;       // S
    keyRotate = 87;     // W
    keySuperDown = 32;  // Space

    fallDelay = 500;
	if(difficulty)fallDelay=300;
    moveDelay = 100;
    downDelay = 50;
    rotateDelay = 200;
    superfallDelay = 300;
	if(difficulty)superfallDelay=200;

    nextScore = 4;
	combo=0, maxcombo=0;
	falled=0;
	
	typeValue[0]=10;
	typeValue[1]=10;
	typeValue[2]=10;
	typeValue[3]=10;
	typeValue[4]=10;
	typeValue[5]=10;
	typeValue[6]=10;
}

int blockrandom(){
	//????
	int foo, sum=0, i;
	for(i=0;i<=6;i++){
		if(!typeValue[i]){
			typeValue[0]=10;
			typeValue[1]=10;
			typeValue[2]=10;
			typeValue[3]=10;
			typeValue[4]=10;
			typeValue[5]=10;
			typeValue[6]=10;
			break;
		}
	}
	for(i=0;i<=6;i++){
		sum+=typeValue[i];
	}
	foo=rand()%(sum+1);
	#ifdef DEBUG_SHOWBLOCKVALUES
	pint(60, 7, sum);
	pint(60, 8, foo);
	#endif
	sum=0;
	for(i=0;i<=6;i++){
		sum+=typeValue[i];
		if(foo<=sum)break;
	}
	typeValue[i]--;
	#ifdef DEBUG_SHOWBLOCKVALUES
	for(sum=0;sum<=6;sum++){
		pint(60, sum, typeValue[sum]);
	}
	#endif
	return i;
}

void boardInit() {

    int i, j;

    system("cls");
    
    score = 0;
    level = 1;

    for (i = 0; i < 20; i++)
        for (j = 0; j < 10; j++)
            board[i][j] = buffer[i][j] = 0;

    for (i = 0; i < 10; i++)
        board[20][i] = 1;

    for (i = 2; i <= 21; i++) {
        pstr(28, i, "≦");
        pstr(50, i, "≦");
    }
    pstr(28, 1, "≦≦≦≦≦≦≦≦≦≦≦≦");
    pstr(28, 22, "≦≦≦≦≦≦≦≦≦≦≦≦");
    pstr(14, 4, "NEXT");
    pstr(10, 5, "旨--------旬");
    pstr(10, 6, "早        早");
    pstr(10, 7, "早        早");
    pstr(10, 8, "曲--------旭");
    pstr(54, 9, "旨--------------------旬");
    pstr(54, 10, "早<A> : Left          早");
    pstr(54, 11, "早<S> : Down          早");
    pstr(54, 12, "早<D> : Right         早");
    pstr(54, 13, "早<W> : Rotate        早");
    pstr(54, 14, "早<Space> : Fast Fall 早");
    pstr(54, 15, "早<P> : Pause         早");
    pstr(54, 16, "曲--------------------旭");
    pstr(56, 18, "旨--------------------旬");
    pstr(56, 19, "早Level               早");
    pstr(56, 20, "早Score :             早");
    pstr(56, 21, "曲--------------------旭");
}

void gameStart() {

    int i, j, nextBlockKind;
	int scoremix=0;
    nextBlockKind = rand() % 7;

    while (1) {
        blockKind = nextBlockKind;
        blockX = 3;
        blockY = 0;
        r = 0;

        switch (blockKind) {
        case 0:
            block = block_i;
            blockY = -1;
            break;
        case 1:
            block = block_l;
            break;
        case 2:
            block = block_j;
            break;
        case 3:
            block = block_o;
            blockX = 4;
            break;
        case 4:
            block = block_s;
            break;
        case 5:
            block = block_t;
            break;
        case 6:
            block = block_z;
            break;
        }

        if (!checkCollision(r))
            return gameOver();
		
        nextBlockKind = blockrandom();
        
		pstr(12, 6, "        ");
        pstr(12, 7, "        ");
		
        switch (nextBlockKind) {
        case 0:
            pstr(12, 6, "﹤﹤﹤﹤");
            break;
        case 1:
            pstr(13, 6, "﹥");
            pstr(13, 7, "﹥﹥﹥");
            break;
        case 2:
            pstr(17, 6, "〡");
            pstr(13, 7, "〡〡〡");
            break;
        case 3:
            pstr(14, 6, "〥〥");
            pstr(14, 7, "〥〥");
            break;
        case 4:
            pstr(15, 6, "〦〦");
            pstr(13, 7, "〦〦");
            break;
        case 5:
            pstr(15, 6, "〧");
            pstr(13, 7, "〧〧〧");
            break;
        case 6:
            pstr(13, 6, "〨〨");
            pstr(15, 7, "〨〨");
            break;
        }
		scoremix=GetTickCount();
        printBlock();
        printBoard();
		pint(64, 19, level);
		pint(66, 20, score);
        while (falled >= nextScore) {
            level++;
            nextScore *= 3.5;
            fallDelay *= 0.9;
            pint(64, 19, level);
        }

        blockLoop();
		scoremix=4000-GetTickCount()+scoremix;
		if(scoremix>0){
			if(scoremix>1500){
				combo++;
				if(maxcombo<combo)maxcombo=combo;
			}
			if(difficulty)scoremix*=10;
			scoremix=(int)log((double)score*(double)combo*(double)level*(double)scoremix);
			if(scoremix>0)score+=3*scoremix;
		}else combo=0;

        if (eraseLine())
            pint(66, 20, score);

    }
}

void gameOver() {

    pstr(10, 12, "GAME OVER");
    pstr(61, 12, "GAME OVER");
	CheckLeaderboard(score);
    pstr(33, 23, "<R> : Try again");
    pstr(28, 24, "<T> : Go to title screen");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (1) {
        if (GetAsyncKeyState(82) & 0x8000) {
            initialize();
			boardInit();
            return gameStart();
        }
        if (GetAsyncKeyState(84) & 0x8000) {
            return titleScreen();
        }
    }
}

void blockLoop() {

    fallTime = moveTime = downTime = rotateTime = superfallTime = GetTickCount();
    int R, i, j, tempX, move;

    while (1) {

        printFlag = 0;
        currentTime = GetTickCount();

        if (currentTime - fallTime > fallDelay) {
            eraseBlock();
            blockY++;
            if (checkCollision(r)) {
                printBlock();
                fallTime = GetTickCount();
                printFlag = 1;
            }
            else {
                blockY--;
                printBlock();
                fallTime = GetTickCount();
                printBoard();
                return;
            }
        }

        if (GetAsyncKeyState(keySuperDown) & 0x8000 && currentTime - superfallTime > superfallDelay) {
            eraseBlock();
            while (checkCollision(r)) {
                blockY++;
            }
            blockY--;
            printBlock();
            superfallTime = fallTime = GetTickCount();
            printBoard();
            return;
        }

        if ((GetAsyncKeyState(keyLeft) & 0x8000 || GetAsyncKeyState(0x25) & 0x8000) && currentTime - moveTime > moveDelay) {
            eraseBlock();
            blockX--;
            if (checkCollision(r)) {
                printBlock();
                moveTime = GetTickCount();
                printFlag = 1;
            }
            else {
                blockX++;
                printBlock();
            }
        }

        else if ((GetAsyncKeyState(keyRight) & 0x8000 || GetAsyncKeyState(0x27) & 0x8000) && currentTime - moveTime > moveDelay) {
            eraseBlock();
            blockX++;
            if (checkCollision(r)) {
                printBlock();
                moveTime = GetTickCount();
                printFlag = 1;
            }
            else {
                blockX--;
                printBlock();
            }
        }

        if ((GetAsyncKeyState(keyDown) & 0x8000 || GetAsyncKeyState(0x28) & 0x8000) && currentTime - downTime > downDelay) {
            eraseBlock();
            blockY++;
            if (checkCollision(r)) {
                printBlock();
                downTime = GetTickCount();
                printFlag = 1;
            }
            else {
                blockY--;
                printBlock();
                fallTime = GetTickCount();
                printBoard();
                return;
            }
        }

        if ((GetAsyncKeyState(keyRotate) & 0x8000 || GetAsyncKeyState(0x26)) && currentTime - rotateTime > rotateDelay) {
            eraseBlock();
            R = (r + 1) % 4;
            if (checkCollision(R)) {
                r = R;
                printBlock();
                rotateTime = GetTickCount();
                printFlag = 1;
            }
            else {
                tempX = blockX;
                if (blockKind == 0) move = 2;
                else move = 1;
                for (i = 1; i <= move; i++) {
                    for (j = -1; j <= 1; j += 2) {
                        blockX = tempX + i * j;
                        if (checkCollision(R)) {
                            r = R;
                            printBlock();
                            rotateTime = GetTickCount();
                            printFlag = 1;
                            goto rotateEnd;
                        }
                    }
                }
                blockX = tempX;
                printBlock();
            }
        }
		
		if (GetAsyncKeyState(80) & 0x8000){
			pstr(10, 12, "Paused.");
			pstr(10, 13, "Press R to");
			pstr(10, 14, " continue.");
			while(!(GetAsyncKeyState(82) & 0x8000));
			pstr(10, 12, "       ");
			pstr(10, 13, "          ");
			pstr(10, 14, "          ");
		}

rotateEnd:;
        if (printFlag) printBoard();
    }
}

int eraseLine() {

    int i, j, k, flag, change = 0;

    for (i = 0; i < 20; i++) {
        flag = 1;
        for (j = 0; j < 10; j++)
            if (board[i][j] == 0)
                flag = 0;
        if (flag) {
            change = 1;
            for (j = 0; j < 10; j++)
                board[i][j] = 0;
            for (k = i - 1; k >= 0; k--)
                for (j = 0; j < 10; j++)
                    board[k + 1][j] = board[k][j];
            score += 500;
			if(difficulty)score+=100;
			falled++;
        }
    }
	score += 50;
	if(difficulty)score+=10;
    return change;
}

int checkCollision(int r) {

    int i, j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (block[r][i][j] && (board[blockY + i][blockX + j] || blockX + j < 0 || blockX + j >= 10)) return 0;

    return 1;
}

void printBlock() {
    blockWrite(blockKind + 1);
}

void eraseBlock() {
    blockWrite(0);
}

void blockWrite(int c) {

    int i, j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (block[r][i][j]) board[blockY + i][blockX + j] = c;
}

void printBoard() {

    int i, j;

    for (i = 0; i < 20; i++) {
        for (j = 0; j < 10; j++) {
            gotoxy(30 + j * 2, i + 2);
            if (board[i][j] != buffer[i][j]) {
                switch (board[i][j]) {
                case 0:
                    printf("  ");
                    break;
                case 1:
                    printf("﹤");
                    break;
                case 2:
                    printf("﹥");
                    break;
                case 3:
                    printf("〡");
                    break;
                case 4:
                    printf("〥");
                    break;
                case 5:
                    printf("〦");
                    break;
                case 6:
                    printf("〧");
                    break;
                case 7:
                    printf("〨");
                }
                buffer[i][j] = board[i][j];
            }
        }
    }
}
