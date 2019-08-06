/*
 * @author	:	Salih Marangoz
 * @date	:	29.02.2016
 * @email	:	salih285@gmail.com
 *
 * Aciklama : Secmeli siralamanin gorsellestirilmesi, konsola hizli cizim yapilmasi ve bir kodun nasil yazilmaMAsi gerektigini aciklayan C kodum :)
 * 				Degistirilebilir degerler adi altindaki makrolari degistirerek renk ve boyutlarda oynamalar yapabilirsiniz
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

// DEGISTIRILEBILIR DEGERLER:
#define MAX_VALUE		25		// [1,44]   Ekrana cizilen goruntunun yuksekligini temsil eder
#define MAX_ELEMENTS	50		// [1,78]   Ekrana cizilen goruntunun genisligini temsil eder
#define DELAY_SWAP		99		
#define DELAY_CHECK		33		// 60hz lik bir ekranda goruntunun iyi gorulebilmesi min gecikme: (1000/60) * 2 = 33 olmalidir.
#define COLOR_SWAP		4+8
#define COLOR_COMPARE	2
#define COLOR_DEFAULT	7


// sabitler
#define SCREENX 80
#define SCREENY 45

// global degiskenler
int buffer = 0; // fix 2

// prototipler
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]);
void setCursorVisible(bool visible);
void GotoXY(short x, short y);
void SetColorAndBackground(int ForgC, int BackC);
void dxPrint(int SCREEN[SCREENY][SCREENX][2], int force);
void sort (int SCREEN[SCREENY][SCREENX][2], int carr[], int dizi[], int n);
void dxDrawArray(int SCREEN[SCREENY][SCREENX][2], int *valArray, int *colorArray, int N);

// *******************************************************************************************************************************

int main(){
	int SCREEN[SCREENY][SCREENX][2];
	int arr[MAX_ELEMENTS];	// value array
	int carr[MAX_ELEMENTS];	// color array
	int i;

	srand(time(NULL));
	setCursorVisible(false);
	
	// random arr[] and fill carr with 7 (white color)
	for (i=0; i<MAX_ELEMENTS; i++){
		carr[i] = 7;
		arr[i] = rand() % MAX_VALUE + 1;
	}
	
	printf("SELECTION SORT ANIMASYONU\n  Konsol ekranini buyuterek izleyin :)\n\nSalih Marangoz\n\n");
	system("pause");
	system("cls");
	
	sort(SCREEN, carr, arr, MAX_ELEMENTS);

	GotoXY(0, MAX_VALUE+1);
	system("pause");
		
	return 0;
}


// ******************************************************* RENDER FONKSIYONLARI ******************************************

// ONEMLI NOT: Dikey yonde renkli cizim yapilmasi icin optimize edilmistir. Yatay cizgiler performansa kaybina neden olur.
// BU SAYEDE YAKLASIK OLARAK %20 HIZ KAZANILDI
void dxPrint(int SCREEN[SCREENY][SCREENX][2], int force){

	int i,j,oldValue;
	
	// set first item's color!!!
	SetColorAndBackground(SCREEN[0][0][buffer], SCREEN[0][0][buffer]);
	oldValue=SCREEN[0][0][buffer];
	
	for (j=0; j<SCREENX; j++)
		for (i=0; i<SCREENY; i++)
			if (SCREEN[i][j][0] != SCREEN[i][j][1] || force){ // fix 2 (artik sadece degisen yerleri yazacak)
				GotoXY(j,i+1);	// +1 eklenerek ustte bir satir bosluk birakildi
				
				if (SCREEN[i][j][buffer] != oldValue){	// Eger renk degistirilmesi gerekiyorsa degistir
					SetColorAndBackground(SCREEN[i][j][buffer], SCREEN[i][j][buffer]);
				}
				oldValue = SCREEN[i][j][buffer];
				
				putchar('#');
			}
	GotoXY(0,0);
	
	SetColorAndBackground(7, 0);
	
	buffer = !buffer;
}

// SCREEN matrisini sifirlar. matris'e sekil cizilmeden once kullanilmalidir.
void dxzerofill(int SCREEN[SCREENY][SCREENX][2]){ // ok
	int i,j;
	for (i=0; i<SCREENY; i++)
		for (j=0; j<SCREENX; j++)
			SCREEN[i][j][buffer] = 0;
}

/**
  * @PARAM:		x		ekranda gidilecek noktanin x koordinati
  *				y		ekranda gidilecek noktanin y koordinati
  *
  * @OUTPUT:	Imleci konsol ekraninda belirtilen koordinata tasir.
  */
void GotoXY(short x, short y){ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ; 
    COORD position = { x, y } ; 
    SetConsoleCursorPosition( hStdout, position ) ; 
}

/**
  * @PARAM:		visible		imlecin gorunurlugu
  *
  * @OUTPUT:	visible=1 icin imlec gorunur, visible=0 icin imlec gorunmez olur.
  */
void setCursorVisible(bool visible){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursoInfo;
	CursoInfo.dwSize = 1;
	CursoInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &CursoInfo);
}

/**
  * @PARAM:		ForgC	yazinin renk kodu
  *				BackC	yazinin arkaplan renk kodu
  *
  * @OUTPUT:	Konsola printf ile yazilan yazilarin renk olaylarina etki eder.
  */
void SetColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

// valArray'den sutun yuksekligini, colorArray'den matrise yazilacak sayilari alarak SCREEN matrisine cizim yapar.
// Matrise yazilacak sayilar, renk kodu olarak algilanacagi icin, ornegin matrisin o gozunde 7 yaziyorsa ekrana beyaz bir kutu cizilir. (dxPrint tarafindan)
// 
/* Ornek  valArray[]={4,3,2,2,4} , colorArray[]={2,1,5,5,3}
	  0 0 0 0 0
	  2 0 0 0 3
	  2 1 0 0 3
	  2 1 5 5 3
	  2 1 5 5 3
*/
void dxDrawArray(int SCREEN[SCREENY][SCREENX][2], int *valArray, int *colorArray, int N){
	
	int i,j;
	
	for (i=1; i<=N; i++){	// solda bir birim bos kalmasý icin i=1
		for (j=MAX_VALUE-valArray[i-1]; j<MAX_VALUE; j++){
			SCREEN[j][i][buffer] = colorArray[i-1];
		}
	}
}

// duzenlenmis selection sort
void sort (int SCREEN[SCREENY][SCREENX][2], int carr[], int dizi[], int n){
	
	int baslangic, minSayi, minYer, tmp, i;
	
	for (baslangic=0; baslangic<n-1; baslangic++){
		
		minSayi = dizi[baslangic];
		minYer = baslangic;
		
		for (i=baslangic+1; i<n; i++){
			
			// karsilastirilacak elemanlari yesil olarak ayarla
			carr[i] = COLOR_COMPARE;   // yesil
			carr[minYer] = COLOR_COMPARE;
			
			// goruntuyu yenile
			dxzerofill(SCREEN);
			dxDrawArray(SCREEN, dizi, carr, n);
			dxPrint(SCREEN, 0);
			
			// karsilastirilacak elemanlari yeniden beyaz olarak ayarla
			carr[i] = COLOR_DEFAULT;	// beyaz
			carr[minYer] = COLOR_DEFAULT;
			
			// biraz bekle
			Sleep(DELAY_CHECK);
						
			if ( dizi[i] < minSayi ){
				minSayi = dizi[i];
				minYer = i;
			}
		}	
		
		// yer degistirilecek elemanlari kirmizi olarak ayarla
		carr[baslangic] = COLOR_SWAP;   // kirmizi
		carr[minYer] = COLOR_SWAP;
		
		// goruntuyu yenile
		dxzerofill(SCREEN);
		dxDrawArray(SCREEN, dizi, carr, n);
		dxPrint(SCREEN, 0);
		
		// yer degistirilecek elemanlari yeniden beyaz olarak ayarla
		carr[baslangic] = COLOR_DEFAULT;	// beyaz
		carr[minYer] = COLOR_DEFAULT;
		
		// biraz bekle
		Sleep(DELAY_SWAP);
		
		tmp = dizi[baslangic];
		dizi[baslangic] = minSayi;
		dizi[minYer] = tmp;
	}
	
	// goruntuyu yenile
	dxzerofill(SCREEN);
	dxDrawArray(SCREEN, dizi, carr, n);
	dxPrint(SCREEN, 0);
}

