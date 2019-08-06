#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define SPEED (50) // 20fps max
#define USE_CHARACTERSET (1)
#define SCREENX (80)
#define SCREENY (45)

int buffer = 0;

int isInRange(int i, int j);
void dxcircle(int SCREEN[SCREENY][SCREENX][2], int mx, int my, int r);
void dxprint(int SCREEN[SCREENY][SCREENX][2], int force);
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]);
void setColorAndBackground(int ForgC, int BackC);
void gotoXY(short x, short y);
void setCursorVisible(bool visible);


int main(){
    int SCREEN[SCREENY][SCREENX][2];

    setCursorVisible(false);

    int uzaklikcarpan=1;
    int i;

    // animasyon!
    int getRadius(){
        static int a = 2, delta=1;

        if (a>9){
            delta = -delta;
        }
        else if (a<2){
            delta = -delta;
        }

        a += delta;

        return a;
    }

    while (1){
        // sol ust koseden saga git
        for (i=0; i<20; i++){
            dxzerofill(SCREEN);
            dxcircle(SCREEN, i+10, 10, getRadius());
            dxprint(SCREEN, 0);
            Sleep(SPEED);
        }

        // asagi git
        for (i=0; i<30; i++){
            dxzerofill(SCREEN);
            dxcircle(SCREEN, 30, i+10, getRadius());
            dxprint(SCREEN, 0);
            Sleep(SPEED);
        }

        // sola git
        for (i=0; i<20; i++){
            dxzerofill(SCREEN);
            dxcircle(SCREEN, 30-i, 40, getRadius());
            dxprint(SCREEN, 0);
            Sleep(SPEED);
        }

        // yukari git
        for (i=0; i<30; i++){
            dxzerofill(SCREEN);
            dxcircle(SCREEN, 10, 40-i, getRadius());
            dxprint(SCREEN, 0);
            Sleep(SPEED);
        }

    }

    return 0;
}

/* ************************************************************************************************************************************** */

int isInRange(int i, int j){
    if (i >= SCREENY)
        return 0;
    else if (i < 0 )
        return 0;
    else if (j >= SCREENX /2)
        return 0;
    else if (j < 0)
        return 0;
    else
        return 1;
}

// mx: merkez x,   my: merkez y,    r: yaricap,     matrix: degisikligin yapilacagi matrix
void dxcircle(int SCREEN[SCREENY][SCREENX][2], int mx, int my, int r){
    /*
        x^2+y^2=r^2
        y = sqrt(r^2-(x-r)^2)
    */
    int i,j,k,y, iaim, jaim, orjr=r;

    for (k=1; k<=10; k++){

        r = orjr * ((double)k / (double)10);

        // daire ciz
        for (i=0; i<=r*2; i++)
        {
            // alt egriyi hesapla
            y = sqrt(r*r - (i-r)*(i-r));

            // alt egriden itibaren yukariya dogru matrisi doldur
            for (j=0; j<=(y)*2; j++)
            {
                iaim = lround(y) + my - j;
                jaim = i + mx - r;
                if (isInRange(iaim, jaim))
                    SCREEN[ iaim ][ jaim*2 ][buffer] += 1;
            }
        }
        
    }
}


void dxprint(int SCREEN[SCREENY][SCREENX][2], int force){

    int i,j;
    const char charset[] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };
    const char richcharset[] = " .,:Il!i><+_-?][}{1)(/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    gotoXY(0,0);
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            if (SCREEN[i][j][0] != SCREEN[i][j][1] || force){ // sadece degisen yerleri yazacak
                gotoXY(j,i);
                if (SCREEN[i][j][buffer] != 0)
                    if (USE_CHARACTERSET)
                        putchar(richcharset[  SCREEN[i][j][buffer]  ]);
                    else
                        putchar('.');
                else
                    putchar(' ');
            }
    gotoXY(0,46);
    
    buffer = !buffer;
}

void dxzerofill(int SCREEN[SCREENY][SCREENX][2]){
    int i,j;
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            SCREEN[i][j][buffer] = 0;
}

/* ************************************************************************************************************************************** */

/**
  * @PARAM:     ForgC   yazinin renk kodu
  *             BackC   yazinin arkaplan renk kodu
  *
  * @OUTPUT:    Konsola printf ile yazilan yazilarin renk olaylarina etki eder.
  */
void setColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

/**
  * @PARAM:     x       ekranda gidilecek noktanin x koordinati
  *             y       ekranda gidilecek noktanin y koordinati
  *
  * @OUTPUT:    Imleci konsol ekraninda belirtilen koordinata tasir.
  */
void gotoXY(short x, short y){ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ; 
    COORD position = { x, y } ; 
    SetConsoleCursorPosition( hStdout, position ) ; 
}

/**
  * @PARAM:     visible     imlecin gorunurlugu
  *
  * @OUTPUT:    visible=1 icin imlec gorunur, visible=0 icin imlec gorunmez olur.
  */
void setCursorVisible(bool visible){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursoInfo;
    CursoInfo.dwSize = 1;
    CursoInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &CursoInfo);
}
