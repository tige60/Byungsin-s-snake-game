#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

void MoveCursor(int x, int y);

// Forced to inject this DUMBASS parameter because the original code was never meant to be modular.
void DrawSnake(POINT *snake, unsigned char ch, int *apple, int *facing);

HANDLE hwd;

int main()
{
	unsigned char ch;
	int i;

	POINT tmp1, tmp2;
	POINT cursor;
	POINT random;
	POINT old_snake[10];
	POINT snake;

	int apple = 0;
	int facing = RIGHT;		// default
	
	snake.x = 10;
	snake.y = 10;

	random.x = rand() % 10;
	random.y = rand() % 10;

	hwd = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(NULL));

//	printf("random.x: %d\n", random.x);
//	printf("random.y: %d\n", random.y);	

	// wait "any key" (pause)
	getch();

	system("cls");
	while(1)
	{
		if (kbhit())
		{

			MoveCursor(random.x, random.y);
			printf("A");

			tmp1 = old_snake[0];
			old_snake[0] = snake;

//			for (i=1; i<apple; i++)
			for (i=1; i<10; i++)
			{
				tmp2 = old_snake[i];
				old_snake[i] = tmp1;
				tmp1 = tmp2;
			}

			ch = getch();
			if (ch == 224) { ch = getch(); }

			DrawSnake(&snake, ch, &apple, &facing);	
		}
	}
	
	return 0;
}

void MoveCursor(int x, int y)
{
	COORD cur;
	cur.X = x;
	cur.Y = y;
	SetConsoleCursorPosition(hwd, cur);
}

void DrawSnake(POINT *snake, unsigned char ch, int *apple, int *facing)
{
	switch(ch)
	{
	case 97:
		(*apple)++;
		break;
	case 27:
		ExitProcess(0);
	case UP:
		if (snake->y == 0) { break; }
		*facing = UP;
	
		MoveCursor(snake->x, snake->y + *apple);
		printf(" ");
	
		snake->y--;
		break;
	
	case DOWN:
		*facing = DOWN;
	
		MoveCursor(snake->x, snake->y - *apple);
		printf(" ");
	
		snake->y++;
		break;
				
	case RIGHT:
		*facing = RIGHT;
	
		MoveCursor(snake->x - *apple, snake->y);
		printf(" ");
	
		snake->x++;
		break;
				
	case LEFT:
		if (snake->x == 0) { break; }
	
		*facing = LEFT;
	
		MoveCursor(snake->x + *apple, snake->y);
		printf(" ");
				
		snake->x--;
		break;
	default:
		return;
	}


	MoveCursor(snake->x, snake->y);
	printf("S");

}