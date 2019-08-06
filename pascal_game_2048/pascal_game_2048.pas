Program Oyun2048;
uses Crt;
{**********************************************************************
 * Yazar         : Salih Marangoz / salih285@gmail.com

 * Tarih         : 02.12.2014 (v0.01) - 11.01.2015 (v1.02 + düzenleme)

 * Versiyon      : 1.02

 * Bilgilendirme : Oyun normalde 4x4 matriste oynanir.

                   Baslangicta rastgele konumda 2 sayi vardir.

                   Rastgele gelen sayilar %90->2 ve %10->4 olmali.

                   Toplamalar duran ve hareket eden arasinda gerceklesir.
                      (Havada carpisma olmaz!)

                   Tek hamlede bir kutuda en fazla 1 defa toplama yapilabilir.
                      (Orn: 2 2 2 2 (sagakaydir) -> 0 0 4 4)

 * Kod hakkinda  : Kodu yazarken cogu seyin degistirilebilmesini istedim:
                      1. Matristeki yazi ve arkaplan renkleri
                      2. Matrisin buyuklugu (satir ve sutun)
                      3. Matris iskeletinin buyuklugu

                   Ancak bazi seyler oldu ki onlari degisken yapmak anlamsizdi:
                      1. Kutularin boyutunun ayarlanabilmesi
                         (bkz: yazi boyutunun degistirilememesi)
                      2. Sayilarin sadece 2'nin kati olmasi
                      3. Baslangicta 2 adet sayiyla baslamak

                   Ayrica kaydirma kodlarini, asamalarini gormek istedigim icin daha kompleks bir hale getirdim.
                      Bu yuzden kaydirma fonksiyonlarini birlestirerek tek parca hale getirdigimde cok daha anlasilmaz ve karisik oldugunu gordum ve vazgectim.

 * Edndgm Tcrube : 1.Clrscr(); yerine GotoXY(x,y); kullanildiginda gecisler daha akici oluyor
                      ( yaz->sil->yaz... yerine uzerineyaz->uzerineyaz... yontemini kullandim)

                   2.Cursoroff; yaparak goruntu yazilmasi sirasinda cikan rahatsiz edici minik siyah kutulardan kurtuluyoruz.
                      (bkz:imlecin yanip sonmesi)

 * Based on 1024 by Veewo Studio and conceptually similar to Threes by Asher Vollmer.
**********************************************************************}

const // GLOBAL SABITLER

   // SABIT RENK KODLARI:
   siyah         = 0; mavi          = 1; yesil         = 2; turkuaz       = 3;
   kirmizi       = 4; pembe         = 5; sari          = 6; beyaz         = 7;


   // DEGISTIRILEBILIR DEGERLER:
   iskelet_renk  = beyaz;
   max_satir     = 13;   // MUMKUNSE   MAX 13
   max_sutun     = 13;   // KESINLIKLE MAX 13
   undo_limiti   = 3;
   sans_2        = 90;   // 2 gelme sansi=90 , 4 gelme sansi=10 , Baslangictaki sayilar icin de gecerlidir.
   u_azalma      = 30;   // undo yapildiginda puanin %30u kesilecek.

var // GLOBAL DEGISKENLER

   // harita(i,j,0)='sayilarin oldugu matris' , harita(i,j,1)='kutuda toplama yapilip yapilmadigini tutan matris'
   harita           : array[1..max_satir,1..max_sutun,0..1] of integer;

   // undo yapilmasi icin degerlerin tutuldugu matris
   u_harita         : array[1..max_satir,1..max_sutun,0..undo_limiti-1] of integer;

   // Gecis efekti verilirken verilen gecikme. Dikey ve yatay yonde karakter sayisi farkli oldugu icin degiskenleri ayri tuttum.
   yatay_gecikme    : integer;
   dikey_gecikme    : integer;

   // u_sayac: undo matrisindeki imlecin konumu , u_hakki: o anda yapabilecegimiz max undo miktari
   u_sayac, u_hakki : integer;

   // undo matris test kodu degiskenleri:
   d1,d2,d3:integer;

   key              : char;
   donguye_gir      : boolean;

   puan_durumu      : real;
   satir,sutun      : integer;
   a,secim          : integer;




//********************************************* ARAYUZ FONKSIYONLARI ***************************************************************

function renk(x,y,z:integer):integer;
// Ekrandaki oordinatlari girilen kutunun renk kodunu verir. Yazi rengi icin z=0, arkaplan rengi icin z=1
// Ornek: @input 15,9,0  @output -> harita[15,9,0]teki kutudaki yazinin renk kodu.
begin
   case (harita[((x-1) div 4)+1,((y-1) div 6)+1,0] + z) of
      // Arkaplan rengi icin +1
      //      YAZI RENGI               ARKAPLAN RENGI
      0     : renk:=siyah;     1     : renk:=siyah;
      2     : renk:=beyaz;     3     : renk:=pembe;
      4     : renk:=beyaz;     5     : renk:=mavi;
      8     : renk:=kirmizi;   9     : renk:=sari;
      16    : renk:=beyaz;     17    : renk:=kirmizi;
      32    : renk:=mavi;      33    : renk:=turkuaz;
      64    : renk:=beyaz;     65    : renk:=yesil;
      128   : renk:=turkuaz;   129   : renk:=siyah;
      256   : renk:=turkuaz;   257   : renk:=beyaz;
      512   : renk:=beyaz;     523   : renk:=siyah;
      1024  : renk:=kirmizi;   1025  : renk:=mavi;
      2048  : renk:=siyah;     2049  : renk:=sari;
   end;
end;


Function sayi_string(x,y:integer):string;
// Girilen sayiyi duzenleyerek 5 (bes) birimlik bir string ciktisi verir.
// Orn: @input: 256  @output: ' 256 '
// Duzeltilebilir: Girilen deger 5 (bes) haneyi asiyorsa cikti olarak '254..' a benzer bir cikti vermeli. (bkz: kod hakkinda bolumu - 1.madde)
var
   tmp_s:string;
   tmp_i:integer;
begin

   // Ekrandaki koordinati girilen kutuda yazmasi gereken sayiyi tmp_s degiskenine STRING cinsinden aktar.
   str((harita[((x-1) div 4)+1,((y-1) div 6)+1,0]),tmp_s);

   // Ekrandaki koordinati girilen kutuda yazmasi gereken sayiyi tmp_i degiskenine INTEGER cinsinden aktar.
   tmp_i:=(harita[((x-1) div 4)+1,((y-1) div 6)+1,0]);

   // Sayinin buyuklugune gore 5 hanelik string ciktisi ver.
   if tmp_i<10 then
      sayi_string:=concat('  ',tmp_s,'  ')
   else if tmp_i<100 then
      sayi_string:=concat(' ',tmp_s,'  ')
   else if tmp_i<1000 then
      sayi_string:=concat(' ',tmp_s,' ')
   else if tmp_i<10000 then
      sayi_string:=concat(tmp_s,' ')
   else if tmp_i<100000 then
      sayi_string:=tmp_s;

end;

Procedure ekran_yazdir();
//Matristeki degerleri renklendirip imlecin oldugu yerden baslayarak konsola yazdirir.
var
   x,y : integer;
begin

   // ekrandaki kutularin genisligi=5 karakter , yuksekligi=3 karakter
   for x:=1 to 4*satir+1 do begin
      for y:=1 to 6*sutun+1 do begin

         if (y mod 6=1) or (x mod 4=1) then begin
            // Yazilacak karakter iskelet uzerinde ise:
            textbackground(iskelet_renk);
            write(' ');
         end
         else begin
            // Yazilacak karakter kutu icinde ise:
            textbackground(renk(x,y,1));
            textcolor(renk(x,y,0));
            if (y mod 6=2) and (x mod 4=3) then
               // Yazilacak bolum sayi yazilan bolum ise 5 haneyi bir kerede yazdir.
               write(sayi_string(x,y))
            else if (x mod 4<>3) then
               // Yazilacak bolumde sayi yok ise sadece renkle doldur.
               write(' ');
         end;

         // Eger satir sonuna geldiysek alt satira gec.
         if y=6*sutun+1 then
            writeln();

      end;
   end;

   // Yazdirma islemi bittikten sonra renkleri eski haline getir. Puanlari yazdirirken sikinti yasamayalim.
   textcolor(7);
   textbackground(0);

   // Puan ve undo durumunu yazdiralim.
   write('PUAN: ',puan_durumu:0:0);
   writeln('                                            '); // bug fix 1.01 - Puan undo ile azaldiginda 1000 -> 700 olmasi gerekirken 7000 yaziyordu (bkz: eski goruntunun silinmemesi)
   writeln;
   writeln('UNDO HAKKI: ', u_hakki);

{
//UNDO MATRISLERINI YAZDIRAN TEST KODU
   textcolor(0);
   textbackground(7);
   for d3:=0 to undo_limiti-1 do begin
      for d1:=1 to satir do
         for d2:=1 to sutun do
             write(u_harita[d1,d2,d3]);
      writeln;
   end;
   writeln('u_hakki: ',u_hakki);
   writeln('u_sayac: ',u_sayac);
}
end;


//********************************************* OYUN FONKSIYONLARI *****************************************************************
procedure yeni_sayi(yeni_oyun:integer);
// yeni_sayi(0): Sans faktorune bagli kalmak sartiyla matristeki bos bir konuma sayi degeri atar.
// yeni_sayi(1): Matrisi sifirlayip 2 adet rastgele sayi atar.
var
   i,j,k,sifir_var_mi : integer;
begin

   if yeni_oyun=1 then begin            // yeni oyun isteniyorsa matrisi sifirla
      u_sayac:=0;
      u_hakki:=0;
      puan_durumu:=0;
      for i:=1 to satir do
         for j:=1 to sutun do
            harita[i,j,0]:=0;
   end;

   for k:=1 to (yeni_oyun + 1) do begin // yeni oyuna basladiysak fazladan 1 sayi daha ekle.

      randomize;                        // random table'i sifirlayalim.

      // BUG FIX 1.02: Kaydirma fonk yanlis cikti vermedigi icin bu durumu kontrol etmeye gerek yok.
      {
      sifir_var_mi:=0;                  // sifir olup olmadigini kontrol edelim.
      for i:=1 to satir do
         for j:=1 to sutun do
            if harita[i,j,0]=0 then sifir_var_mi:=1;
      }
      sifir_var_mi:=1;                  // Yukarýdaki BUG FIX 1.02 geri alinirsa bunu sil!!!

      if sifir_var_mi=1 then begin

         // rastgele sectigimiz matris degeri sifir olana kadar tekrarlayalim.
         repeat
            i:=random(satir)+1;
            j:=random(sutun)+1;
         until harita[i,j,0]=0;

         // Random sayiya gore 2 veya 4 verelim.
         if random(100)<sans_2 then
            harita[i,j,0]:=2
         else
            harita[i,j,0]:=4;

      end
      else begin
         //eger buraya geliyorsa kaydirma fonk hatali demektir.
      end;
   end;
end;


function yenildi_mi():integer;
// Yenilme durumu gerceklestiyse 1 ciktisi ver.
var
   i,j,durum : integer;
begin

   durum:=1; // Yenildigini farzedelim. Yenilmedigini kanitlamaya calisalim.

   for i:=1 to satir do begin            // Eger bos bir kutumuz varsa kesinlikle yenilmemistir.
      for j:=1 to sutun do begin
         if harita[i,j,0]=0 then begin
            durum:=0;
            break;
         end;
      end;
      if durum=0 then
         break;
   end;

   // algoritma_1: sayinin altindakini ve sagindakini kontrol et.
   for i:=1 to satir-1 do begin
      for j:=1 to sutun-1 do begin
         if (harita[i,j,0]=harita[i+1,j,0]) OR (harita[i,j,0]=harita[i,j+1,0]) then begin
            durum:=0;
            break;
         end;
      end;
      if durum=0 then
         break;
   end;

   // algoritma_2: algoritma1 den kacan sayilari kontrol et. (en sagdaki sutun ve en alttaki satirda bulunan sayilari kontrol eder)
   if durum<>0 then
      for i:=1 to satir-1 do
         if (harita[satir,i,0]=harita[satir,i+1,0]) then
            durum:=0;
   if durum<>0 then
      for i:=1 to sutun-1 do
         if (harita[i,sutun,0]=harita[i+1,sutun,0]) then
            durum:=0;

   yenildi_mi:=durum;

end;

function geri_al(komut:integer):integer;
// geri_al(0); -> Matrisi, daha sonra geri almak icin hafizaya kaydeder.
// geri_al(1); -> Matrisi bir onceki konumuna geri cevirerek (ekrana yazdirmaz) puani azaltir. Geri alindiysa 0 , alinaMAdiysa -1 verir.
var
   i,j : integer;
begin

   if komut=0 then begin                // Hafizaya kaydet.
      u_sayac:=u_sayac+1;
      if u_sayac=undo_limiti then u_sayac:=0;
      for i:=1 to satir do
         for j:=1 to sutun do
            u_harita[i,j,u_sayac]:=harita[i,j,0];
      if u_hakki <> undo_limiti then u_hakki:=u_hakki+1;
   end;

   if komut=1 then begin                // Geri al.
      if u_hakki<>0 then begin
         for i:=1 to satir do
            for j:=1 to sutun do
               harita[i,j,0]:=u_harita[i,j,u_sayac];
         u_sayac:=u_sayac-1;
         if u_sayac=-1 then u_sayac:=undo_limiti-1;
         u_hakki:=u_hakki-1;
         geri_al:=0;                    //dogru calisti!
      end
      else
         geri_al:=-1;                   //bi sikinti var ise hata ver!
   end;

end;

procedure puan(komut,eklenecek:integer);
// input 0,x: x puanini ekler.
// input 2,x: undo sonrasi puani keser.
// (SÝLÝNDÝ!!!)input 1,x: puan bilgisini real olarak cikti verir.
begin

   if komut=0 then                      // Puan ekle.
      puan_durumu:=puan_durumu+eklenecek;

   if komut=2 then                      // Undo yapilmis. Puani azalt.
      puan_durumu:=puan_durumu*(100-u_azalma)/100;

end;


//********************************************* KAYDIRMA FONKSIYONLARI *************************************************************

function saga_kaydir():integer;

// Kaydirma islemi yapar. Bu sirada gecis efektini gosterir ve otomatik olarak ekrana yazdirir.
// Eger matriste bir degisim olursa 0, olmazsa 1 ciktisi verir.

{ Algoritma aciklamasi:

    Gecis efektini gosterme amacýmýz oldugu icin sayilari birer birim oynatarak ekrana gecis goruntusunu vermemiz gerekiyordu.
    Ancak soyle bir durum vardý: Eger bir kutuda toplama islemi yapildiysa o kutuda yeniden bir toplama islemi yapilmamasi gerekiyor (Orn matris[i,j,0]: 2 2 2 2 -> 0 0 4 4 )
    Bu yuzden matrisimizi 3 boyutlu hale cevirdim ve toplama islemi yapilmadan once arkaplan matrisinin degerini kontrol ettirdim.   (Orn matris[i,j,1]: 0 0 0 0 -> 0 0 1 1 )

    #saga_kaydir fonk icin orneklendireyim:
       1-Arkaplan matrisi sýfýrlanir.
       2-Sayimiz sifir degil ise isleme girilir.
       3-Eger matris kaydedilmediyse geri_al(0) yapilarak matris hafizaya alinir.
       4-Sayýlar 'en sagdan baslayarak'*** birer birim kaydýrýlýr VEYA yanýndaki sayýnýn üzerine eklenir. Ekleme yapildiysa eklenen sayiya gore puan eklenir VE eklemenin yapildigi kutu icin matris[i,j,1]:=1 yapilir.
       5-Bir birim kaydirilan matris ekrana yazdirilir ve belirlenen DELAY degeri kadar beklenir.
          Degisti=1 ise 2. asamaya donulur.
          Degisti=0 ise fonk cikti vererek sonlandirilir.

    ***: Veri kaybýna ugramamak icin kaydirma yonunden diger yone dogru sayi tasima islemi yapilir.
}

var
   i,j,degisti,cikti,kaydedildi : integer;

begin

   cikti:=1;                                // Kaydirma yapilmadigin farzedelim.
   kaydedildi:=0;                           // Undo icin hafizaya kaydedilmedigi farzedelim.

   // Once  arkaplani (toplama yapilabilirligi) sifirla.
   for i:=1 to satir do
      for j:=1 to sutun do
         harita[i,j,1]:=0;

   // Degisim olmayana kadar birer birim kaydirma islemi yap.
   repeat

      degisti:=0;

      for i:=1 to satir do begin
         for j:=sutun-1 downto 1 do begin


            if harita[i,j,0]<>0 then begin  // Sayimiz sifir degil ise isleme gir.
               if harita[i,j+1,0]=0 then begin   // Sagdaki sayi sifir ise sayiyi saga kaydir.
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  harita[i,j+1,0]:=harita[i,j,0];
                  harita[i,j,0]:=0;
                  degisti:=1;
               end
               else if (harita[i,j+1,0]=harita[i,j,0]) AND ((harita[i,j+1,1]=0) AND (harita[i,j,1]=0)) then begin  // Sagdaki sayi, sayimiza esitse ve daha once islem yapilmamissa 2 ile carp.
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  puan(0,harita[i,j,0]);  //puani ekle.
                  harita[i,j+1,0]:=harita[i,j+1,0]*2;
                  harita[i,j,0]:=0;
                  harita[i,j+1,1]:=1;
                  degisti:=1;
               end;
            end;


         end;
      end;

      // Degisim olmussa ekrana goruntuyu yazdir ve bir sure bekle.
      if degisti=1 then begin
         cikti:=0;
         gotoxy(1,1);
         ekran_yazdir();
         delay(yatay_gecikme);
      end;

   until degisti=0;

   saga_kaydir:=cikti;
end;


function sola_kaydir():integer;
//Aciklamalar saga_kaydir() fonk icin yapilmistir. Kullanimi aynidir.
var
   i,j,degisti,cikti,kaydedildi : integer;
begin
   cikti:=1;
   kaydedildi:=0;
   for i:=1 to satir do
      for j:=1 to sutun do
         harita[i,j,1]:=0;
   repeat
      degisti:=0;
      for i:=1 to satir do begin
         for j:=2 to sutun do begin
            if harita[i,j,0]<>0 then begin
               if harita[i,j-1,0]=0 then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  harita[i,j-1,0]:=harita[i,j,0];
                  harita[i,j,0]:=0;
                  degisti:=1;
               end
               else if (harita[i,j-1,0]=harita[i,j,0]) AND ((harita[i,j-1,1]=0) AND (harita[i,j,1]=0)) then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  puan(0,harita[i,j,0]);  //puani ekle.
                  harita[i,j-1,0]:=harita[i,j-1,0]*2;
                  harita[i,j,0]:=0;
                  harita[i,j-1,1]:=1;
                  degisti:=1;
               end;
            end;
         end;
      end;
      if degisti=1 then begin
         cikti:=0;
         gotoxy(1,1);
         ekran_yazdir();
         delay(yatay_gecikme);
      end;
   until degisti=0;
   sola_kaydir:=cikti;
end;

function yukari_kaydir():integer;
//Aciklamalar saga_kaydir() fonk icin yapilmistir. Kullanimi aynidir.
var
   i,j,degisti,cikti,kaydedildi : integer;
begin
   cikti:=1;
   kaydedildi:=0;
   for i:=1 to satir do
      for j:=1 to sutun do
         harita[i,j,1]:=0;
   repeat
      degisti:=0;
      for j:=1 to sutun do begin
         for i:=2 to satir do begin
            if harita[i,j,0]<>0 then begin
               if harita[i-1,j,0]=0 then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  harita[i-1,j,0]:=harita[i,j,0];
                  harita[i,j,0]:=0;
                  degisti:=1;
               end
               else if (harita[i-1,j,0]=harita[i,j,0]) AND ((harita[i-1,j,1]=0) AND (harita[i,j,1]=0)) then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  puan(0,harita[i,j,0]);  //puani ekle.
                  harita[i-1,j,0]:=harita[i-1,j,0]*2;
                  harita[i,j,0]:=0;
                  harita[i-1,j,1]:=1;
                  degisti:=1;
               end;
            end;
         end;
      end;
      if degisti=1 then begin
         cikti:=0;
         gotoxy(1,1);
         ekran_yazdir();
         delay(dikey_gecikme);
      end;
   until degisti=0;
   yukari_kaydir:=cikti;
end;

function asagi_kaydir():integer;
//Aciklamalar saga_kaydir() fonk icin yapilmistir. Kullanimi aynidir.
var
   i,j,degisti,cikti,kaydedildi : integer;
begin
   cikti:=1;
   kaydedildi:=0;
   for i:=1 to satir do
      for j:=1 to sutun do
         harita[i,j,1]:=0;
   repeat
      degisti:=0;
      for j:=1 to sutun do begin
         for i:=satir-1 downto 1 do begin
            if harita[i,j,0]<>0 then begin
               if harita[i+1,j,0]=0 then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  harita[i+1,j,0]:=harita[i,j,0];
                  harita[i,j,0]:=0;
                  degisti:=1;
               end
               else if (harita[i+1,j,0]=harita[i,j,0]) AND ((harita[i+1,j,1]=0) AND (harita[i,j,1]=0)) then begin
                  if kaydedildi=0 then begin
                     geri_al(0); // undo icin kaydet.
                     kaydedildi:=1;
                  end;
                  puan(0,harita[i,j,0]);  //puani ekle.
                  harita[i+1,j,0]:=harita[i+1,j,0]*2;
                  harita[i,j,0]:=0;
                  harita[i+1,j,1]:=1;
                  degisti:=1;
               end;
            end;
         end;
      end;
      if degisti=1 then begin
         cikti:=0;
         gotoxy(1,1);
         ekran_yazdir();
         delay(dikey_gecikme);
      end;
   until degisti=0;
   asagi_kaydir:=cikti;
end;


//********************************************* MAIN *******************************************************************************
begin

   // DEGISKENLERIN ILK DEGERLERI:
   u_sayac:=0;
   u_hakki:=0;
   donguye_gir:=true;         // tus okuma dongusunden hemen cikma.
   cursoroff;                 // imlecin yanip sonmesini engelle.

   // ANA MENU EKRANI:
   writeln('Oyunu seciniz:');
   writeln('1. 4x4 (Klasik)');
   for a:= 2 to max_satir-3 do
      writeln(a,'. ',a+3,'x',a+3);
   repeat
      readln(secim);
   until (secim <=max_satir) and (secim >= 1);
   satir:=secim+3;            // Secime gore matrisin sinirlarini belirle.
   sutun:=secim+3;

   // BOYUTA GORE GECIKMENIN AYARLANMASI:
   if satir<=6 then begin
      yatay_gecikme:=20;
      dikey_gecikme:=20;
   end;

   // OYUN EKRANI:
   yeni_sayi(1);
   gotoxy(1,1);
   ekran_yazdir;
   writeln;
   writeln('N: yeni oyun , U: geri al');
   writeln;
   writeln('Salih Marangoz / salih285@gmail.com');

   // TUS KOMUTLARI:
   while donguye_gir do begin

      // Tusa basildiysa kontrol et.
      if keypressed then begin

         // Girilen tusu oku.
         key:=readkey;
         if keypressed then key:=readkey;

         // Girilen tusa gore islem yap.
         case key of

            #75: // SOLA KAYDIR
                 if sola_kaydir()=0 then yeni_sayi(0);

            #77: // SAGA KAYDIR
                 if saga_kaydir()=0 then yeni_sayi(0);

            #72: // YUKARI KAYDIR
                 if yukari_kaydir()=0 then yeni_sayi(0);

            #80: // ASAGI KAYDIR
                 if asagi_kaydir()=0 then yeni_sayi(0);

            #27: // ESC
                 donguye_gir:=false;

            'u': // UNDO
                 if geri_al(1)=0 then puan(2,0);

            'n': // YENI OYUNA BASLA
                 yeni_sayi(1);
         end;

         // Ekrana yazdir.
         gotoxy(1,1);
         ekran_yazdir;

         // Yenildiyse yenildin yazisini (matrisin ortasýnda) goster.
         if yenildi_mi()=1 then begin
            gotoxy(((sutun*6)div 2)-4+1,((satir*4)div 2)+1);
            textcolor(7);
            textbackground(0);
            write('YENILDIN!');
         end;

      end;
   end;
end.
