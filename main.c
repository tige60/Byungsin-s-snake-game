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
// 오후 11:25 2026-06-20 In my think, it may be suitabler for "UpdateSnake" than DrawSnake
void DrawSnake(POINT *snake, unsigned char ch, int *apple, int *facing);

void ChangeApplePos(POINT *apple_pos);

void RemoveTail(POINT snake_tails[100], int apple);

int IsSnakeEatApple(POINT apple_pos, POINT snake);

HANDLE hwd;

int main()
{
	unsigned char ch;
	int i;

	POINT tmp1, tmp2;
	POINT apple_pos;
	POINT snake_tails[100];
	POINT snake;

	int apple = 0;
	int facing = RIGHT;		// default
	
	snake.x = 10;
	snake.y = 10;

	apple_pos.x = rand() % 10;
	apple_pos.y = rand() % 10;

	hwd = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(NULL));

	// wait "any key" (pause)
	getch();

	system("cls");
	while(1)
	{
		if (kbhit())
		{
			if ( IsSnakeEatApple(apple_pos, snake) ) 
			{ apple++;
			  ChangeApplePos(&apple_pos); }

			MoveCursor(apple_pos.x, apple_pos.y);
			printf("A");

			tmp1 = snake_tails[0];
			snake_tails[0] = snake;

			for (i=1; i<apple+1; i++)
			{
				tmp2 = snake_tails[i];
				snake_tails[i] = tmp1;
				tmp1 = tmp2;
			}

			ch = getch();
			if (ch == 224) { ch = getch(); }

			DrawSnake(&snake, ch, &apple, &facing);
			RemoveTail(snake_tails, apple);
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

int IsSnakeEatApple(POINT apple_pos, POINT snake)
{
	if (apple_pos.x == snake.x && apple_pos.y == apple_pos.y) 
	{ return 1; }
	else { return 0; }
}

void ChangeApplePos(POINT *apple_pos)
{
	MoveCursor(apple_pos->x, apple_pos->y);
	putc(8, stdout);
	apple_pos->x = rand() % 10;
	apple_pos->y = rand() % 10;
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
	
		snake->y--;
		break;
	
	case DOWN:
		*facing = DOWN;
	
		snake->y++;
		break;
				
	case RIGHT:
		*facing = RIGHT;	

		snake->x++;
		break;
				
	case LEFT:
		if (snake->x == 0) { break; }
	
		*facing = LEFT;
					
		snake->x--;
		break;
	default:
		return;
	}


	MoveCursor(snake->x, snake->y);
	printf("S");

}

void RemoveTail(POINT snake_tails[100], int apple)
{
	int i = apple;
	MoveCursor(snake_tails[i].x , snake_tails[i].y);
	printf(" ");
}