x----------------------------------------------------------------------------------------x 
|                                       2048 OYUNU					 |                                      
| @ Author  : Salih Marangoz (salih285@gmail.com - www.indirgecim.tr.gg)		 |
| @ Date    : 07.01.2015								 |
| @ Version : 1.02									 |
|											 |
| -Coded with Pascal Language.								 |
|											 |
| -Based on 1024 by Veewo Studio and conceptually similar to Threes by Asher Vollmer.	 |
|											 |
|----------------------------------------------------------------------------------------|
| # OYUN KURALLARI VE B�LG�LEND�RME:							 |
|											 |
| - Oyun ok tu�lar�yla oynan�r.								 |
|											 |
| - Rastgele gelen say�lar %90->2  %10->4 olas�l�k da��l�m�na sahiptir.			 |
|											 |
| - Oyunda bir kerede en fazla 3 kere geri alma (undo) hakk�n�z vard�r. Oynad�k�a undo   |
|   hakk�n�z artar (max 3).								 |
|											 |
| - Her undo yap���n�zda puan�n�z�n %30'unu kaybedersiniz.				 |
|											 |
| - Tek hamlede en fazla iki tane say� toplanabilir. �rn: 1. sat�rda '2 2 2 2' var ise	 |
|   sa�a kayd�rd���n�zda '0 0 4 4' olur.						 |
|											 |
|----------------------------------------------------------------------------------------|
| # B�L�NEN SORUNLAR:									 |
|											 |
| - 8x8'lik haritadan daha b�y�k oyunlarda ge�i�ler �ok yava�l�yor. 			 |
| > Konsol ekran�n�n belirli limitleri oldu�u i�in bu sorun d�zeltilemedi.		 |
|											 |
| - 2048'den sonra sayilari renklerinin ayni olmasi.					 |
| > Degerleri 2048'e kadar girdim. Ekleme yapmak istiyorsaniz kaynak kodunu 		 |
|   kullanabilirsiniz.									 |
|											 |  
x----------------------------------------------------------------------------------------x