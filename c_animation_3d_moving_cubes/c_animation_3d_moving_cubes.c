/* 04.06.2015 - Salih Marangoz */

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

#define SCREENX 80
#define SCREENY 45
#define PERDEZ 20.0
/* x = [-40,39] , y = [-42,+45] @z=20 */

int buffer = 0;
int sayimatris = 0;


void dxkup(int SCREEN[SCREENY][SCREENX][2], VEKTOR a, VEKTOR b, VEKTOR c, VEKTOR k );
void dxprint(int SCREEN[SCREENY][SCREENX][2], int force);
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]);
void dxdot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0);
void dxline(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0, double x1, double y1, double z1);
void SetColorAndBackground(int ForgC, int BackC);
void GotoXY(short x, short y);
void setCursorVisible(bool visible);


int main(){
    int SCREEN[SCREENY][SCREENX][2];

    setCursorVisible(false);

    // kup tanimlamasi:
    VEKTOR a,b,c,k;
    a.x = 40.0; a.y = 0; a.z = 0;
    b.x = 0; b.y = 40.0; b.z = 0;
    c.x = 0; c.y = 0; c.z = 20.0;
    k.x = -80; k.y = 20; k.z = 70; // pozisyon

    VEKTOR e,f,g,h;
    e.x = 40.0; e.y = 0; e.z = 0;
    f.x = 0; f.y = 40.0; f.z = 0;
    g.x = 0; g.y = 0; g.z = 20.0;
    h.x = 10; h.y = -40; h.z = 30;

    VEKTOR o,i,j,l;
    o.x = 40.0; o.y = 0; o.z = 0;
    i.x = 0; i.y = 40.0; i.z = 0;
    j.x = 0; j.y = 0; j.z = 20.0;
    l.x = 30; l.y = 10; l.z = 100;

    time_t t = clock();
    time_t t2 = clock();
    time_t t3;

    int hareket_yonu=-1;
    double animasyon=0.0;
    int fpssayac=1;

    while(1){
        t3 = clock();

        // matrisi sifirla
        dxzerofill(SCREEN);

        // kupu ciz
        dxkup(SCREEN, a,b,c,k);
        dxkup(SCREEN, e,f,g,h);
        dxkup(SCREEN, o,i,j,l);

        // guncel goruntuyu yenile
        dxprint(SCREEN,0);
        buffer = !buffer;

        // fps yazdir
        if (fpssayac==100){
            printf("FPS: %.0f", (100*1000.0) / (clock() - t) );
            t = clock();
            fpssayac=0;
        }
        fpssayac++;
        
        // bekle
        while (clock()-t3 < (int)(1000.0/60.0)); // max 60 fps
        t2 = clock();

        // cisimleri hareket ettir
        h.x += fabs(0.025*(t3 - t2))*hareket_yonu;
        k.y += fabs(0.025*(t3 - t2))*hareket_yonu;
        l.z += fabs(0.025*(t3 - t2))*hareket_yonu;

        animasyon+=fabs(t3-t2);
        if (animasyon>3500.0) {hareket_yonu=-hareket_yonu; animasyon=0.0;}
        //GotoXY(20,40); printf("%d", animasyon);
    }


    return 0;
}

/* ************************************************************************************************************************************** */

void dxkup(int SCREEN[SCREENY][SCREENX][2], VEKTOR a, VEKTOR b, VEKTOR c, VEKTOR k ){
    VEKTOR n[8];

    // noktalari k'ya tasi
    int i;
    for (i=0; i<8; i++){
        n[i].x = k.x;
        n[i].y = k.y;
        n[i].z = k.z;
    }

    // on kisim
    // n[0] OK!
    n[1].x += a.x; n[1].y += a.y; n[1].z += a.z;
    n[2].x += a.x+b.x; n[2].y += a.y+b.y; n[2].z += a.z+b.z;
    n[3].x += b.x; n[3].y += b.y; n[3].z += b.z;

    dxline(SCREEN, n[0].x, n[0].y, n[0].z, n[1].x, n[1].y, n[1].z);
    dxline(SCREEN, n[1].x, n[1].y, n[1].z, n[2].x, n[2].y, n[2].z);
    dxline(SCREEN, n[2].x, n[2].y, n[2].z, n[3].x, n[3].y, n[3].z);
    dxline(SCREEN, n[3].x, n[3].y, n[3].z, n[0].x, n[0].y, n[0].z);

    // arka kisim
    // noktalari k'dan c+k'ya tasi
    for (i=4; i<8; i++){
        n[i].x += c.x;
        n[i].y += c.y;
        n[i].z += c.z;
    }
    // n[4] OK!
    n[5].x += a.x; n[5].y += a.y; n[5].z += a.z;
    n[6].x += a.x+b.x; n[6].y += a.y+b.y; n[6].z += a.z+b.z;
    n[7].x += b.x; n[7].y += b.y; n[7].z += b.z;
    
    dxline(SCREEN, n[4].x, n[4].y, n[4].z, n[5].x, n[5].y, n[5].z);
    dxline(SCREEN, n[5].x, n[5].y, n[5].z, n[6].x, n[6].y, n[6].z);
    dxline(SCREEN, n[6].x, n[6].y, n[6].z, n[7].x, n[7].y, n[7].z);
    dxline(SCREEN, n[7].x, n[7].y, n[7].z, n[4].x, n[4].y, n[4].z);

    // ara kisim
    dxline(SCREEN, n[0].x, n[0].y, n[0].z, n[4].x, n[4].y, n[4].z);
    dxline(SCREEN, n[1].x, n[1].y, n[1].z, n[5].x, n[5].y, n[5].z);
    dxline(SCREEN, n[2].x, n[2].y, n[2].z, n[6].x, n[6].y, n[6].z);
    dxline(SCREEN, n[3].x, n[3].y, n[3].z, n[7].x, n[7].y, n[7].z);
    
}


void dxprint(int SCREEN[SCREENY][SCREENX][2], int force){
    int i,j;
    const char richcharset[] = " .,:Il!i><+_-?][}{1)(/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    GotoXY(0,0);
    for (i=0; i<SCREENY; i++)
        for (j=0; j<SCREENX; j++)
            if (SCREEN[i][j][0] != SCREEN[i][j][1] || force){ // sadece degisen yerleri yaz
                GotoXY(j,i);
                if (SCREEN[i][j][buffer] != 0)
                    if (sayimatris)
                        putchar(SCREEN[i][j][buffer] + '0');
                    else
                        putchar(richcharset[ SCREEN[i][j][buffer] ]);
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

void dxdot(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0){
    double x,y,oran;
    oran = PERDEZ / z0;
    x = x0 * oran;
    y = -y0 * oran;
    //z = z0 * oran;
    SCREEN[ (int)round(y/2) + 23 ][ (int)round(x) + 40 ][buffer] += 1;   // kesisen noktalari daha dolgun gozukmesi icin
}

void dxline(int SCREEN[SCREENY][SCREENX][2], double x0, double y0, double z0, double x1, double y1, double z1){
    double n0x,n0y,oran0;
    double n1x,n1y,oran1;
    int i,N;
    int noktax, noktay;
    
    if (z0==0) z0=0.05;
        
    oran0 = PERDEZ / z0;
    n0x = x0 * oran0;
    n0y = -y0 * oran0;
    //n0z = z0 * oran0;
        n0y = (n0y/2) + 23.0; // 0. nokta
        n0x += 40;

    if (z0==0) z0=0.05;

    oran1 = PERDEZ / z1;
    n1x = x1 * oran1;
    n1y = -y1 * oran1;
    //n1z = z1 * oran1;
        n1y = (n1y/2) + 23.0; // 1. nokta
        n1x += 40;

    if (fabs(n0x-n1x)>=fabs(n0y-n1y)){
        // dogru yatay veya 45derece ise
        // yatay ise yatayda kac tane nokta kullanacagim bellidir.
        N = (int)round(n1x - n0x);
    }
    else
    {
        // dogru dikey ise
        N = (int)round(n1y - n0y);
    }
    for (i=0; i<abs(N); i++){   
        noktax = (int)round(i*(n1x - n0x)/abs(N) + n0x);
        noktay = (int)round(i*(n1y - n0y)/abs(N) + n0y);
        if (noktax>=0 && noktax<80 && noktay>=0 && noktay<45 && z0>0 && z1>0)
            SCREEN[ noktay ][ noktax ][buffer] += 1;
    }
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
