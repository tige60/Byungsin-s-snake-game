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

void DrawJail(int jail);

// Forced to inject this DUMBASS parameter because the original code was never meant to be modular.
// 오후 11:25 2026-06-20 In my think, it may be suitabler for "UpdateSnake" than DrawSnak
void DrawSnake(POINT *snake, unsigned char ch);

void UpdateSnake(POINT *snake, unsigned char ch, int *facing);

void ChangeApplePos(APPLE *apple, POINT snake_tails[100], int jail);

void RemoveTail(POINT snake_tails[100], APPLE *apple);

int IsSnakeEatApple(APPLE apple, POINT snake);

void GameOver();

HANDLE hwd;

int main(char argc, char* argv[])
{
	int jail = atoi(argv[1]);
	int facing = RIGHT;
	unsigned char ch = RIGHT;
	int i;

	hwd = GetStdHandle(STD_OUTPUT_HANDLE);

	POINT tmp1, tmp2;
	POINT snake_tails[100];
	POINT snake;
	APPLE apple;
	
	ch = facing;

	snake.x = 3;
	snake.y = 3;

	apple.cnt = 0;
	apple.x = rand() % 10;
	apple.y = rand() % 10;

	srand(time(NULL));

	// wait "any key" (pause)
	getch();

	system("cls");

	DrawJail(jail);
	while(1)
	{
		//Let's make to auto move!
		Sleep(100);


		// THX gpt

		if ( IsSnakeEatApple(apple, snake) ) 
			{	apple.cnt++;
				ChangeApplePos(&apple, snake_tails,jail); }

			SetConsoleTextAttribute(hwd, 12);

			MoveCursor(apple.x, apple.y);
			printf("A");

			SetConsoleTextAttribute(hwd, 15);

		if (kbhit())
		{
			ch = getch();
			if (ch == 224) { ch = getch(); }
		}


			tmp1 = snake_tails[0];
			snake_tails[0] = snake;

			for (i=1; i<apple.cnt+1; i++)
			{
				tmp2 = snake_tails[i];
				snake_tails[i] = tmp1;
				tmp1 = tmp2;
			}


			UpdateSnake(&snake, ch, &facing);

			for (i = 1; i <= apple.cnt; i++)
			{
				if (snake_tails[i].x == snake.x && snake_tails[i].y == snake.y)
				{
					GameOver();
				}
			}

			// IDK why cause a problem at ceiling so snake.y < 1
			if (snake.x < 0 || snake.y < 1)
			{
				GameOver();
			}

			if (snake.x >= jail) { GameOver(); }
			if ((snake.y+1) >= jail) { GameOver(); }

			DrawSnake(&snake, ch); 
			RemoveTail(snake_tails, &apple);
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

void ChangeApplePos(APPLE *apple, POINT snake_tails[100], int jail)
{
	int i;
	int overlap;
/*
	while (1) {
		apple->x = rand() % (jail - 2) + 1;
		apple->y = rand() % (jail - 2) + 1;
		for (i = 1; i <= apple->cnt; i++)
		{
			if (snake_tails[i].x == apple->x && snake_tails[i].y == apple->y)
			{
				continue;
			} else { break; }
		} }*/
	// Thanks chatgpt
	while (1)
	{
		overlap = 0;

		apple->x = rand() % (jail - 2) + 1;
		apple->y = rand() % (jail - 2) + 1;

		for (i = 1; i <= apple->cnt; i++)
		{
			if (snake_tails[i].x == apple->x &&
				snake_tails[i].y == apple->y)
			{
				overlap = 1;
				break;
			}
		}

		if (!overlap)
		{
			break;
		}
	}

	MoveCursor(apple->x, apple->y);
	putc(8, stdout);
}

void UpdateSnake(POINT *snake, unsigned char ch, int *facing)
{
	switch(ch)
	{
/*	case 97:
		(*apple)++;
		break;		*/
	case 27:
		ExitProcess(0);
	case DOWN:
		if (snake->y == 0) { break; }
		snake->y++;
		*facing = UP;

		break;
	
	case UP:
		snake->y--;
		*facing = DOWN;

		break;
				
	case RIGHT:
		snake->x++;
		*facing = RIGHT;
	
		break;
				
	case LEFT:
		if (snake->x == 0) { break; }
		snake->x--;
		*facing = LEFT;		
		
		break;
	default:
		return;
	}
}

void DrawSnake(POINT *snake, unsigned char ch)
{
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

void GameOver()
{
	printf("Game Over\n");
	ExitProcess(0);
}

void DrawJail(int jail)
{
	int i;	

	MoveCursor(0,0);
	for (i = 0; i < jail; i++) {printf("#");}

	for (i=1; i < jail-1; i++) {
	MoveCursor(jail, i);
	printf("#"); }

	printf("\n");
	for (i = 0; i < jail; i++) {printf("#");}
}