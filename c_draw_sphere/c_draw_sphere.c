#include <stdio.h>
#include <math.h>

#define MAX_ROW		45
#define MAX_COLUMN	39

int isInRange(int i, int j)
{
	if (i >= MAX_ROW)
		return 0;
	else if (i < 0 )
		return 0;
	else if (j >= MAX_COLUMN)
		return 0;
	else if (j < 0)
		return 0;
	else
		return 1;
}

// $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.        "BLACK TO WHITE"
void printMatrix(int matrix[MAX_ROW][MAX_COLUMN])
{
	const char charset[] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };
	//const char charset[] = {' ', ':', '1', 'I', '?', 'u', 'X', 'C', 'Q', 'Z', '#', 'M', 'B', '@', '$'};
	int i,j;
	for (i=0; i<MAX_ROW; i++)
	{
		for (j=0; j<MAX_COLUMN; j++)
			if (matrix[i][j] == 0)
				printf("  ");
			else
				printf("%c ", charset[ matrix[i][j] ]);
				
		printf("\n");
	}
	
}

// mx: merkez x,   my: merkez y,    r: yarýcap,     matrix: degisikligin yapilacagi matrix
void drawCircle(int mx, int my, int r, int matrix[MAX_ROW][MAX_COLUMN])
{
	/*
		x^2+y^2=r^2
		y = sqrt(r^2-(x-r)^2)
	*/
	int i,j,y, iaim, jaim;
	for (i=0; i<=r*2; i++)
	{
		// alt egriyi hesapla
		y = sqrt(r*r - (i-r)*(i-r));
		
		// alt egriden itibaren yukarýya dogru matrisi doldur
		for (j=0; j<=(y)*2; j++)
		{
			iaim = lround(y) + my - j;
			jaim = i + mx - r;
			if (isInRange(iaim, jaim))
				matrix[ iaim ][ jaim ] += 1;
		}	
	}
}


int main(){
	int matrix[MAX_ROW][MAX_COLUMN] = {0};
	
	// ust uste 15 adet daire ciz, boylece 3 boyutlu hissi verecek.
	int i;
	for (i=0; i<10; i++)
		drawCircle(29-i, 10+i, i*2, matrix);
	
	printMatrix(matrix);
	getch();
}
