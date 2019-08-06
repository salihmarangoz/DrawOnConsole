#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

typedef struct vektor{
    double x,y,z;
}VEKTOR;

/* x = [-40,39] , y = [-42,+45] @z=20 */
#define SCREENX 80
#define SCREENY 45
#define PERDEZ 30.0
#define UZAKLIK_MAX 360
#define UZAKLIK_MIN 45

int buffer = 0;
int sayimatris = 1;

void dxWave(int SCREEN[SCREENY][SCREENX][2], int uzaklik, int aci);
void dxdot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0);
void dxprint(int SCREEN[SCREENY][SCREENX][2], int force);
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]);
void SetColorAndBackground(int ForgC, int BackC);
void GotoXY(short x, short y);
void setCursorVisible(bool visible);


int main(){
    int SCREEN[SCREENY][SCREENX][2];

    setCursorVisible(false);
    
    int uzaklik=UZAKLIK_MAX;
    int uzaklikcarpan=1;
    int a=1;
    time_t t;

    while (1){
        t = clock();

        dxzerofill(SCREEN);
        dxWave(SCREEN, 150, uzaklik);
        dxprint(SCREEN,0);
        buffer = !buffer;

        // animasyon
        if (uzaklik==UZAKLIK_MAX){
            uzaklikcarpan=-1;
        }
        else if (uzaklik==UZAKLIK_MIN)
        {
            uzaklikcarpan=1;
        }
        uzaklik += uzaklikcarpan;

        // bekle
        while (clock()-t < (int)(1000.0/60.0)); // max 60 fps
    }

    return 0;
}

/* ************************************************************************************************************************************** */

void dxWave(int SCREEN[SCREENY][SCREENX][2], int uzaklik, int aci){
    float dr, x, y, z, r, t, xp, yp;
    
    dr = 3.1415 / aci;
    
    float size = 0.3;
    int quality = 9;
    
    for (r=5; r<360; r+=quality){
        for (t=0; t<360; t+=quality){
            
            x = r * cos( t * dr ) * size;
            y = r * sin( r * dr ) * size;
            z = r * cos( t * dr ) * size;
            xp = x + ( cos(15*dr) * z )+r;
            yp = y + ( sin(15*dr) * z )+r;
            
            //xp = 160 + xp;
            //yp = 60-yp;
            
            dxdot(SCREEN, xp-200, yp-200, uzaklik);
            
        }
    }
}

void dxdot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0){
    double x,y,oran;
    oran = PERDEZ / z0;
    x = x0 * oran;
    y = -y0 * oran;
    //z = z0 * oran;
    if ( (int)round(y/2) + 23 < SCREENY && (int)round(y/2) + 23 >= 0 && (int)round(x) + 40 < SCREENX && (int)round(x) + 40 >=0)
        SCREEN[ (int)round(y/2) + 23 ][ (int)round(x) + 40 ][buffer] += 1;   // kesisen noktalari daha dolgun gozukmesi icin
}


void dxprint(int SCREEN[SCREENY][SCREENX][2], int force){
    int i,j;
    const char charset[] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };
    const char richcharset[] = " .,:Il!i><+_-?][}{1)(/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

    GotoXY(0,0);
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            if (SCREEN[i][j][0] != SCREEN[i][j][1] || force){ // artik sadece degisen yerleri yazacak
                GotoXY(j,i);
                if (SCREEN[i][j][buffer] != 0)
                    if (sayimatris)
                        putchar(richcharset[  SCREEN[i][j][buffer]  ]);
                    else
                        putchar('.');
                else
                    putchar(' ');
            }
    GotoXY(0,46);
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
void SetColorAndBackground(int ForgC, int BackC){
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
void GotoXY(short x, short y){ 
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
