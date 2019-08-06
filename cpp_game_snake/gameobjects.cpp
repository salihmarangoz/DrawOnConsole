#include "gameobjects.h"

/*------------------------------------ SNAKE ---------------------------------------------------------*/
Snake::Snake(int length, int start_i, int start_j, Direction start_direction, int delay_)		// TESTED OK!
{	
	
	int x; // temp
	
	// set start values
	rear = 0;
	head = length-1;
	direction = start_direction;
	len = length;
	delay = delay_;
	
	// set snake body
	for (x=0; x<length; x++)
	{
		body[x].i = x * icoord[direction] + start_i;
		body[x].j = x * jcoord[direction] + start_j;
		body[x].type = SNAKE_BLOCK;
	}
}

MapBlock Snake::move(Direction d, MapBlock **map)	
{			
	// set new direction
	direction = d;
	MapBlock temp;
	
	// new coords
	int newi, newj;
	newi = body[ mod(head, MAX_BODY)].i + icoord[d];
	newj = body[ mod(head, MAX_BODY)].j + jcoord[d];
	
	temp = map[newi+1][newj+1];
	
	
	//ADDED! dont collide with bad situations
	if (!(temp==EMPTY_BLOCK || temp==FOOD_BLOCK))
		return temp;
	
	// set new head bodypart
	body[mod(head+1, MAX_BODY)].i = newi;
	body[mod(head+1, MAX_BODY)].j = newj;
	
	// move head straight the stack
	head = mod(head+1, MAX_BODY);
	
	// move rear staight the stack. If eaten food on rear body, make snake longer
	if (body[rear].type == FAT_BLOCK)
	{
		body[rear].type = SNAKE_BLOCK;
		len++;
	}
	else
		rear = mod(rear+1, MAX_BODY);
	
	// if ate food!
	if (temp == FOOD_BLOCK)
		body[head].type = FAT_BLOCK;
	else
		body[head].type = SNAKE_BLOCK;
	
	return temp;
}

int Snake::isCollide(int i_, int j_)
{	// TESTED OK!
	int a;
	for (a=0; a<len; a++)
		if (getBlock(a).i == i_ && getBlock(a).j == j_)
			return 1;
	return 0;
}

BLOCK Snake::getBlock(int b)
{			// TESTED OK!
	return body[ mod(b+rear, MAX_BODY) ];
}

int Snake::getLength()
{					// TESTED OK!
	return len;
}
	
Direction Snake::getDirection()
{
	return direction;
}



/*------------------------------------ FOOD ----------------------------------------------------------*/
void Food::setCoord(int i_, int j_){
	f.i = i_;
	f.j = j_;
}

BLOCK Food::getBlock()
{
	return f;
}



/*------------------------------------ MAP -----------------------------------------------------------*/
Map::Map(int row, int column, Snake &s, Food &f)			// TESTED OK !
{
	int i,j;	// temp
	
	// add snake and food to map
	snake = &s;
	food = &f;
	
	// init matrix size
	M = row;
	N = column;
	
	MapBlock **m;
	m = (MapBlock**)malloc((M+2) * sizeof(MapBlock*));
	for (i=0; i<M+2; i++)
		m[i] = (MapBlock*)malloc((N+2) * sizeof(MapBlock));
	map = m;
	
	MapBlock **oldm;
	oldm = (MapBlock**)malloc((M+2) * sizeof(MapBlock*));
	for (i=0; i<M+2; i++)
		oldm[i] = (MapBlock*)calloc((N+2), sizeof(MapBlock));
	oldmatrix = oldm;
	
	// Init map
	for (i=0; i<N+1; i++)		// right-left walls
	{
		map[0][i] = WALL_BLOCK;
		map[M+1][i] = WALL_BLOCK;
	}
	for (i=0; i<M+1; i++)		// top-bottom walls
	{
		map[i][0] = WALL_BLOCK;
		map[i][N+1] = WALL_BLOCK;
	}
	
	for (i=1; i<=M; i++)
		for (j=1; j<=N; j++)
			map[i][j] = EMPTY_BLOCK;
			
	for (i=0; i<=M+1; i++)
		for (j=0; j<=N+1; j++)
			oldmatrix[i][j] = EMPTY_BLOCK;
}

Map::~Map(){
	int i;
	for (i=0; i<M+2; i++)
		free (map[i]);
	free(map);
	for (i=0; i<M+2; i++)
		free (oldmatrix[i]);
	free(oldmatrix);
}

void Map::updateMap(){
	int i,j;
	for (i=1; i<M+1; i++)
		for (j=1; j<N+1; j++)
			map[i][j] = EMPTY_BLOCK;	
	for (i=0; i<snake->getLength(); i++)
		map[ snake->getBlock(i).i +1 ][ snake->getBlock(i).j +1 ] = snake->getBlock(i).type;   // +1 because of walls
	map[food->getBlock().i][food->getBlock().j] = FOOD_BLOCK;
}


void Map::paintMap(){
	int i,j;	// temp
	
	char charset[] = {'<', ' ', 'o', 'O', 'x'};	// charset
	
	// clear screen
	gotoxy(0,0);
	
	// print matrix
	for (i=0; i<M+2; i++)
	{
		for (j=0; j<N+2; j++)
		{
			if (oldmatrix[i][j] != map[i][j])
			{
				gotoxy(j*2,i);
				printf("%c ", charset[map[i][j]]);
			}
			oldmatrix[i][j] = map[i][j];
		}
		putchar('\n');
	}
	
}

MapBlock** Map::getMap(){
	return map;
}



/*---------------------------------- GameController --------------------------------------------------*/

GameController::GameController(Snake *s, Food *f, Map *m){
	snake = s;
	food = f;
	map = m;
	play=1;
	point = 0;
	
	// random food
	int rndi, rndj;
	do
	{
		rndi = rand() % m->M + 1;
		rndj = rand() % m->N + 1;
	}while (s->isCollide(rndi, rndj));
	f->setCoord(rndi, rndj);
	
}

void GameController::tick(char key){
	
	Direction d;
	MapBlock b;
	
	// switch key cases
	switch(key){
		case 'w': case 'W': d = UP; break;
		case 'd': case 'D': d = RIGHT; break;
		case 's': case 'S': d = DOWN; break;
		case 'a': case 'A': d = LEFT; break;
	}
	
	// if snake tries to  go back, fix this
	if (mod(d - snake->getDirection(), 4) == 2)
		d = snake->getDirection();
	
	b = snake->move(d, map->getMap());
	if (b != EMPTY_BLOCK && b != FOOD_BLOCK)
		play=0;
		
	if (b == FOOD_BLOCK)
	{
		// increase point and lower delay
		point += 10;
		//snake->delay *= 0.9;   TODO: fixed delay for testing AI
		
		// random food
		int rndi, rndj;
		do
		{
			rndi = rand() % map->M + 1;
			rndj = rand() % map->N + 1;
		} while ((map->getMap())[rndi][rndj] != EMPTY_BLOCK);
		food->setCoord(rndi, rndj);		
	}
	
	map->updateMap();
}

/*-------------------------------------- AI ----------------------------------------------------------*/

AI::AI()
{
	lastMove = RIGHT;
	doubleMove = 0;
}

MapBlock AI::look(Snake *s, Map *m, Direction d, int *distance)
{
	d = (Direction)((d + s->getDirection()) % 4);
	
	BLOCK head = s->getBlock(s->getLength()-1);
	
	int i=0;
	do
	{
		i++;		// distance not tested!
	} while ( (m->getMap())[ head.i + icoord[d]*i + 1 ][ head.j + jcoord[d]*i + 1 ] == EMPTY_BLOCK);
	
	*distance = i;
	
	return (m->getMap())[ head.i + icoord[d]*i + 1 ][ head.j + jcoord[d]*i + 1 ];
}

char AI::control(Snake *s, Map *m)
{
	DetectedObj output[4];
	char returnset[] = {'w', 'd', 's', 'a'};
	int i, tmp;
	Direction currentd = s->getDirection();
	Direction tmp_d;
	int decided=0;
	
	// look around and save data!
	output[RIGHT].type = look(s, m, RIGHT, &tmp);
	output[RIGHT].distance = tmp;
	output[LEFT].type = look(s, m, LEFT, &tmp);
	output[LEFT].distance = tmp;	
	output[UP].type = look(s, m, UP, &tmp);
	output[UP].distance = tmp;
		
	// algorithm start here!!!
	
	if (doubleMove == 1 && output[lastMove].distance != 1)
	{
		tmp_d = lastMove;
		doubleMove = 0;
	}
	else if (output[LEFT].type == FOOD_BLOCK)
		tmp_d = LEFT;
	else if (output[RIGHT].type == FOOD_BLOCK)
		tmp_d = RIGHT;
	else if (output[RIGHT].type == SNAKE_BLOCK && output[RIGHT].distance == 1 && output[LEFT].type == WALL_BLOCK && output[LEFT].distance > 1)
		tmp_d = LEFT;
	else
	{
		if (output[UP].distance > 1 || output[UP].type == FOOD_BLOCK)
		{
			if (output[RIGHT].type == FOOD_BLOCK)														// yemek sagdaysa saga git
				tmp_d = RIGHT;
			else if (output[LEFT].type == FOOD_BLOCK)													// yemek soldaysa sola git
				tmp_d = LEFT;
			else																						// yoksa duz devam et
				tmp_d = UP;	
		}
		else
		{
			if ((output[RIGHT].type != FOOD_BLOCK || output[RIGHT].type != WALL_BLOCK) && (output[LEFT].type != FOOD_BLOCK || output[LEFT].type != WALL_BLOCK))
			{
				tmp_d = (output[RIGHT].distance >= output[LEFT].distance ? RIGHT : LEFT);
				doubleMove = 1;
			}
			else if ((output[RIGHT].type != FOOD_BLOCK || output[RIGHT].type != WALL_BLOCK) || (output[LEFT].type != FOOD_BLOCK || output[LEFT].type != WALL_BLOCK))
			{
				if ((output[RIGHT].type != FOOD_BLOCK || output[RIGHT].type != WALL_BLOCK) && output[LEFT].distance > 1)
					tmp_d = LEFT;
				else if ((output[LEFT].type != FOOD_BLOCK || output[LEFT].type != WALL_BLOCK) && output[RIGHT].distance > 1)
					tmp_d = RIGHT;
			}
			else
				if (output[RIGHT].type == FOOD_BLOCK)														// yemek sagdaysa saga git
					tmp_d = RIGHT;
				else if (output[LEFT].type == FOOD_BLOCK)													// yemek soldaysa sola git
					tmp_d = LEFT;
				else																						
					tmp_d = (output[RIGHT].distance >= output[LEFT].distance ? RIGHT : LEFT);	
		}
	}
	
	if (tmp_d == RIGHT || tmp_d == LEFT)	// LEFT/RIGHT ise son hamleyi kaydet!
		lastMove = tmp_d;
			
	return returnset[ mod(currentd + tmp_d, 4) ];
}


/*-------------------------------------- OTHER -------------------------------------------------------*/
// Mod function: (defined because -5%20 must be 15) (Added for FIFO)
int mod(int a, int b)
{
	if (a<0)
		return b - (-a % b);
	else
		return a % b;
}

// move cursor
void gotoxy(short x, short y){ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ; 
    COORD position = { x, y } ; 
    SetConsoleCursorPosition( hStdout, position ) ; 
}
