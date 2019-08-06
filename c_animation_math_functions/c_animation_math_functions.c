#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

// sabitler
#define SCREENX (80)
#define SCREENY (45)
#define PERDEZ (20.0)
#define SPEED (5)

// global degiskenler
int buffer = 0;
int sayimatris = 0;

// prototipler
void dxprint(int SCREEN[SCREENY][SCREENX][2], int force);
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]);
void dxDrawFunction(int SCREEN[SCREENY][SCREENX][2], double f(double), double start, double stop, double increment);
void dxDrawDot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0);
void setCursorVisible(bool visible);
void GotoXY(short x, short y);

// *******************************************************************************************************************************

double func1(double x){
    return 15*sin(x)+5*sin(3*x)+3*sin(5*x);
}

double func2(double x){
    return 60*sin(x)-30*sin(2*x)+20*sin(3*x)-15*sin(4*x)+12*sin(5*x);
}

double func3(double x){
    return sin(x) * log(x);
}

double func4(double x){
    return sin(x) + log(x);
}


void demo(int SCREEN[SCREENY][SCREENX][2]){
    double ustsinir;
    
    system("cls");
    printf("DEMO FOR DRAWFUNCTION()\n\n");
    system("pause");
    system("cls");
    
    dxzerofill(SCREEN);
    ustsinir = 0;
    while (ustsinir < 25){
        dxDrawFunction(SCREEN, func1, 0, ustsinir, 0.05);
        ustsinir += 0.05;
        dxprint(SCREEN, 0);
        dxzerofill(SCREEN);
        printf("15*sin(x) + 5*sin(3*x) + 3*sin(5*x), x=[0.00 , %.2f]", ustsinir);
        Sleep(SPEED);
    }
    Sleep(4000);
    system("cls");
    
    dxzerofill(SCREEN);
    ustsinir = 0;
    while (ustsinir < 25){
        dxDrawFunction(SCREEN, func2, 0, ustsinir, 0.05);
        ustsinir += 0.05;
        dxprint(SCREEN, 0);
        dxzerofill(SCREEN);
        printf("60*sin(x)-30*sin(2*x)+20*sin(3*x)-15*sin(4*x)+12*sin(5*x), x=[0.00 , %.2f]", ustsinir);
        Sleep(SPEED);
    }
    Sleep(4000);
    system("cls");
    
    dxzerofill(SCREEN);
    ustsinir = 0;
    while (ustsinir < 40){
        dxDrawFunction(SCREEN, func3, 1, ustsinir, 0.1);
        ustsinir += 0.05;
        dxprint(SCREEN, 0);
        dxzerofill(SCREEN);
        printf("sin(x) * log(x), x=[1.00 , %.2f]", ustsinir);
        Sleep(SPEED);
    }
    Sleep(4000);
    system("cls");
    
    dxzerofill(SCREEN);
    ustsinir = 0;
    while (ustsinir < 40){
        dxDrawFunction(SCREEN, func4, 1, ustsinir, 0.1);
        ustsinir += 0.05;
        dxprint(SCREEN, 0);
        dxzerofill(SCREEN);
        printf("sin(x) + log(x), x=[%.2f , %.2f]", 1.0, ustsinir);
        Sleep(SPEED);
    }
    Sleep(4000);
    system("cls");
}

int main(){
    int SCREEN[SCREENY][SCREENX][2];
    
    setCursorVisible(false);
    
    while (1){
        demo(SCREEN);
    }
        
    return 0;
}


// ******************************************************* RENDER FONKSIYONLARI ******************************************


// force 1 ise ekrandaki butun karakterler yenilenir
void dxprint(int SCREEN[SCREENY][SCREENX][2], int force){

    int i,j;
    //const char charset[] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };
    const char richcharset[] = " .,:Il!i><+_-?][}{1)(/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    GotoXY(0,0);
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            if (SCREEN[i][j][0] != SCREEN[i][j][1] || force){ // artik sadece degisen yerleri yazacak
                GotoXY(j,i);
                if (SCREEN[i][j][buffer] != 0)
                    if (sayimatris)                         // sayi matrisi modu
                        putchar('0' + SCREEN[i][j][buffer]);
                    else
                        putchar(richcharset[  SCREEN[i][j][buffer] % 32 ]); // % -> TODO
                else
                    putchar(' ');
            }
    GotoXY(0,0);
    
    buffer = !buffer;
}

// SCREEN matrisini sifirlar
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]){
    int i,j;
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            SCREEN[i][j][buffer] = 0;
}

/* x = [-40,39] , y = [-42,+45] @z=20 */
void dxDrawDot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0){
    double x,y,oran;
    oran = PERDEZ / z0;
    x = x0 * oran;
    y = -y0 * oran;
    //z = z0 * oran;
    if ( (int)round(y/2) + 23 < SCREENY && (int)round(y/2) + 23 >= 0 && (int)round(x) + 40 < SCREENX && (int)round(x) + 40 >=0)
        SCREEN[ (int)round(y/2) + 23 ][ (int)round(x) + 40 ][buffer] += 1;   // kesisen noktalarin daha dolgun gozukmesi icin +=
}

// TODO: SIMDILIK BUTUN SEKIL KONSOLA SIGDIRILMAYA CALISILACAK
void dxDrawFunction(int SCREEN[SCREENY][SCREENX][2], double f(double), double start, double stop, double increment){
    
    // parametreler
    //double start=1.0, stop=100.0, increment=1;
    
    // kullanilan degiskenler (degistirmeyin)
    double x = start, y;
    double xmax=stop, xmin=start, xOran, yOran;
    double ymin=f(x), ymax=f(x);
    
    // sinirlari belirle
    while (x<=stop){
        y = f(x);
        if (y > ymax){
            ymax = y;
        }
        if (y < ymin){
            ymin = y;
        }
        x += increment;
    }
    xOran = 90 / (xmax - xmin);
    yOran = 40 / (ymax - ymin)*2;
    
    // cizim yap
    x = start;
    while (x<=stop){
        dxDrawDot(SCREEN,  (x - xmin)*xOran - 45, (f(x) - ymin)*yOran - 30, 25);
        x += increment;
    }
    
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

