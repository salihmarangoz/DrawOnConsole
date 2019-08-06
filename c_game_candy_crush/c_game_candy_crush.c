/* NOT:
	-> HALA CIZGILI TASINMA SONRASI PATLAMALARDA TASINAN KUTU DIGERLERI GIBI PARLAMAYABILIYOR. DUZELTEMEDIM
	-> SATIR-SUTUN 20 UZERI SECIMLERINDE HATA VERIYOR. KONTROL EKLENMEDI
	-> YENILDIMI FONK YAZILMADI DAHA
*/

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <limits.h>


// matrisin max boyutu (13 tavsiye edilir)
#define MAX_SATIR 20
#define MAX_SUTUN 20

/* HAZIR RENK KODLARI
   not: renk kodunun parlak versiyonu icin +8 yap */
	#define SIYAH 			0
	#define MAVI			1
	#define YESIL			2	
	#define TURKUAZ			3
	#define KIRMIZI			4
	#define PEMBE			5
	#define SARI			6
	#define BEYAZ			7
/*------------------------*/
	#define ACIK_SIYAH 		0
	#define ACIK_MAVI		9
	#define ACIK_YESIL		10
	#define ACIK_TURKUAZ	11
	#define ACIK_KIRMIZI	12
	#define ACIK_PEMBE		13
	#define ACIK_SARI		14
	#define ACIK_BEYAZ		15
/* end */


/*  Render(...) fonk icin kullanilacak sabitler burada basliyor	*/
	#define KUTU_SATIR 3		// kutunun yuksekligi (satir miktari cinsinden)
	#define KUTU_SUTUN 5		// kutunun genisligi (sutun miktari cinsinden)
	#define YAZI_SATIR 1		// yazinin bulunacagi satir INDEXI!
	#define YAZI_SUTUN 2		// yazinin bulunacagi sutun INDEXI!
	#define DEFAULT_YAZIRENGI BEYAZ
	#define DEFAULT_ARKAPLAN SIYAH
	#define CERCEVE_YAZI_RENGI SIYAH
	#define CERCEVE_RENGI BEYAZ		// cercevenin dolgu renk kodu
	#define NORMAL_DOLGU ' '	// normal sayi cizimde kullanilan dolgu karakteri
	#define CIZGILI_DOLGU '*'	// cizgili sayi ciziminde kullanilan dolgu karakteri
	#define CERCEVE_DOLGU ' '	// cerceve ciziminde kullanilan dolgu karakteri
	
	/* SAYI KODU ICIN ARKAPLAN RENGI */
	int numBackColor[] =/*0*/ { SIYAH,
						/*1*/	MAVI,
						/*2*/	YESIL,	
						/*3*/	TURKUAZ,	
						/*4*/	KIRMIZI,
						/*5*/	PEMBE,
						/*6*/	SARI,
						/*7*/	BEYAZ
							  };
							  
	/* SAYI KODU ICIN YAZI RENGI */
	int numFontColor[] =/*0*/ { ACIK_SIYAH,
						/*1*/	ACIK_MAVI,
						/*2*/	ACIK_YESIL,	
						/*3*/	ACIK_TURKUAZ,	
						/*4*/	ACIK_KIRMIZI,
						/*5*/	ACIK_PEMBE,
						/*6*/	ACIK_SARI,
						/*7*/	ACIK_BEYAZ
							  };
/*  end	*/


// kac farkli sayi turumuz olacak? ( DIKKAT! EN AZ 4 AYARLA. YOKSA TAKILIYOR )
#define SAYI_MIKTARI	6	// 6 degeri icin 0,1,2,3,4,5 olur. Bu deger degistirilirse numBackColor[] ve numFontColor[] un da duzenlenmesi gerekebilir!

// Blink sureleri
#define SECIM_BLINK		70
#define GEZINTI_BLINK	300

int CIZGILI_SANSI =	7;	// yuzde 7 gayet iyi bir olasilik, patlangac modunda yuzde 20 oluyor (in main cfg)


// mtr[][][0] -> ust katman
// mtr[][][1] -> parlaklik katmani (0 ise mat, 1 ise parlak)
// mtr[][][2] -> cizgilik katmani (0 ise duz, 1 ise cizgili)
// mtr[][][3] -> isaretliler katmani (0 ise isaretli degil, 1 ise isaretli) ( isaretliler yokedilecek olanlardir)
int ***matris;

// ekranda yanip sonen kutucuklari pivot olarak adlandiriyoruz
int pivot[4][2]		// pivotlarin koordinatlari pivot[][0] = i, pivot[][1] = j olmak uzere
				= { {0,0} , {-1,-1} , {-1,-1} , {-1,-1} };
int pivotSayisi=1;	// baslangicta 1 ama sonradan 4 e kadar cikabilir

// [0] patlatma sonrasi parlak kutularin gorunme suresi, [1] bir birim kaydiktan sonra bekleme suresi
int gecikme[2] = {200,100};

int enablePatlangac=0;
unsigned long puan;

// ------------------------------------------------- DEGISKEN-SABIT TANIMLAMALARININ SONU -------------------------------------------------------




// KUTUPHANEDEKI FONKSIYONLAR
void SetColorAndBackground(int, int);																	// ok
void GotoXY(short, short);																				// ok
void setCursorVisible(bool);																			// ok

// YAZILAN FONKSIYONLAR
void RenderPivot(int setLight, int ***mtr, int mtr_i, int mtr_j, int mtr_x, int mtr_y, int cerceve); 	// ok
void Render(int ***mtr, int i_start, int j_start, int satir, int sutun, int x, int y, int cerceve); 	// ok
void SayiPatlat(int ***mtr, int satir, int sutun);														// ok
int SayiIsaretle(int ***mtr, int satir, int sutun); 													// ok
int BirBirimAsagiKaydir(int ***mtr, int satir, int sutun);												// ok
int IsInRange(int satir, int sutun, int i, int j);														// ok
int RastgeleSayi(int boslukDahil);																		// ok
int YenildiMi(int***, int, int);																		// in dev

int kontrolPlus1[6][2] = { {-1,-1},{0,-2},{1,-1},{-1,2},{0,3},{1,2} };
int kontrolPlus2[2][2] = { {-1,1},{1,1} };

/*
int YenildiMi(int ***mtr, int satir, int sutun){
	
	int i,j,k;
	
	for (i=0; i<satir; i++){
		for (j=0; j<sutun-1; j=j+2){
			
		}
	}
	
	
	
}
*/


int main(){
	
	int i,j;
	srand(time(NULL));				// rand() fonksiyonunu resetle
	
	// dinamik 3d matris tanimlamasi
	matris = (int***) malloc (MAX_SATIR*sizeof(int**));
	for (i=0; i<MAX_SUTUN; i++){
		matris[i] = (int**) malloc (MAX_SUTUN*sizeof(int*));
		for (j=0; j<MAX_SUTUN; j++){
			matris[i][j] = (int*) calloc (4, sizeof(int));
		}
	}
	
	// Iki sayinin gerekli verilerinin yerlerini degistir
	void Swap(int i1, int j1, int i2, int j2){
		int swap_i;
		for (swap_i = 0; swap_i<3; swap_i++){
			int tempValue;
			tempValue = matris[i1][j1][swap_i];
			matris[i1][j1][swap_i] = matris[i2][j2][swap_i];
			matris[i2][j2][swap_i] = tempValue;	
		}
	}
	
	// GETKEYSTATE FONKSIYONU
	int GetKeyState( int key ){
		if ( GetAsyncKeyState ( key ) & SHRT_MAX )
			return 1;
		return 0;
	}
	
	int matSatir,matSutun,secim;
	
	
	do{
		printf("NUMBER CRASH v0.9 (salih marangoz)\n\n1. 6x6 ve patlangac modu pasif \n2. 6x13 ve patlangac pasif\n3. 15x15 ve patlangac aktif (CrashMania)\n4. Diger\n\nNOT: Tam ekran oynayiniz.\n\nSecim yapiniz: ");
		scanf("%d", &secim);
		if (secim==1){
			matSatir = 6;
			matSutun = 6;
		}
		else if (secim==2){
			matSatir = 6;
			matSutun = 13;		
		}
		else if (secim==3){
			matSatir = 15;
			matSutun = 15;
			enablePatlangac = 1;
		}
		else if (secim==4){
			system("cls");
			printf("Kaca kac oynamak istersiniz? (6x6 ideal, 1920x1080 ekran icin max 15x15)\nSatir sayisi: ");
			scanf("%d", &matSatir);
			printf("Sutun Sayisi: ");
			scanf("%d", &matSutun);	
			printf("Patlangac ozelligi ile cizgili seker sansi yuzde 20ye cikar ve cizgili sekerlerin patlamasi diger sekerleri de tetikler hale gelir. Acmak icin 1, normal oynamak icin 0 girin.\nenablePatlangac: ");
			scanf("%d", &enablePatlangac);
		}
		system("cls");			//3x3 gibi kucuk secimlerde yazilar kaliyordu BUGFIX
	} while (!(0<secim && secim<5));

	// patlangac aktif edilmek istendiyse ozel ayarlari uygula
	if (enablePatlangac){
		CIZGILI_SANSI = 20;
		gecikme[0] = 200;
		gecikme[1] = 100;
	}
		
		
	// imlecin yanip sonmesini durdur
	setCursorVisible(false);		
	
	// RASTGELE BIR MATRIS OLUSTUR (EN AZ 3LU YANYANA SAYI OLMASIN)
	for (i=0; i<matSatir; i++)
		BirBirimAsagiKaydir(matris,matSatir,matSutun);
	Render(matris, 0,0,matSatir,matSutun,0,0,1);
	while (SayiIsaretle(matris, matSatir, matSutun)){
		SayiPatlat(matris, matSatir, matSutun);
		while (BirBirimAsagiKaydir(matris, matSatir, matSutun));
	}
	
	puan=0;
	time_t t;
	int blink=0;
	int temp;
	t = clock();
	
	Render(matris, 0,0,matSatir,matSutun,0,0,1);
	printf("\nPUAN: 0\t\t\t\nYon tuslari ile gezin, Enter ile sec ve yon tusu ile o yone kaydir!");
	pivot[0][0] = 0;
	pivot[0][1] = 0;			
	pivotSayisi = 1;
	
	short esc = 0;
	int dongudenCik;
	int firstRun = GetKeyState ( VK_RETURN );	// ilk calistirmada, scanflerdeki enter basimlari okunmadigi icin burada bir okuma yapalim ki oyuna baslar basmalaz entera basti olayi yasanmasin BUGFIX
	while ( !esc ) {
		
		esc = GetKeyState ( VK_ESCAPE );
		
		// if else durumu cikartildi. icteki iflerle karisiyor ve capraz gitme olayý var!
		
		// YUKARI GIT
		if ( GetKeyState ( VK_UP ) )				
			if (IsInRange(matSatir,matSutun,pivot[0][0]-1,pivot[0][1])){
				RenderPivot(0, matris, matSatir,matSutun, 0,0, 1);	
				pivot[0][0]--;
				RenderPivot(1, matris, matSatir,matSutun, 0,0, 1);
			}
			
		// ASAGI GIT
		if ( GetKeyState ( VK_DOWN ) )
			if (IsInRange(matSatir,matSutun,pivot[0][0]+1,pivot[0][1])){
				RenderPivot(0, matris, matSatir,matSutun, 0,0, 1);	
				pivot[0][0]++;
				RenderPivot(1, matris, matSatir,matSutun, 0,0, 1);
			}
			
		// SOLA GIT
		if ( GetKeyState ( VK_LEFT ) )
			if (IsInRange(matSatir,matSutun,pivot[0][0],pivot[0][1]-1)){
				RenderPivot(0, matris, matSatir,matSutun, 0,0, 1);	
				pivot[0][1]--;
				RenderPivot(1, matris, matSatir,matSutun, 0,0, 1);	
			}
				
		// SAGA GIT
	   	if ( GetKeyState ( VK_RIGHT ) )
			if (IsInRange(matSatir,matSutun,pivot[0][0],pivot[0][1]+1)){
				RenderPivot(0, matris, matSatir,matSutun, 0,0, 1);	
				pivot[0][1]++;
				RenderPivot(1, matris, matSatir,matSutun, 0,0, 1);
			}
			
		// ENTER ILE SEC
	   	if ( GetKeyState ( VK_RETURN ) ){
			dongudenCik=0;
				
			while (!dongudenCik){
				
				// SECIM BLINK
				temp =  ((clock() - t));
				if ( temp > SECIM_BLINK){
					blink = !blink;
					RenderPivot(blink, matris, matSatir,matSutun, 0,0, 1);
					t = clock();
				}
				Sleep(1);
				
				// MUMKUNSE KUTUYU BELIRLENEN TARAFA KAYDIR
				// YUKARI KAYDIR
				if (GetKeyState(VK_UP) && !dongudenCik && IsInRange(matSatir,matSutun,pivot[0][0]-1,pivot[0][1])){
					Swap(pivot[0][0],pivot[0][1],pivot[0][0]-1,pivot[0][1]);
					if (SayiIsaretle(matris, matSatir, matSutun))
						pivot[0][0]--;
					else
						Swap(pivot[0][0],pivot[0][1],pivot[0][0]-1,pivot[0][1]);
					dongudenCik=1;
				}
				// ASAGI KAYDIR
				if (GetKeyState(VK_DOWN) && !dongudenCik && IsInRange(matSatir,matSutun,pivot[0][0]+1,pivot[0][1])){
					Swap(pivot[0][0],pivot[0][1],pivot[0][0]+1,pivot[0][1]);
					if (SayiIsaretle(matris, matSatir, matSutun))
						pivot[0][0]++;
					else
						Swap(pivot[0][0],pivot[0][1],pivot[0][0]+1,pivot[0][1]);
					dongudenCik=1;
				}
				// SOLA KAYDIR
				if (GetKeyState(VK_LEFT) && !dongudenCik && IsInRange(matSatir,matSutun,pivot[0][0],pivot[0][1]-1)){
					Swap(pivot[0][0],pivot[0][1],pivot[0][0],pivot[0][1]-1);
					if (SayiIsaretle(matris, matSatir, matSutun))
						pivot[0][1]--;
					else
						Swap(pivot[0][0],pivot[0][1],pivot[0][0],pivot[0][1]-1);
					dongudenCik=1;
				}
				// SAGA KAYDIR
			   	if (GetKeyState(VK_RIGHT) && !dongudenCik && IsInRange(matSatir,matSutun,pivot[0][0],pivot[0][1]+1)){
					Swap(pivot[0][0],pivot[0][1],pivot[0][0],pivot[0][1]+1);
					if (SayiIsaretle(matris, matSatir, matSutun))
						pivot[0][1]++;
					else
						Swap(pivot[0][0],pivot[0][1],pivot[0][0],pivot[0][1]+1);
					dongudenCik=1;
				}
				// ENTER
			   	if (GetKeyState(VK_RETURN) && !dongudenCik)	//ENTER'a basildiysa hicbisey yapmadan direk cik
					dongudenCik = 1;
				// ESC
				if (GetKeyState ( VK_ESCAPE ) ){			//ESC'ye basildiysa oyunu sonlandir
					dongudenCik = 1;
					esc = 1;
				}
		
			}
			
			// bu bolumde, sayi elenmezse render ardarda 1 defa elenirse 2 defa calisiyor. yani sayi elenme durumunda 1 fazla TODO!!!
			RenderPivot(0, matris, matSatir,matSutun, 0,0, 1);
			Render(matris, 0,0,matSatir,matSutun,0,0,1);
			while (SayiIsaretle(matris, matSatir, matSutun)){
				Render(matris, 0,0,matSatir,matSutun,0,0,1);
				Sleep(gecikme[0]);
				SayiPatlat(matris, matSatir, matSutun);
				Render(matris, 0,0,matSatir,matSutun,0,0,1);
				printf("\nPUAN: %ld\t\t\t",puan);
				while (BirBirimAsagiKaydir(matris, matSatir, matSutun)){
					Sleep(gecikme[1]);
					Render(matris, 0,0,matSatir,matSutun,0,0,1);			
				}
			}	
		}
		
		// YENILDI MI FONKSIYONU BURAYA. ESC = 2 KULLANABILIRSIN!
		
		// GEZINTI BLINK
		temp =  ((clock() - t));
		if ( temp > GEZINTI_BLINK){
			blink = !blink;
			RenderPivot(blink, matris, matSatir,matSutun, 0,0, 1);
			t = clock();
		}
		Sleep(1);	
	}		
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++  RENDER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
  * @PARAM:		***mtr				islem yapilacak olan matris. Belirli bir formati var.
  *				i_start, j_start	matriste islem yapmaya hangi indexten baslanacagi
  *				satir, sutun		baslangic noktasindan itibaren kac satir-sutun cizilecegi
  *				x,y					tablonun ekranda cizilecegi koordinat
  *				cerceve				cerceve olup olmayacagi. Eger cerceve var ise tablo birer birim saga ve asagiya kayar!!!
  *
  * @OUTPUT:	Renkli tablo.
  *
  * @NOTE:		Buyuk tablo yazdirildiktan sonra sadece degisen verileri gostermek icin o degisen verilerin bilgileri girilerek onlari yazabilirsin. ORN: RenderPivot(..) fonksiyonu gibi.
  */
void Render(int ***mtr, int i_start, int j_start, int satir, int sutun, int x, int y, int cerceve){
	
	// degisken tanimlamalari
	int i,j,tmp;
	
	// KutuRender'da kullanilmak uzere tmpS dizisi olustur
	char tmpS[KUTU_SUTUN+1];

	
	/**
	  * @PARAM:		i_kutu, j_kutu		cizilecek kutunun matristeki satir-sutun konumu (indexleme 0dan baslar)
	  *				x_kutu, y_kutu		cizilecek kutunun sol ust kosesinin ekran koordinatlari (indexleme 0dan baslar)
	  *
	  * @OUTPUT:	Belirlenen konuma bir kutu cizilir
	  */
	void KutuRender(int i_kutu, int j_kutu, int x_kutu, int y_kutu){
		
		int i,j,backC,fontC,
			isLighted = mtr[i_kutu][j_kutu][1],		// secilen kutunun aydinlik olup olmadigini tutar
			num = mtr[i_kutu][j_kutu][0];			// secilen kutunun verisini tutar
			
		if (mtr[i_kutu][j_kutu][2])
			for (i=0; i<KUTU_SUTUN; i++)
				tmpS[i] = CIZGILI_DOLGU;
		else
			for (i=0; i<KUTU_SUTUN; i++)
				tmpS[i] = NORMAL_DOLGU;
		tmpS[i] = '\0';
			
			
		// rengi ayarla
		fontC = numFontColor [ num ];
		backC = numBackColor [ num ] + isLighted*8;
		SetColorAndBackground(fontC, backC);
			
		// ust kismi yaz
		if (mtr[i_kutu][j_kutu][2]) 	tmpS[YAZI_SUTUN] = CIZGILI_DOLGU;	else	tmpS[YAZI_SUTUN] = NORMAL_DOLGU;
		for (i=0; i<YAZI_SATIR; i++){
			GotoXY(x_kutu, y_kutu+i);
			printf("%s", tmpS);
		}
		
		// yazi iceren satiri yaz
		tmpS[YAZI_SUTUN] = num + '0';
		GotoXY(x_kutu, y_kutu+i);
		printf("%s", tmpS);
		
		// alt kismi yaz
		if (mtr[i_kutu][j_kutu][2]) 	tmpS[YAZI_SUTUN] = CIZGILI_DOLGU;	else	tmpS[YAZI_SUTUN] = NORMAL_DOLGU;
		for (i=i+1; i<KUTU_SATIR; i++){
			GotoXY(x_kutu, y_kutu+i);
			printf("%s", tmpS);
		}
	}
	
	// cerceve kodlari buradan basliyor
	if (cerceve){
		// cerceve rengini ayarla
		SetColorAndBackground(CERCEVE_YAZI_RENGI, CERCEVE_RENGI);
		
		// ust satiri ciz
		GotoXY(x,y);
		tmp = KUTU_SUTUN*sutun+2;
		for (i=0; i<tmp; i++)
			putchar(CERCEVE_DOLGU);
			
		// yan parcalari ciz
		tmp = KUTU_SATIR*satir+2+y;								// NEDEN Y EKLEDIGIMI BILMIYORUM??? :D
		for (i=y+1; i<tmp; i++){
			// soldaki parca
			GotoXY(x,i);
			putchar(CERCEVE_DOLGU);
			// sagdaki parca
			GotoXY(x+KUTU_SUTUN*sutun+1,i);
			putchar(CERCEVE_DOLGU);
		}
		
		// alt satiri ciz
		GotoXY(x,y+KUTU_SATIR*satir+1);
		tmp = KUTU_SUTUN*sutun+2;
		for (i=0; i<tmp; i++)
			putchar(CERCEVE_DOLGU);
			
		// cerceve eklendiyse eklenecek matrisin koordinatini bir saga ve bir asagi kaydir
		x++;
		y++;
	}

	// her bir kutu icin render islemi yapicaz
	for (i=0; i<satir; i++)
		for (j=0; j<sutun; j++)
			KutuRender(i+i_start, j+j_start, x+j*KUTU_SUTUN, y+i*KUTU_SATIR);
	
	// yazma islemi bittikten sonra renklendirmeyi eski haline getirip satiri sonlandiracagiz
	SetColorAndBackground(DEFAULT_YAZIRENGI, DEFAULT_ARKAPLAN);
	printf("\n");
}

/**
  * @PARAM:		mtr			Sayilarin uzerinde oldugu matris. mtr[][][1] kullanilacak.
  *				mtr_i		matrisin satir sayisi
  *				mtr_j		matrisin sutun sayisi
  *				mtr_x		matrisin ekrandaki x konumu (cerceve varsa dahil)
  *				mtr_y 		matrisin ekrandaki y konumu (cerceve varsa dahil)
  *				cerceve		ekrandaki matriste cerceve varsa 1, yoksa 0 gonderilmeli!
  *
  * @OUTPUT:	pivot[][] ve pivotSayisi degiskenlerini kullanarak ekrandaki pivotlari yeniden cizerek gunceller.
  *				not: ekrandaki diger elemanlarda degisiklik yapilmaz. Sadece pivotlar yeniden cizilir.
  */
void RenderPivot(int setLight, int ***mtr, int mtr_i, int mtr_j, int mtr_x, int mtr_y, int cerceve){
	int a,x,y,i,j;
	for (a=0; a<pivotSayisi; a++){
		i = pivot[a][0];
		j = pivot[a][1];
		if (IsInRange(mtr_i, mtr_j, i, j)){
			mtr[i][j][1] = setLight;
			Render(mtr, i, j, 1, 1, mtr_x+j*KUTU_SUTUN+cerceve, mtr_y+i*KUTU_SATIR+cerceve, 0);		// Tek bir kutu icin render yap!
		}			
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++ ISARETLEME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
  * @PARAM:		mtr			Islem yapilacak matris
  *				satir		Matrisin satir sayisi
  *				sutun		Matrisin sutun sayisi
  *
  * @RETURN:	En az bir isaretleme yapilirsa 1, isaretleme yapilmamissa 0 dondurur.
  *
  * NOTE:		Patlangac eklentisi eklendi!
  */
int SayiIsaretle(int ***mtr, int satir, int sutun){
	
	int i,j,k,count,selected, found=0;
	
	/**
	  * @PARAM:		merkez_i	Yokedilecek 3x3 karenin merkezinin satir indexi
	  *				merkez_j	Yokedilecek 3x3 karenin merkezinin sutun indexi
	  *
	  * @RETURN:	En az bir 9lu kare isaretmesi yapilmissa 1, degilse 0 dondurur.
	  *				Yokedilmesi gereken sayilarin [][][1] (parlaklik) ve [][][3] (yokedilmeli) degerlerini 1'e esitler.
	  */
	int KareIsaretle(int merkez_i, int merkez_j){
		int t_i, t_j, didSomething=0;
		for (t_i=-1; t_i<2; t_i++)
			for (t_j=-1; t_j<2; t_j++)
				if (IsInRange(satir, sutun, merkez_i+t_i, merkez_j+t_j) && !mtr[merkez_i+t_i][merkez_j+t_j][3]){	// patlangac eklemesi yapildi!
					mtr[merkez_i+t_i][merkez_j+t_j][1] = 1;
					mtr[merkez_i+t_i][merkez_j+t_j][3] = 1;
					didSomething=1;
				}
		return didSomething;
	}
	
	/** (patlangac eklentisi)
	  * @PARAM:		-
	  *
	  * @RETURN:	Cizgili sayilarin diger cizgili sayilari tetiklemesi surene kadar isaretleme yapar
	  */
	void PatlangacliIsaretle(){
		int t_i, t_j;
		for (t_i=0; t_i<satir; t_i++){
			for (t_j=0; t_j<sutun; t_j++){
				if (mtr[t_i][t_j][2] && mtr[t_i][t_j][3])
					if (KareIsaretle(t_i,t_j))
						PatlangacliIsaretle();
			}
		}
	}

	
	// yanyana sayilar icin isaretleme yap
	for (i=0; i<satir; i++){	
		selected = 0;
		count = 0;
		for (j=0; j<sutun; j++){
			if (mtr[i][j][0] == selected){
				count++;		
			}
			else
			{
				if (count > 1)
					for (k=0; k<=count; k++){
						if (mtr[i][j-k-1][2])
							KareIsaretle(i,j-k-1);	
						else
							mtr[i][j-k-1][3] = 1;	
						mtr[i][j-k-1][1] = 1;
						found=1;
					}					
				selected = mtr[i][j][0];
				count = 0;
			}	
		}
		if (count > 1)
			for (k=0; k<=count; k++){
				if (mtr[i][j-k-1][2])
					KareIsaretle(i,j-k-1);	
				else
					mtr[i][j-k-1][3] = 1;	
				mtr[i][j-k-1][1] = 1;
				found=1;
			}		
	}
	
	// ustuste sayilar icin isaretleme yap
	for (j=0; j<sutun; j++){	
		selected = 0;
		count = 0;
		for (i=0; i<satir; i++){
			if (mtr[i][j][0] == selected){
				count++;		
			}
			else
			{
				if (count > 1)
					for (k=0; k<=count; k++){
						if (mtr[i-k-1][j][2])
							KareIsaretle(i-k-1,j);		
						else
							mtr[i-k-1][j][3] = 1;
						mtr[i-k-1][j][1] = 1;
						found=1;
					}	
				selected = mtr[i][j][0];
				count = 0;
			}	
		}
		if (count > 1)
			for (k=0; k<=count; k++){
				if (mtr[i-k-1][j][2])
					KareIsaretle(i-k-1,j);	
				else
					mtr[i-k-1][j][3] = 1;		
				mtr[i-k-1][j][1] = 1;
				found=1;
			}		
	}
	
	// patlangac eklentisi aktifse, tetikleme modunu aktif hale getir
	if (enablePatlangac)
		PatlangacliIsaretle();
	
	return found;
}

/**
  * @PARAM:		mtr		Islem yapilacak matris. Matriste sadece [][][3] (yokedilmeli)'e gore karar veriliyor
  *				satir	Matrisin satir sayisi
  *				sutun	Matrisin sutun sayisi
  *
  * @RETURN:	[][][3] olan kutulari sifirlar ve sifirlanan kutu basina +1 puan ekler
  */
void SayiPatlat(int ***mtr, int satir, int sutun){
	int i,j;	
	for (i=0; i<satir; i++)
		for (j=0; j<sutun; j++)
			if (mtr[i][j][3]){
				mtr[i][j][0] = 0;
				mtr[i][j][1] = 0;
				mtr[i][j][2] = 0;
				mtr[i][j][3] = 0;
				puan++;
			}
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++ DIGER ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
  * @PARAM:		mtr				kaydirma isleminin yapilacagi matris
  *				satir,sutun		mtr matrisinin satir sutun sayisi
  *
  * @OUTPUT:	mtr matrisindeki sayilari, 0 sayisini bosluk olarak varsayarak asagiya birer birim kaydirir.
  * 			Kaydirma sonucunda ustte kalan bosluklara da RastgeleSayi(0) fonksiyonu ile sayilarla doldurur.
  *				Kaydirma gerceklestiyle 1, kaydirma olmadiysa 0 degeri dondurur.
  *
  * @NOTE:		Dikkat! Eger yokedilmis bir sayinin matris[][][2] degeri sifirlanmazsa hata meydana gelir!
  */
int BirBirimAsagiKaydir(int ***mtr, int satir, int sutun){	
	int i,j,tmp,kaydirildi=0;
	for (j=0; j<sutun; j++){
		for (i=satir-1; i>0; i--){
			
			// Sayinin alti bos ise usttekiyle yer degistir
			if ( !mtr[i][j][0] && mtr[i-1][j][0] ){
				
				// Sadece tasinmasi gerekli verileri tasi
				tmp = mtr[i-1][j][0];			// sayi degeri verisi
				mtr[i-1][j][0] = mtr[i][j][0];
				mtr[i][j][0] = tmp;
				
				tmp = mtr[i-1][j][2];			// cizgililik verisi
				mtr[i-1][j][2] = mtr[i][j][2];
				mtr[i][j][2] = tmp;
				
				kaydirildi=1;
			}
		}
		
		// eger en ustte bir bosluk kaldiysa oraya bir sayi ekle
		if ( !mtr[0][j][0] ){
			mtr[0][j][0] = RastgeleSayi(0);
			if (rand() % 100 < CIZGILI_SANSI)
				mtr[0][j][2] = 1;
			kaydirildi=1;
		}	
	}
	return kaydirildi;
}


/**
  * @PARAM:		satir		Matrisin satir sayisi
  *				sutun		Matrisin sutun sayisi
  *				i			Matriste gecerli olup olmadigi kontrol edilecek satir indexi
  *				j			Matriste gecerli olup olmadigi kontrol edilecek sutun indexi
  *
  * @RETURN		Girilen i,j koordinatlari matris icerisinde ise 1, degilse 0 dondurur.
  */
int IsInRange(int satir, int sutun, int i, int j){
	if (i < 0 || j < 0)
		return 0;
	if (i >= satir || j >= sutun)
		return 0;
	return 1;
}

/**
  * @PARAM:		boslukDahil		Dahil edilirse 0 sayisini rastgele secilebilir yapar.
  *
  * @RETURN:	[!boslukDahil, SAYI_MIKTARI-1] araliginda rastgele bir sayi dondurur
  */
int RastgeleSayi(int boslukDahil){
	if (boslukDahil)
		return rand() % (SAYI_MIKTARI+1);	// 0,1,2,3,4,5
	return rand() % SAYI_MIKTARI + 1;	// 1,2,3,4,5
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
