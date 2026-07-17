#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

typedef struct _APPLE
{
	int cnt;
	int x;
	int y;
}APPLE;

void MoveCursor(int x, int y);

// Forced to inject this DUMBASS parameter because the original code was never meant to be modular.
// 오후 11:25 2026-06-20 In my think, it may be suitabler for "UpdateSnake" than DrawSnake
void DrawSnake(POINT *snake, unsigned char ch, APPLE *apple, int *facing);

void ChangeApplePos(APPLE *apple);

void RemoveTail(POINT snake_tails[100], APPLE *apple);

int IsSnakeEatApple(APPLE apple, POINT snake);

HANDLE hwd;

int main()
{
	FILE *fp = NULL;
	unsigned char ch;
	int i;

	POINT tmp1, tmp2;
	POINT snake_tails[100];
	POINT snake;
	APPLE apple;

	int facing = RIGHT;		// default
	
	snake.x = 10;
	snake.y = 10;

	apple.cnt = 0;
	apple.x = rand() % 10;
	apple.y = rand() % 10;

	hwd = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(NULL));

	// wait "any key" (pause)
	getch();

	fp = fopen("logfile.log", "r");

	system("cls");
	while(1)
	{
		if (kbhit())
		{
			if ( IsSnakeEatApple(apple, snake) ) 
			{	apple.cnt++;
				fprintf(fp, "%s %d", "apple: ", apple.cnt);
				ChangeApplePos(&apple); }

			fprintf(fp, "%s %d %d", "snake: ", snake.x, snake.y);

			SetConsoleTextAttribute(hwd, 12);

			MoveCursor(apple.x, apple.y);
			printf("A");

			SetConsoleTextAttribute(hwd, 15);

			tmp1 = snake_tails[0];
			snake_tails[0] = snake;

			for (i=1; i<apple.cnt+1; i++)
			{
				tmp2 = snake_tails[i];
				snake_tails[i] = tmp1;
				tmp1 = tmp2;
			}

			ch = getch();
			if (ch == 224) { ch = getch(); }

			for (i=1; i < apple.cnt; i++)
			{
				if (snake_tails[i].x == snake.x && snake_tails[i].y == snake.y)
				{
					printf("Game Over\n");
					ExitProcess(0);
				}
			}

			DrawSnake(&snake, ch, &apple, &facing);
			RemoveTail(snake_tails, &apple);
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

int IsSnakeEatApple(APPLE apple, POINT snake)
{
	if (apple.x == snake.x && apple.y == snake.y) 
	{ return 1; }
	else { return 0; }
}

void ChangeApplePos(APPLE *apple)
{
	MoveCursor(apple->x, apple->y);
	putc(8, stdout);
	apple->x = rand() % 10;
	apple->y = rand() % 10;
}

void DrawSnake(POINT *snake, unsigned char ch, APPLE *apple, int *facing)
{
	switch(ch)
	{
/*	case 97:
		(*apple)++;
		break;		*/
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

	switch(ch)
	{
	case RIGHT:
		printf(">");
		break;
	case LEFT:
		printf("<");
		break;
	case UP:
		printf("^");
		break;
	case DOWN:
		printf("v");
		break;
	}
}

void RemoveTail(POINT snake_tails[100], APPLE *apple)
{
	int i = apple->cnt;
	MoveCursor(snake_tails[i].x , snake_tails[i].y);
	printf(" ");
}