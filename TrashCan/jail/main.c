#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

HANDLE hwd;

void MoveCursor(int x, int y)
{
	COORD cur;
	cur.X = x;
	cur.Y = y;
	SetConsoleCursorPosition(hwd, cur);
}

int main(char argc, char* argv[])
{
	int jail = atoi(argv[1]);
	int i;

	hwd = GetStdHandle(STD_OUTPUT_HANDLE);

	for (i=0; i < jail-1; i++) {
	MoveCursor(jail, i);
	printf("#"); }

	printf("\n");
	for (i = 0; i < jail; i++) {printf("#");}
}
