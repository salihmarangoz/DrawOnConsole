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
| # OYUN KURALLARI VE BÝLGÝLENDÝRME:							 |
|											 |
| - Oyun ok tuþlarýyla oynanýr.								 |
|											 |
| - Rastgele gelen sayýlar %90->2  %10->4 olasýlýk daðýlýmýna sahiptir.			 |
|											 |
| - Oyunda bir kerede en fazla 3 kere geri alma (undo) hakkýnýz vardýr. Oynadýkça undo   |
|   hakkýnýz artar (max 3).								 |
|											 |
| - Her undo yapýþýnýzda puanýnýzýn %30'unu kaybedersiniz.				 |
|											 |
| - Tek hamlede en fazla iki tane sayý toplanabilir. Örn: 1. satýrda '2 2 2 2' var ise	 |
|   saða kaydýrdýðýnýzda '0 0 4 4' olur.						 |
|											 |
|----------------------------------------------------------------------------------------|
| # BÝLÝNEN SORUNLAR:									 |
|											 |
| - 8x8'lik haritadan daha büyük oyunlarda geçiþler çok yavaþlýyor. 			 |
| > Konsol ekranýnýn belirli limitleri olduðu için bu sorun düzeltilemedi.		 |
|											 |
| - 2048'den sonra sayilari renklerinin ayni olmasi.					 |
| > Degerleri 2048'e kadar girdim. Ekleme yapmak istiyorsaniz kaynak kodunu 		 |
|   kullanabilirsiniz.									 |
|											 |  
x----------------------------------------------------------------------------------------x