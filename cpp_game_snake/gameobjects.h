#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

/**
  * TODO: update() function must be optimized
  */

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define MAPSIZE		20							// 20x20 playable area
#define MAX_BODY 	(MAPSIZE*MAPSIZE)			// max body length

using namespace std;

enum Direction{UP=0, RIGHT=1, DOWN=2, LEFT=3};
enum MapBlock{WALL_BLOCK=0, EMPTY_BLOCK=1, SNAKE_BLOCK=2, FAT_BLOCK=3, FOOD_BLOCK=4};
typedef struct{ int i,j; MapBlock type; }BLOCK;

// Ready const for moving in matrix like robots
const int icoord[] = {-1, 0, +1, 0};
const int jcoord[] = {0, +1, 0, -1};

// function implements
int mod(int, int);
void gotoxy(short, short);


class Snake										
{
	private:
		int head, rear, len;					// head,rear values are need for FIFO algorithm
		BLOCK body[MAX_BODY];					// Coordinates of snake's body
		Direction direction;					// Direction of snake
	public:
		int delay; 								// Delay time (in ms)
		Snake(int, int, int, Direction, int);	// Constructor. @Param: (length, i, j, dir, delay).							
		MapBlock move(Direction, MapBlock**);	// Snake, tries to move that direction. 
		int isCollide(int, int);				// @Param: (i,j coordinates). If snake collides with this coord, return 1 else return 0.
		BLOCK getBlock(int);					// 0 for rear, len-1 for head
		int getLength();
		Direction getDirection();
};


class Food								
{
	private:
		BLOCK f;
	public:
		BLOCK getBlock();						// returns food block
		void setCoord(int, int);				// sets food blocks coordinates
};


class Map									
{
	private:
		MapBlock **map, **oldmatrix;			// MxN matrix
		Snake *snake;							// Snake pointer
		Food *food;								// Food  pointer
	public:
		int M,N;								// MxN -> RowXColumn of matrix
		Map(int, int, Snake&, Food&);			// @Param: (M, N, _snake, _food)
		~Map();
		void updateMap();						// Updates matrix
		void paintMap();						// Paints matrix to the screen     (TODO!)
		MapBlock** getMap();
};

class GameController
{
	private:									// Holds snake, food and map pointer
		Snake *snake;
		Food *food;
		Map *map;
	public:
		int play, point;						// play: continuegame?  |  point: game point
		GameController(Snake*, Food*, Map*);	// constructor
		void tick(char);						// @Param: Key:Pressed button    Ticks the game. 
		
};

typedef struct{
	int distance; MapBlock type;
}DetectedObj;

class AI
{
	private:
		Direction lastMove;
		int doubleMove;
	public:
		AI();
		MapBlock look(Snake*, Map*, Direction, int*);			// Looks from snake's head. Returns first had seen MapBlock in that direction
		char control(Snake*, Map*);								// Decides where to go. Return keyboard character (w/a/s/b)
	
};

#endif
