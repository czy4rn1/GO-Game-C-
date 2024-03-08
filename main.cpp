#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include <cmath>

int sizeY = 13;
int sizeX = 13;
int timer = 2;
int groupXSize = 0;
int groupYSize = 0;

struct stone {
	int x = 1500, y=1500;
	int kolor=5;
	bool killedInOneMove = false;
};

stone groupsY[20] = {{0}};
stone groupsX[20] = {{0}};

void initGroupX() {
	for (int i = 0; i < 20; i++) {
		groupsX[i].x = 1500;
		groupsX[i].y = 1500;
		groupsX[i].kolor = 5;
		groupsX[i].killedInOneMove = false;
	}
}
void initGroupY() {
	for (int i = 0; i < 20; i++) {
		groupsY[i].x = 1500;
		groupsY[i].y = 1500;
		groupsY[i].kolor = 5;
		groupsY[i].killedInOneMove = false;
	}
}
void checkIfGroupDies(stone** placement, int height, int width, double* WScore, int* GScore, int direction);
void setGroups(stone** placement, int height, int width, int* groupXSize, int* groupYSize, double* WScore, int* GScore);
void changeSize(stone** placement, int *height, int *width, int *sizeX, int *sizeY, double* WScore, int* GScore);
void deleteBoard(stone** placement, int height);
void loadstate(stone ***placement, int* height, int* width, int* sizeX, int* sizeY, double* WScore, int* GScore);
void savestate(stone **placement, double WScore, int GScore);
void printLegend(int zero, int legendX, int legendY, int mapX, int zn, char* txt, int x, int y, double WScore, int GScore);
void printBoard(int mapX, stone **placement, int height, int width);
void addToBoard(int x, int y, stone **placement, int mapX, int* kolor);
void startNewGame(stone **placement, int* x, int* y, int mapX, int height, int width, double* WScore, int* GScore);
void setBoard(stone **placement, int height, int width);
void checkIfSomeoneDies(stone **placement, double* WScore, int* GScore);
void kill(stone **placement, int x, int y, double* WScore, int* GScore, bool ifCounts);
stone** initBoard(stone** placement, int height, int width);
void changetoDefined(stone** placement, int* height, int* width, int* sizeX, int* sizeY, int x, int y, double* WScore, int* GScore);
bool checkIfCanPlace(stone** placement, int x, int y, int kolor, int mapX, double* WScore, int* GScore, int* timer);
void endGame(int mapX, int mapY, double* WScore, int* GScore);

int main() {
	initGroupX();
	initGroupY();
	bool flag = true, gameBegin = false, addPoint = true;
	const int legendX = 1;
	const int legendY = 1;
	const int mapX = 60;
	int zn = 0, x = mapX + sizeX, y = sizeY, zero = 0, kolor = 1, GScore = 0, currentX = x, currentY = y;
	double WScore = 0;
	char txt[32];
	int height = sizeY * 2 + 1;
	int width = sizeX * 2 + 1;
	stone** placement = NULL;
	placement = initBoard(placement, height, width);
	setBoard(placement, height, width);
#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("Michal, Czarnobaj, 188816");
	_setcursortype(_NOCURSOR);
	do {
		textbackground(BLACK);
		clrscr();
		textcolor(7);
		printLegend(zero, legendX, legendY, mapX, zn, txt, x, y, WScore, GScore);
		printBoard(mapX, placement, height, width);
		gotoxy(x, y);
		if (kolor == 0) textcolor(7);
		else textcolor(LIGHTGREEN);
		textbackground(BLACK);
		putch('*');
		zero = 0;
		zn = getch();
		switch (zn) {
		case 0:
			zero = 1;
			zn = getch();
			if (zn == 0x48 && y > 1) y--;
			else if (zn == 0x50 && y < height) y++;
			else if (zn == 0x4b && x > mapX + 1) x--;
			else if (zn == 0x4d && x < mapX + width) x++;
			break;
		case 0x0d:
			checkIfSomeoneDies(placement, &WScore, &GScore);
			setGroups(placement, height, width, &groupXSize, &groupYSize, &WScore, &GScore);
			flag = true;
			gameBegin = true;
			timer--;
			break;
		case 0x1b:
			kill(placement, currentX, currentY, &WScore, &GScore, false);
			printBoard(mapX, placement, height, width);
			flag = true;
			if (kolor == 0) kolor = 1;
			else kolor = 0;
			break;
		case 'i':
			if (x % 2 == 1 && y % 2 == 1 && flag && checkIfCanPlace(placement, x, y, kolor, mapX, &WScore, &GScore, &timer)) {
				currentX = x - mapX - 1;
				currentY = y - 1;
				addToBoard(x, y, placement, mapX, &kolor);
				flag = false;
			}
			break;
		case 'h':
			if (x % 2 == 1 && y % 2 == 1 && !gameBegin) {
				kolor = 1;
				addToBoard(x, y, placement, mapX, &kolor);
				if (addPoint) {
					WScore += 0.5;
					addPoint = false;
				}
			}
			break;
		case 'f':
			endGame(48, 10, &WScore, &GScore);
			zn = getch();
			setBoard(placement, height, width);
			break;
		case 'n':
			startNewGame(placement, &x, &y, mapX, height, width, &WScore, &GScore);
			gameBegin = false;
			addPoint = true;
			break;
		case 's':
			savestate(placement, WScore, GScore);
			break;
		case 'l':
			loadstate(&placement, &height, &width, &sizeX, &sizeY, &WScore, &GScore);
			break;
		case 'c':
			changeSize(placement, &height, &width, &sizeX, &sizeY, &WScore, &GScore);
			placement = initBoard(placement, height, width);
			setBoard(placement, height, width);
			break;
		case '9':
			changetoDefined(placement, &height, &width, &sizeX, &sizeY, 9, 9, &WScore, &GScore);
			placement = initBoard(placement, height, width);
			setBoard(placement, height, width);
			break;
		case '3':
			changetoDefined(placement, &height, &width, &sizeX, &sizeY, 13, 13, &WScore, &GScore);
			placement = initBoard(placement, height, width);
			setBoard(placement, height, width);
			break;
		case '0':
			changetoDefined(placement, &height, &width, &sizeX, &sizeY, 19, 19, &WScore, &GScore);
			placement = initBoard(placement, height, width);
			setBoard(placement, height, width);
			break;
		default:
			break;
		}
	} while (zn != 'q');
	deleteBoard(placement, height);
	_setcursortype(_NORMALCURSOR);
	return 0;
}

void printLegend(int zero, int legendX, int legendY, int mapX, int zn, char* txt, int x, int y, double WScore, int GScore) {
	gotoxy(legendX, legendY);
	cputs("up_arrow = move up");
	gotoxy(legendX, legendY+1);
	cputs("down_arrow = move down");
	gotoxy(legendX, legendY+2);
	cputs("left_arrow = move left");
	gotoxy(legendX, legendY+3);
	cputs("right_arrow = move right");
	gotoxy(legendX, legendY+4);
	cputs("q       = exit");
	gotoxy(legendX, legendY+5);
	cputs("n       = start new game");
	if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
	else sprintf(txt, "key code: 0x%02x", zn);
	gotoxy(legendX, legendY+6);
	cputs(txt);
	gotoxy(legendX, legendY+7);
	cputs("i       = put on the board");
	gotoxy(legendX, legendY+8);
	cputs("s       = save gamestate");
	gotoxy(legendX, legendY+9);
	cputs("l       = load gamestate");
	gotoxy(legendX, legendY+10);
	cputs("f       = end game");
	gotoxy(legendX, legendY+11);
	cputs("enter   = next round");
	gotoxy(legendX, legendY + 12);
	cputs("esc     = cancel your move");
	gotoxy(legendX, legendY+13);
	cputs("9       = change size to 9x9");
	gotoxy(legendX, legendY+14);
	cputs("3       = change size to 13x13");
	gotoxy(legendX, legendY+15);
	cputs("0       = change size to 19x19");
	gotoxy(legendX, legendY+16);
	cputs("c       = change to any size");
	gotoxy(legendX, legendY+17);
	if (x >= mapX + 1 && y <= sizeY * 2 + 1 && x <= mapX + sizeX * 2 + 1) {
		sprintf(txt, "current position: %d, %d", x - mapX, y);
	}
	else sprintf(txt, "current position: 0, 0");
	cputs(txt);
	gotoxy(legendX + 2, legendY+21);
	cputs("White Score");
	gotoxy(legendX + 7, legendY+22);
	if (fmod(WScore, 1.0) == 0) sprintf(txt, "%d", WScore);
	else sprintf(txt, "%.1f", WScore);
	cputs(txt);
	gotoxy(legendX + 17, legendY+21);
	cputs("Green Score");
	gotoxy(legendX + 22, legendY+22);
	sprintf(txt, "%d", GScore);
	cputs(txt);
}
void printBoard(int mapX, stone **placement, int height, int width) {
	gotoxy(mapX, 1);
	textcolor(LIGHTRED);
	for (int i = 1; i <= height; i++) {
		for (int j = 1; j <= width; j++) {
			gotoxy(mapX + j, i);
			if (i % 2 == 1) {
				cputs("-");
			}
			else {
				if (j % 2 != 0) cputs("|");
			}
		}
	}
	for (int i = 0; i < height ; i++) {
		for (int j = 0; j < width; j++) {
			if (placement[i][j].x != 1500 && placement[i][j].y != 1500) {
				gotoxy(placement[i][j].x + mapX + 1, placement[i][j].y + 1);
				if (placement[i][j].kolor == 0) textcolor(7);
				else if (placement[i][j].kolor == 1) textcolor(LIGHTGREEN);
				cputs("*");
			}
		}
	}
}

void addToBoard(int x, int y, stone **placement, int mapX, int* kolor) {
	if (placement[y - 1][x - mapX - 1].x == 1500 && placement[y - 1][x - mapX - 1].y == 1500) {
		placement[y - 1][x - mapX - 1].x = x - mapX - 1;
		placement[y - 1][x - mapX - 1].y = y - 1;
		placement[y - 1][x - mapX - 1].kolor = *kolor;
		if (*kolor == 0) *kolor = 1;
		else *kolor = 0;
	}

}

void startNewGame(stone **placement, int* x, int* y, int mapX, int height, int width, double* WScore, int* GScore) {
	setBoard(placement, height, width);
	*x = sizeX + mapX;
	*y = sizeY;
	*WScore = 0;
	*GScore = 0;
}

void setBoard(stone **placement, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			placement[i][j].x = 1500;
			placement[i][j].y = 1500;
			placement[i][j].kolor = 5;
			placement[i][j].killedInOneMove = false;
		}
	}
}

stone **initBoard(stone** placement, int height, int width) {
	placement = new stone * [height];
	for (int i = 0; i < height; i++) {
		placement[i] = new stone[width];
	}
	return placement;
}

void checkIfSomeoneDies(stone **placement, double* WScore, int* GScore) {
	short enemyCounter = 0;
	for (int i = 0; i < sizeY * 2 + 1; i++) {
		for (int j = 0; j < sizeX * 2 + 1; j++) {
			stone checked = placement[i][j];
			if (checked.x != 1500 && checked.y != 1500) {
				int right = checked.x + 2;
				int left = checked.x - 2;
				int up = checked.y - 2;
				int down = checked.y + 2;
				if (right <= sizeX*2+1 && placement[i][right].x == right && placement[i][right].kolor != checked.kolor) {
					enemyCounter += 1;
				}
				if (left >= 0 && placement[i][left].x == left && placement[i][left].kolor != checked.kolor) {
					enemyCounter += 1;
				}
				if (up >= 0 && placement[up][j].y == up && placement[up][j].kolor != checked.kolor) {
					enemyCounter += 1;
				}
				if (down <= sizeY*2+1 && placement[down][j].y == down && placement[down][j].kolor != checked.kolor) {
					enemyCounter += 1;
				}
				if (enemyCounter == 4) {
					kill(placement, j, i, WScore, GScore, true);
				}
				else if (enemyCounter == 3) {
					if (left < 0 || \
						up<0 || \
						right >sizeX * 2 + 1 || \
						down>sizeY * 2 + 1) kill(placement, j, i, WScore, GScore, true);
					else placement[i][j].killedInOneMove = true;
				}
				else if (enemyCounter == 2) {
					if ((left < 0 && down > sizeY * 2 + 1) || \
						(left < 0 && up < 0) || \
						(right > sizeX * 2 + 1 && up < 0) || \
						(right > sizeX * 2 + 1 && down > sizeY * 2 + 1)) kill(placement, j, i, WScore, GScore, true);
				}
				enemyCounter = 0;
			}
		}
	}

}

void kill(stone **placement, int x, int y, double* WScore, int* GScore, bool ifCounts) {
	placement[y][x].x = 1500;
	placement[y][x].y = 1500;
	if (ifCounts) {
		if (placement[y][x].kolor == 0) *GScore += 1;
		else *WScore += 1;
	}
	placement[y][x].kolor = 5;
}

void loadstate(stone*** placement, int* height, int* width, int* sizeX, int* sizeY, double* WScore, int* GScore) {
	gotoxy(1, 20);
	_setcursortype(_NORMALCURSOR);
	char filename[30];
	char znak;
	for (int i = 0; i < 29; i++) {
		znak = getch();
		if (znak != 0x0d && znak != 0x1b) filename[i] = znak;
		else if (znak == 0x1b) {
			_setcursortype(_NOCURSOR);
			return;
		}
		else {
			filename[i] = '\0';
			break;
		}
	}
	FILE* file = fopen(filename, "r");
	int x, y, kolor, currentSizeX, currentSizeY;
	if (file != NULL) {
		fscanf(file, "%lf %d", WScore, GScore);
		fscanf(file, "%d", &timer);
		fscanf(file, "%d %d", &currentSizeX, &currentSizeY); 
		*sizeX = currentSizeX;
		*sizeY = currentSizeY;
		deleteBoard(*placement, *height);   
		*height = currentSizeY * 2 + 1;
		*width = currentSizeX * 2 + 1;
		*placement = initBoard(*placement, *height, *width); 
		setBoard(*placement, *height, *width);
		for (int i = 0; i < *height; i++) {
			for (int j = 0; j < *width; j++) {
				fscanf(file, "%d %d %d", &x, &y, &kolor);
				(* placement)[i][j].x = x; 
				(* placement)[i][j].y = y;
				(* placement)[i][j].kolor = kolor;
			}
		}
		fclose(file);
	}
	_setcursortype(_NOCURSOR);
}

void savestate(stone **placement, double WScore, int GScore) {
	gotoxy(1, 20);
	_setcursortype(_NORMALCURSOR);
	char filename[30];
	char znak;
	for (int i = 0; i < 29; i++) {
		znak = getch();
		if (znak != 0x0d && znak != 0x1b) filename[i] = znak;
		else if (znak == 0x1b) {
			_setcursortype(_NOCURSOR);
			return;
		}
		else {
			filename[i] = '\0';
			break;
		}
	}
	FILE* file = fopen(filename, "w");
	if (file != NULL) {
		fprintf(file, "%.1f %d\n", WScore, GScore);
		fprintf(file, "%d\n", timer);
		fprintf(file, "%d %d\n", sizeX, sizeY);
		for (int i = 0; i < sizeY * 2 + 1; i++) {
			for (int j = 0; j < sizeX * 2 + 1; j++) {
				fprintf(file, "%d %d %d\n", placement[i][j].x, \
					placement[i][j].y, placement[i][j].kolor);
			}
		}
		fclose(file);
	}

	_setcursortype(_NOCURSOR);

}

void deleteBoard(stone** placement, int height) {
	for (int i = 0; i < height; i++) {
		delete[] placement[i];
	}
	delete[] placement;
}

void changeSize(stone** placement, int *height, int *width, int *sizeX, int *sizeY, double* WScore, int* GScore) {
	deleteBoard(placement, *height);
	int TabNewSizeX[] = {0, 0};
	int TabNewSizeY[] = {0, 0};
	char znak;
	_setcursortype(_NORMALCURSOR);
	for (int i = 0; i < 2; i++) {
		znak = getch();
		if (znak != 0x0d) TabNewSizeX[i] = znak - '0';
		else break;
	}
	for (int i = 0; i < 2; i++) {
		znak = getch();
		if (znak != 0x0d) TabNewSizeY[i] = znak - '0';
		else break;
	}
	int newSizeX = TabNewSizeX[0] * 10 + TabNewSizeX[1];
	int newSizeY = TabNewSizeY[0] * 10 + TabNewSizeY[1];
	*height = newSizeY * 2 + 1;
	*width = newSizeX * 2 + 1;
	*sizeX = newSizeX;
	*sizeY = newSizeY;
	*WScore = 0;
	*GScore = 0;
	_setcursortype(_NOCURSOR);
}

void changetoDefined(stone** placement, int* height, int* width, int* sizeX, int* sizeY, int x, int y, double* WScore, int* GScore) {
	deleteBoard(placement, *height);
	*sizeX = x;
	*sizeY = y;
	*height = *sizeX * 2 + 1;
	*width = *sizeY * 2 + 1;
	*WScore = 0;
	*GScore = 0;
}

bool checkIfCanPlace(stone** placement, int x, int y, int kolor, int mapX, double* WScore, int* GScore, int* timer) {
	int enemyCounter = 0;
	int right = x + 2 - mapX-1;
	int left = x - 2 - mapX-1;
	int up = y - 2 - 1;
	int down = y + 2 - 1;
	if (y - 1 >= 0 && right <= sizeX * 2 + 1 && placement[y - 1][right].x == right && placement[y - 1][right].kolor != kolor) {
		enemyCounter += 1;
	}
	if (y - 1 >= 0 && left >= 0 && placement[y - 1][left].x == left && placement[y - 1][left].kolor != kolor) {
		enemyCounter += 1;
	}
	if (x - mapX - 1 >= 0 && up >= 0 && placement[up][x-mapX-1].y == up && placement[up][x-mapX-1].kolor != kolor) {
		enemyCounter += 1;
	}
	if (x - mapX - 1 >= 0 && down <= sizeY * 2 + 1 && placement[down][x-mapX-1].y == down && placement[down][x-mapX-1].kolor != kolor) {
		enemyCounter += 1;
	}
	if (enemyCounter == 4) {
			if (*timer <= 0 && \
				placement[y - 1][right].x == right && \
				placement[y - 1][right].kolor != kolor && \
				placement[y - 1][right].killedInOneMove) {
				kill(placement, right, y - 1, WScore, GScore, true);
				*timer = 2;
				return true;
			}
			else if (*timer <= 0 && \
				placement[y - 1][left].x == left && \
				placement[y - 1][left].kolor != kolor && \
				placement[y - 1][left].killedInOneMove) {
				kill(placement, left, y - 1, WScore, GScore, true);
				*timer = 2;
				return true;
			}
			else if (*timer <= 0 && \
				placement[up][x - mapX - 1].y == up && \
				placement[up][x - mapX - 1].kolor != kolor && \
				placement[up][x - mapX - 1].killedInOneMove) {
				kill(placement, x - mapX - 1, up, WScore, GScore, true);
				*timer = 2;
				return true;
			}
			else if (*timer <= 0 && \
				placement[down][x - mapX - 1].y == down && \
				placement[down][x - mapX - 1].kolor != kolor && \
				placement[down][x - mapX - 1].killedInOneMove) {
				kill(placement, x - mapX - 1, down, WScore, GScore, true);
				*timer = 2;
				return true;
			}
			else {
				enemyCounter = 0;
				return false;
		}
	}
	else if (enemyCounter == 3) {
		if (left < 0 || \
			up<0 || \
			right >sizeX * 2 + 1 || \
			down>sizeY * 2 + 1) {
			enemyCounter = 0;
			return false;
		}
	}
	else if (enemyCounter == 2) {
		if ((left < 0 && down > sizeY * 2 + 1) || \
			(left < 0 && up < 0) || \
			(right > sizeX * 2 + 1 && up < 0) || \
			(right > sizeX * 2 + 1 && down > sizeY * 2 + 1)) {
			enemyCounter = 0;
			return false;
		}
	}
	enemyCounter = 0;
	return true;
}

void setGroups(stone** placement, int height, int width, int* groupXSize, int* groupYSize, double* WScore, int* GScore) {
	int idx = 0, idy = 0;
	stone* lastFromGroupX = NULL;
	stone* lastFromGroupY = NULL;

	for (int j = 0; j < width; j++) {
		idy = 0;
		*groupYSize = 0;
		lastFromGroupY = NULL;
		for (int i = 0; i < height; i++) {
			if (i < height - 1) {
				if (placement[i][j].x != 1500 && placement[i][j].kolor == placement[i + 2][j].kolor) {
					groupsY[idy] = placement[i][j];
					groupsY[idy + 1] = placement[i + 2][j];
					*groupYSize = *groupYSize + 1;
					idy++;
					lastFromGroupY = &groupsY[idy];
				}
				if (lastFromGroupY != NULL && i % 2 == 0 && ((*lastFromGroupY).kolor != placement[i + 2][j].kolor || (*lastFromGroupY).y == height - 1)) {
					*groupYSize += 1;
					checkIfGroupDies(placement, height, width, WScore, GScore, 0);
					lastFromGroupY = NULL;
					*groupYSize = 0;
					idy = 0;
					initGroupY();
				}
			}
		}
	}


	for (int i = 0; i < height; i++) {
		idx = 0;
		*groupXSize = 0;
		lastFromGroupX = NULL;
		for (int j = 0; j < width; j++) {
			if (j < width - 1) {
				if (placement[i][j].x != 1500 && placement[i][j].kolor == placement[i][j+2].kolor) {
					groupsX[idx] = placement[i][j];
					groupsX[idx + 1] = placement[i][j+2];
					*groupXSize = *groupXSize + 1;
					idx++;			
					lastFromGroupX = &groupsX[idx];
				}
				if (lastFromGroupX != NULL && j%2==0 && ((* lastFromGroupX).kolor != placement[i][j+2].kolor || (*lastFromGroupX).x == width-1)) {
					*groupXSize += 1;
					checkIfGroupDies(placement, height, width, WScore, GScore, 1);
					lastFromGroupX = NULL;
					*groupXSize = 0;
					idx = 0;
					initGroupX();
				}
			}
		}
	}
	lastFromGroupX = NULL;
	lastFromGroupY = NULL;
	idx = 0;
	idy = 0;
}

void checkIfGroupDies(stone** placement, int height, int width, double* WScore, int* GScore, int direction) {
	int enemyCounter = 0, x = 0, y = 0, kolor = 0;
	bool firstIsOxO = false, lastIsXxO = false, firstIsOxY = false, lastIsXxY = false;
	if (direction == 1) {
		for (int i = 0; i < groupXSize; i++) {
			x = groupsX[i].x;
			y = groupsX[i].y;
			if (x == 0 && y == 0) firstIsOxO = true;
			else if (x == 0 && y == height - 1) firstIsOxY = true;
			else if (x == width - 1 && y == 0) lastIsXxO = true;
			else if (x == width - 1 && y == height - 1) lastIsXxY = true;

			kolor = groupsX[i].kolor;
			if (i == 0) {
					if (x != 0 && placement[y][x - 2].x != 1500 && placement[y][x - 2].kolor != kolor) enemyCounter++;
					if (y != 0 && placement[y - 2][x].x != 1500 && placement[y - 2][x].kolor != kolor) enemyCounter++;
					if (y != height - 1 && placement[y + 2][x].x != 1500 && placement[y + 2][x].kolor != kolor) enemyCounter++;
			}
			else if (i == groupXSize - 1) {
					if (x != width - 1 && placement[y][x + 2].x != 1500 && placement[y][x + 2].kolor != kolor) enemyCounter++;
					if (y != 0 && placement[y - 2][x].x != 1500 && placement[y - 2][x].kolor != kolor) enemyCounter++;
					if (y != height - 1 && placement[y + 2][x].x != 1500 && placement[y + 2][x].kolor != kolor) enemyCounter++;
			}
			else {
					if (y != 0 && placement[y - 2][x].x != 1500 && placement[y - 2][x].kolor != kolor) enemyCounter++;
					if (y != height - 1 && placement[y + 2][x].x != 1500 && placement[y + 2][x].kolor != kolor) enemyCounter++;
			}
		}
		if (enemyCounter == groupXSize * 2 + 2) {
			for (int i = 0; i < groupXSize; i++)
				kill(placement, groupsX[i].x, groupsX[i].y, WScore, GScore, true);
		}
		else if (enemyCounter == groupXSize + 2 && (y - 2 < 0 || y + 2 > height)) {
			for (int i = 0; i < groupXSize; i++)
				kill(placement, groupsX[i].x, groupsX[i].y, WScore, GScore, true);
		}
		else if (enemyCounter == groupXSize + 1 && (firstIsOxO || lastIsXxY || firstIsOxY || lastIsXxO)) {
			for (int i = 0; i < groupXSize; i++)
				kill(placement, groupsX[i].x, groupsX[i].y, WScore, GScore, true);
		}
		firstIsOxO = false;
		firstIsOxY = false;
		lastIsXxY = false;
		lastIsXxO = false;
	}
	else {
		for (int i = 0; i < groupYSize; i++) {
			x = groupsY[i].x;
			y = groupsY[i].y;
			if (x == 0 && y == 0) firstIsOxO = true;
			else if (x == 0 && y == height - 1) firstIsOxY = true;
			else if (x == width - 1 && y == 0) lastIsXxO = true;
			else if (x == width - 1 && y == height - 1) lastIsXxY = true;
			kolor = groupsY[i].kolor;
			if (i == 0) {
					if (y != 0 && placement[y-2][x].x != 1500 && placement[y-2][x].kolor != kolor) enemyCounter++;
					if (x != 0 && placement[y][x-2].x != 1500 && placement[y][x-2].kolor != kolor) enemyCounter++;
					if (x != width - 1 && placement[y][x+2].x != 1500 && placement[y][x+2].kolor != kolor) enemyCounter++;
			}
			else if (i == groupYSize - 1) {
					if (y != height - 1 && placement[y+2][x].x != 1500 && placement[y+2][x].kolor != kolor) enemyCounter++;
					if (x != 0 && placement[y][x-2].x != 1500 && placement[y][x-2].kolor != kolor) enemyCounter++;
					if (x != width - 1 && placement[y][x+2].x != 1500 && placement[y][x+2].kolor != kolor) enemyCounter++;
			}
			else {
					if (x != 0 && placement[y][x - 2].x != 1500 && placement[y][x - 2].kolor != kolor) enemyCounter++;
					if (x != width - 1 && placement[y][x + 2].x != 1500 && placement[y][x + 2].kolor != kolor) enemyCounter++;
			}
		}
		if (enemyCounter == groupYSize * 2 + 2) {
			for (int i = 0; i < groupYSize; i++)
				kill(placement, groupsY[i].x, groupsY[i].y, WScore, GScore, true);
		}
		else if (enemyCounter == groupYSize + 2 && (x - 2 < 0 || x+2 > width)) {
			for (int i = 0; i < groupYSize; i++) {
				kill(placement, groupsY[i].x, groupsY[i].y, WScore, GScore, true);
			}
		}
		else if (enemyCounter == groupYSize + 1 && (firstIsOxO || lastIsXxY || firstIsOxY || lastIsXxO)) {
			for (int i = 0; i < groupYSize; i++)
				kill(placement, groupsY[i].x, groupsY[i].y, WScore, GScore, true);
		}
		firstIsOxO = false;
		firstIsOxY = false;
		lastIsXxY = false;
		lastIsXxO = false;
	}
}

void endGame(int mapX, int mapY, double* WScore, int* GScore) {
	char txt[32];
	clrscr();
	textcolor(LIGHTGREEN);
	gotoxy(mapX + 9, mapY - 5);
	cputs("Game Over");
	gotoxy(mapX, mapY);
	cputs("White Score");
	gotoxy(mapX + 5, mapY + 1);
	if (fmod(*WScore, 1.0) == 0) sprintf(txt, "%d", *WScore);
	else sprintf(txt, "%.1f", *WScore);
	cputs(txt);
	gotoxy(mapX + 15, mapY);
	cputs("Green Score");
	gotoxy(mapX + 20, mapY + 1);
	sprintf(txt, "%d", *GScore);
	cputs(txt);
	gotoxy(mapX + 9, mapY + 6);
	if (*WScore > *GScore) cputs("White Won");
	else cputs("Green Won");
	*WScore = 0;
	*GScore = 0;
}