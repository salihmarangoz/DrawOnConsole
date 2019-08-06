/**
  * 	@Author		:	Salih Marangoz - salih285@gmail.com - www.indirgecim.tr.gg
  * 	@Date		:	xx.xx.xxxx - xx:xx */
#define VERSION 		"v1.3 (included AI)"
/**
  * 	@Definition	:	Simple snake game. Includes map, snake and a food.
  *
  *		@Note		:	Algorithm can be used in 2D (not maze) platform. For ex. turning with degree snake! (TODO: icoord,jcoord->degree)
  */
  
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include "gameobjects.h"

// settings:
#define S_LENGTH	3
#define S_DELAY 	500
#define AI_DELAY	40

using namespace std;

void setCursorVisible(int);

int main(int argc, char** argv)
{	
	// reset rand table
	srand(time(NULL));
	
	// Create snake, food. Add them in to map. And at last add all of them in to gamecontroller
	Snake s = Snake(S_LENGTH, 3, 3, RIGHT, S_DELAY);
	Food f = Food();
	Map m = Map(MAPSIZE, MAPSIZE, s, f);
	GameController game = GameController(&s, &f, &m);
	
	// Set AI
	AI ai = AI();
	
	// Variables
	int tmp, set_ai = -1;
	char key;
	
	// get set_ai parameter from user
	cout << "SNAKE GAME " << VERSION << " | Salih M.\nPlay with w/a/s/d\n\nPress A to play manuel, B to play automatic.\n";
	do
	{
		key = getch();
		switch(tolower(key))
		{
			case 'a': set_ai = 0; break;
			case 'b': set_ai = 1; s.delay = AI_DELAY; break;
		}
	} while (set_ai == -1);
	
	// timer and key variables
	time_t t = clock();
	key = 'd';
	
	// stop cursor blinking
	setCursorVisible(0);

	// show map first!
	system("cls");
	m.updateMap();
	m.paintMap();

	// game starts!
	while(game.play)
	{
		// save last key hit in variable
		if (kbhit())
			key = getch();
			
		// if snake have to move, go in!
		if (clock() - t >= s.delay)
		{
			// tick the game. This func also updates the map.
			
			if (set_ai == 1)
				game.tick(ai.control(&s, &m));
			else
				game.tick(key);
			
			// paint!
			m.paintMap();
			
			// print string!
			printf("Points: %d\nlength: %d", game.point,  s.getLength());
			
			// reset timer!
			t = clock();
		}
		
		Sleep(1);
		
	}
	
	
	
	cout << "\nGAME OVER!!!";
	system("pause");

	return 0;
}



void setCursorVisible(int visible){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursoInfo;
	CursoInfo.dwSize = 1;
	CursoInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &CursoInfo);
}
