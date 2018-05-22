 #include <stdlib.h>
 #include <stdio.h>
 #include <windows.h>
 #define kropka 46
 #define BUFOR 1000

//struktura zawieraj¹ca kod znaku oraz ilosc jego wystapien
 struct element_zrodla
 {
 int indeks_znaku;
 int ilosc_wystapien;
 };
 //struktura o rozmiarze 256
struct element_zrodla model[256];
//licznik elementów ¿ród³a
int licznik_symboli=0;
//struktura reprezentuj¹ca ga³¹Ÿ drzewa: ojciec_.potomek1 oraz ojciec->potomek2
struct galaz_drzewa
{
    int ojciec; 
    int potomek1; 
    int potomek2;
};
//struktura reprezentuj¹ca elementy tablicy kodowania
struct element_tablicy
{
        int symbol;
        unsigned char slowo[4]; 
        int liczba_bitow;
};
//tablica kodowania Huffmana
struct galaz_drzewa drzewo_kodowania[255];
//tablica kodowania Huffmana odczytana z pliku
struct element_tablicy tablica_kodowania[256];
//licznik elementów Ÿród³a
struct element_tablicy tablica_kodowania_odczytana[256]; 

 //funkcja przypisuj¹ca nazwê pliku wraz z nowym rozszerzeniem
 int nazwa_pliku_Huffman(char *nazwa_pliku, char *rozszerzenie, char *nazwa_pliku_z_nowym_rozszerzeniem)
 {
 int i=0; //zmienna pomocnicza
 int n=0; //zmienna pomocnicza
 for(i=0;i<35;i++) //pêtla po d³ugoœci nazwa_pliku_z_nowym_rozszerzeniem
  {
  nazwa_pliku_z_nowym_rozszerzeniem[i]=0; //wyzerowanie wartoœci
  }
//pêtla dopóki w nazwie pliku nie ma kropki b¹dŸ pustego znaku
 while((nazwa_pliku[n]!=kropka)&&(nazwa_pliku[n]!='\0'))
 {
 	n++; //zwiêkszanie zmiennej pomocniczej
 }
 for(i=0;i<n;i++) //pêtla po d³ugoœci nazwy pliku bez rozszerzenia
  {
    nazwa_pliku_z_nowym_rozszerzeniem[i]=nazwa_pliku[i]; //przypisywanie kolejnych znaków nazwy plików bez rozszerzenia
  }
 nazwa_pliku_z_nowym_rozszerzeniem[i]=kropka; //wstawienie kropki
 n=0; //wyzerowanie zmiennej pomocniczej
 i++; //zwiêkszenie iteratora
 
 while(rozszerzenie[n]!='\0') //pêtla dopóki znak nowego rozszerzenia nie jest 0
  {
    nazwa_pliku_z_nowym_rozszerzeniem[i]=rozszerzenie[n]; //przypisywanie znakó rozszerzenia do nowej nazwy pliku
    i++; //zwiêkszenie iteratora
    n++; //zwiêkszenie iteratora
   }
 return 0;
 }

 //metoda tworz¹ca niepe³ny model Huffmana (pomija elementy z iloscia wyst¹pieñ = 0)
 int utworz_model_niepelny_Huffman(struct element_zrodla model[], struct element_zrodla model_pelny[256])
 {
 int n=0; //zmienna pomocnicza (iloœæ elementów struktury niepe³nej)
 int i;
 for(i=0;i<256;i++) //pêtla po wszystkich elementach struktury
 	{
         if(model_pelny[i].ilosc_wystapien>0) //je¿eli znak wystêpuje przynajmniej raz w pe³nym modelu
        {
            model[n].indeks_znaku = model_pelny[i].indeks_znaku; //przypisanie kodu znaku
            model[n].ilosc_wystapien = model_pelny[i].ilosc_wystapien; //przypisanie ilosci wyst¹pieñ
            n++;  //zwiêkszenie zmiennej pomocniczej
        }
 	}
 return n;
 }
//metoda obliczaj¹ca iloœæ wyst¹pieñ poszczególnych znaków
 int Oblicz_model_Huffman(char *nazwa_pliku, char *nazwa_pliku_model, char *nazwa_pliku_wynik, struct element_zrodla model[])
  {
 int liczba_znakow=0; //zmienne pomocnicze
 int n=0;
 unsigned char BUFOR_WEJSCIOWY[BUFOR]; //bufor wykorzystywany przy odczycie danych z pliku
 struct element_zrodla model_pelny[256]; //pe³ny model zród³a
 int i;
 for(i=0;i<256;i++) //pêtla po pe³nym modelu Ÿród³a 
  {
   model_pelny[i].indeks_znaku=i; //przypisanie kodu znaku
   model_pelny[i]. ilosc_wystapien=0; // ustawienie iloœci wyst¹pieñ na 0
   }

 FILE *plik; //wskaŸnik do pliku 
 plik = fopen(nazwa_pliku,"rb"); //otwarcie pliku w trybie "read-binary"
 
 if(plik==NULL)// je¿eli nie uda³o siê otworzyæ pliku do odczytu
 {
  printf("Nie udalo sie otworzyc pliku zrodlowego\n");
 }
 
 else  //je¿eli uda³o siê otworzyæ plik
 {
  printf("Odczytuje plik zrodlowy\n");
  while(n=fread(BUFOR_WEJSCIOWY,sizeof(unsigned char),BUFOR,plik)) //dopóki ¹ odczytywane znaki pliku
  {
    int j; 
    for( j=0;j<n;j++) //pêtla od 0 do n
    {
      model_pelny[BUFOR_WEJSCIOWY[j]].ilosc_wystapien++; //zwiêkszenie iloœci wyst¹pieñ odczytanego znaku z pliku
    }
    liczba_znakow+=n; //zwiêkszenie liczby znaków w pliku
   }
 
 printf("W pliku wejsciowym bylo %d znakow\n",liczba_znakow);
 fclose(plik); //zamkniêcie pliku
 plik = fopen(nazwa_pliku_wynik,"wb"); //otwarcie pliku do zapisu binarnego (plik z ilosci¹ znaków)
 
 if(plik==NULL) //je¿eli uda³o siê otworzyæ plik
 {
   printf("Nie mozna otworzyc pliku %d",nazwa_pliku_wynik);
   exit(EXIT_FAILURE);
  }

 else 
  {
   fprintf(plik,"%d \n",liczba_znakow); 
   fclose(plik); 
  }
 
 licznik_symboli = utworz_model_niepelny_Huffman(model,model_pelny); 
 plik = fopen(nazwa_pliku_model,"wb"); 
 
 if(plik==NULL) 
 {
  printf("Nie mozna otworzyc pliku %d",nazwa_pliku_model);
  exit(EXIT_FAILURE);
}
 
 else 
 {
 int k;
  for( k=0;k<licznik_symboli;k++) 
  {
   fprintf(plik,"%d %d \n",model[k].indeks_znaku,model[k].ilosc_wystapien); 
   }
  fclose(plik); 
 }
}
return 0;
}


 int funkcja_porownujaca(const void *element1, const void *element2)
 {
 const struct element_zrodla *p1 = (const struct element_zrodla*) element1;
 const struct element_zrodla *p2 =(const struct element_zrodla*) element2;
 if((p1->ilosc_wystapien)==(p2->ilosc_wystapien))
 {
  return 0;
 }
 else if ((p1->ilosc_wystapien)<(p2->ilosc_wystapien))
 {
  return 1;
 }
 else
 {
 return -1;
 }
}

 
int posortuj_model_Huffmana(struct element_zrodla model[],int liczba_elementow, char* plik_model_posortowany)
{
 
 qsort(model,liczba_elementow,sizeof(struct element_zrodla),funkcja_porownujaca);
 
 FILE *plik;

 plik=fopen(plik_model_posortowany,"wb");

 if(plik==NULL)
 {
 printf("Blad w trakcie otwierania pliku do zapisu posortowanego modelu\n");
 exit(EXIT_FAILURE);
 }
 else
 {
  
          int i; 
          for(i=0;i<liczba_elementow;i++)
          {
          
          fprintf(plik,"%d %d\n",model[i].indeks_znaku,model[i].ilosc_wystapien);
          }
         
          fclose(plik);
 }
}


int utworz_drzewo(struct element_zrodla model[], struct galaz_drzewa drzewo[], int liczba_symboli, char* plik_drzewa, char* plik_mod_model)
{
	
    int licznik_pomocniczy = liczba_symboli; 
    int licznik_symboli = liczba_symboli;
    int licznik_elementow_drzewa = 0; 
    int licznik_dodanych = 0; 
    int i=0;
    for(i=0;i<liczba_symboli-1;i++) 
    {
        drzewo[i].ojciec = 256+i; 
        drzewo[i].potomek1 = model[liczba_symboli-1-i].indeks_znaku ; 
        drzewo[i].potomek2 = model[liczba_symboli-2-i].indeks_znaku ; 
        model[liczba_symboli-2-i].indeks_znaku=256+i; 
        model[liczba_symboli-2-i].ilosc_wystapien = model[liczba_symboli-2-i].ilosc_wystapien+model[liczba_symboli-1-i].ilosc_wystapien; 
        licznik_symboli--; 
        licznik_elementow_drzewa++; 
        qsort(model,licznik_symboli,sizeof(struct element_zrodla),funkcja_porownujaca); 
        licznik_dodanych++;
	}
    FILE *plik; 
    plik=fopen(plik_mod_model,"wb"); 
    if(plik==NULL) 
    {
        printf("Blad w trakcie otwierania pliku ze zmodyfikowanym modelem\n"), 
        exit(EXIT_FAILURE);
    }
    else 
    {
        for(i=0;i<liczba_symboli;i++) 
        {
            fprintf(plik,"%d %d\n",model[i].indeks_znaku,model[i].ilosc_wystapien); 
        }
        fclose(plik); 
    }
    plik=fopen(plik_drzewa,"a+"); 
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z drzewem\n"); 
        exit(EXIT_FAILURE);
    }
    else 
    {
        fprintf(plik,"%d \n",licznik_dodanych); 
        for(i=0;i<licznik_elementow_drzewa;i++) 
        {
            fprintf(plik,"%d %d %d\n",drzewo[i].ojciec,drzewo[i].potomek1,drzewo[i].potomek2); 
        }
        fclose(plik); 
    }
}

int utworz_tablice_kodowania(struct galaz_drzewa drzewo[], int liczba_symboli,struct element_tablicy tablica[],char *nazwa_pliku_tablicy, char* nazwa_pliku_tablicy_mod)
{
    int liczba_drzewa = liczba_symboli-2; 
    int licznik_tablicy = 0; 
    int ojciec; 
    int indeks_ojca_tablica ; 
    int liczba_bajtow; 
    int liczba_bitow; 
    int jedynka = 1; 
    unsigned char bajty[4]; 
    
    int i;
    for(i=0;i<2*liczba_symboli;i++)
    {
        tablica[i].symbol=0; 
        tablica[i].liczba_bitow=0;
        int j;
        for(j=0;j<4;j++)
        {
            tablica[i].slowo[j]=0;
        }
    }

    tablica[licznik_tablicy].symbol=drzewo[liczba_drzewa].ojciec; 
    while(liczba_drzewa+1)
    {
        ojciec = drzewo[liczba_drzewa].ojciec; 
        tablica[licznik_tablicy+1].symbol=drzewo[liczba_drzewa].potomek1; 
        tablica[licznik_tablicy+2].symbol=drzewo[liczba_drzewa].potomek2; 
        indeks_ojca_tablica=- 1; 
        int i;
        for(i=0;i<licznik_tablicy+1;i++)
        {
            if((tablica[i].symbol) == (ojciec)) 
            {
                indeks_ojca_tablica=i;
                break; 
            }
        }
        if(indeks_ojca_tablica==-1) 
        {
            printf("Blad tworzenia tablicy kodowania"); 
            exit(EXIT_FAILURE);
        }
        else 
        {
             int i;
            for(i=0;i<4;i++)
            {
                tablica[licznik_tablicy+1].slowo[i]=tablica[indeks_ojca_tablica].slowo[i]; 
                tablica[licznik_tablicy+2].slowo[i]=tablica[indeks_ojca_tablica].slowo[i];
            }
            liczba_bajtow = (tablica[indeks_ojca_tablica].liczba_bitow)/8; 
            liczba_bitow = (tablica[indeks_ojca_tablica].liczba_bitow )%8;
            jedynka = 1;
            jedynka = jedynka<<7-liczba_bitow; 
            jedynka = tablica[indeks_ojca_tablica].slowo[liczba_bajtow]|jedynka; 
            tablica[licznik_tablicy+1].slowo[liczba_bajtow]=jedynka; 
            tablica[licznik_tablicy+1].liczba_bitow=tablica[indeks_ojca_tablica].liczba_bitow+1 ; 
            tablica[licznik_tablicy+2].liczba_bitow=tablica[indeks_ojca_tablica].liczba_bitow+1 ; 
            licznik_tablicy = licznik_tablicy+2 ; 
            liczba_drzewa-- ; 
        }
    }

    FILE *plik; 
    plik = fopen(nazwa_pliku_tablicy,"wb"); 
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z tablica kodowania\n"); 
        exit(EXIT_FAILURE);
    }
    else 
    {
         int i;
        for(i=0;i<2*liczba_symboli-1;i++)
        {   
            int j;
            for(j=0;j<4;j++)
            {
                bajty[j]=tablica[i].slowo[j];
                }
            fprintf(plik,"%d %d %d %d %d %d\n",tablica[i].symbol,bajty[0],bajty[1],bajty[2],bajty[3],tablica[i].liczba_bitow); 
            }
        fclose(plik); 
	}
    plik = fopen(nazwa_pliku_tablicy_mod,"wb"); 
    if(plik==NULL) 
    {   printf("Blad w trakcie otwierania pliku z tablica kodowania\n"); 
        exit(EXIT_FAILURE);
        }
    else
    {
        int i;
        for(i=0;i<2*liczba_symboli-1;i++)
        {
            if(tablica[i].symbol<256) 
            {
                                      int j;
                for(j=0;j<4;j++)
                {
                     bajty[j]=tablica[i].slowo[j];
                     }
                fprintf(plik,"%d %d %d %d %d %d\n",tablica[i].symbol,bajty[0],bajty[1],bajty[2],bajty[3],tablica[i].liczba_bitow); 
			}
        }
        fclose(plik); 
    }
}



int odczytaj_tablice_kodowania(struct element_tablicy tablica_kodowania[], char *plik_mod_tablicy)
{
int dlugosc_slowa;
int bajty[4];
int kod_znaku;
int liczba_znakow = 0;
FILE *plik;
plik = fopen(plik_mod_tablicy, "rb");
if(plik == NULL)
{
printf("Blad otwarcia pliku ze zmodyfikowana tablica kodowania\n");
exit(EXIT_FAILURE);
}
else
{
while(fscanf(plik, "%d %d %d %d %d %d \n", &kod_znaku, &bajty[0], &bajty[1], &bajty[2], &bajty[3], &dlugosc_slowa) != EOF)
{
if(dlugosc_slowa > 0)
{
tablica_kodowania[liczba_znakow].symbol = kod_znaku;
tablica_kodowania[liczba_znakow].liczba_bitow = dlugosc_slowa;
int i=0;
for( i = 0; i < 4; i++)
{
tablica_kodowania[liczba_znakow].slowo[i] = bajty[i];
}
liczba_znakow++;
}
}
fclose(plik);
printf("Odczytano %d znakow z pliku ze zmodyfikowana tablica kodowania\n", liczba_znakow);
}
return liczba_znakow;
}

int kompresja_Huffman(struct element_tablicy tablica_kodowania[],char* plik_wejsciowy, char* plik_wynikowy, int liczba_elementow_tablicy)
{
int n; 
int liczba_bitow_znaku;
int suma_bitow;
int kursor;
int kursor_out;
int liczba_pozostalych;
int liczba_wolne_bity;
int liczba_zak_symboli;
int numer_slowa;
int numer_bajtu;
int dopisek;
unsigned char maska;
unsigned char znak;
unsigned char bufor_wejsciowy[BUFOR];
unsigned char bufor_wyjsciowy[BUFOR];
unsigned char slowo_znaku[4];
unsigned char bajt_out;
FILE *plik; 
FILE *plik_out; 
plik_out = fopen(plik_wynikowy,"a+"); 
if(plik_out==NULL)
{
printf("Nie udalo sie otworzyc pliku wynikowego \n");
exit(EXIT_FAILURE);
}
plik = fopen(plik_wejsciowy,"rb"); 
if(plik==NULL)
{
printf("Nie udalo sie otworzyc pliku wejsciowego \n");
exit(EXIT_FAILURE);
}

kursor_out=0; 
numer_slowa=0;
numer_bajtu=0;
bajt_out=0;
liczba_zak_symboli=0;
while(n=fread(bufor_wejsciowy,sizeof(unsigned char),BUFOR,plik))
{
	int i=0,j=0,k=0;
for( i=0;i<n;i++)
{
znak=bufor_wejsciowy[i]; 
for( j=0;j<liczba_elementow_tablicy;j++)
{
if(znak==tablica_kodowania[j].symbol)
{

for( k=0;k<4;k++)
{
slowo_znaku[k]=tablica_kodowania[j].slowo[k];
}
liczba_bitow_znaku=tablica_kodowania[j].liczba_bitow; 
suma_bitow=suma_bitow+liczba_bitow_znaku; 
kursor=0;
numer_slowa=0;
break;
}
}

while(liczba_bitow_znaku>0) 
{
liczba_pozostalych = 8 - kursor; 
if(liczba_pozostalych>liczba_bitow_znaku) 
{
liczba_pozostalych = liczba_bitow_znaku;
}
liczba_wolne_bity = 8- kursor_out; 
if(liczba_pozostalych<liczba_wolne_bity)
{
liczba_wolne_bity = liczba_pozostalych;
}
maska = ((255>>kursor)&(255<<(8-(kursor+liczba_wolne_bity)))); 
dopisek = slowo_znaku[numer_slowa]&maska; 
dopisek = dopisek<<kursor; 
dopisek = dopisek>>kursor_out;
bajt_out=bajt_out|dopisek; 
kursor_out+=liczba_wolne_bity; 
liczba_bitow_znaku-=liczba_wolne_bity; 
kursor+=liczba_wolne_bity;
liczba_pozostalych-=liczba_wolne_bity;
if (liczba_pozostalych==0) 
{
numer_slowa++; 
kursor=0; 
if(liczba_bitow_znaku>=8){
	liczba_pozostalych=8;
	}
else 
{
	liczba_pozostalych=liczba_bitow_znaku;
}
}
if (kursor_out==8)
{
fwrite(&bajt_out, sizeof(unsigned char), 1, plik_out);
numer_bajtu++; 
kursor_out=0; 
bajt_out=0;
}
}
liczba_zak_symboli++; 
}
}
printf("Bajt wyjsciowy numer %d kod hdec %x \n", numer_bajtu, bajt_out);
fwrite (&bajt_out, sizeof(unsigned char), 1, plik_out);

printf("Liczba zakodowanych znakow pliku kompresowanego: %d\n", liczba_zak_symboli);
printf("Liczba bajtow w pliku skompresowanym: %d\n", numer_bajtu+1);
printf("Wskaznik upakowania: %5.1f procent\n", 100*(float)(numer_bajtu+1)/(float)liczba_zak_symboli);
fclose(plik);
fclose(plik_out);

return suma_bitow; 
}

int dekompresja_Huffmana(char *nazwa_pliku_skom,char *nazwa_pliku_dekom,struct galaz_drzewa drzewo_kodowania[])
{
    int n;
    int m;
    int ojciec;
    int potomek1;
    int potomek2;
    int ilosc_symboli;
    int rozmiar_drzewa;
    int licznik_drzewa;
    int suma_symboli;
    int suma_bitow;
    unsigned char bajt_wejsciowy;
    unsigned char bajt_wyjsciowy;
    unsigned char maska=128;
    unsigned char bit;
    
    FILE *plik_dek=NULL;
    plik_dek=fopen(nazwa_pliku_dekom,"wb");
    if(plik_dek==NULL)
    {
                      printf("Blad w trakcie otwierania pliku zdekodowanego do zapisu\n");
                      exit(EXIT_FAILURE);
}
    FILE *plik=NULL;
    plik=fopen(nazwa_pliku_skom,"rb");
    if(plik==NULL)
    {
                      printf("Blad w trakcie otwierania pliku zdekodowanego pliku\n");
                      exit(EXIT_FAILURE);
}
else
{
    fscanf(plik, "%d\n %d\n", &ilosc_symboli, &rozmiar_drzewa);
    licznik_drzewa=0;
    int i=0;
    for(i=0;i<rozmiar_drzewa;i++)
    {
                                 fscanf(plik,"%d %d %d\n",&ojciec,&potomek1,&potomek2);
                                 drzewo_kodowania[licznik_drzewa].ojciec=ojciec;
                                 drzewo_kodowania[licznik_drzewa].potomek1=potomek1;
                                 drzewo_kodowania[licznik_drzewa].potomek2=potomek2;
                                 licznik_drzewa++;
    }
    suma_symboli=0;
    suma_bitow=0;
    ojciec=drzewo_kodowania[rozmiar_drzewa-1].ojciec;
    potomek1=drzewo_kodowania[rozmiar_drzewa-1].potomek1;
    potomek2=drzewo_kodowania[rozmiar_drzewa-1].potomek2;


while(suma_symboli<ilosc_symboli)
{
	n=fread(&bajt_wejsciowy,sizeof(unsigned char),1,plik);
	if(n==1)
		{
            int j;
		for(j=0; j<8; j++)
			{		
			bit=bajt_wejsciowy&maska;
			bajt_wejsciowy=bajt_wejsciowy<<1;
			suma_bitow++;
			if(bit)
				{
				ojciec=potomek1;
				}
			else
				{
				ojciec=potomek2;
				}
			if(ojciec<256)
				{
				bajt_wyjsciowy=ojciec;
				fwrite(&bajt_wyjsciowy,sizeof(unsigned char),1,plik_dek);
				suma_symboli++;
				licznik_drzewa=rozmiar_drzewa-1;
				ojciec=drzewo_kodowania[rozmiar_drzewa-1].ojciec;
				potomek1=drzewo_kodowania[rozmiar_drzewa-1].potomek1;
				potomek2=drzewo_kodowania[rozmiar_drzewa-1].potomek2;
				if(suma_symboli==ilosc_symboli)
					{
					break;
					}
				

}
else
{
m=rozmiar_drzewa-1;
while(m>=0)
{
if(drzewo_kodowania[m].ojciec==ojciec)
{
licznik_drzewa=m;
m=-1;
}
m=m-1;
}
potomek1=drzewo_kodowania[licznik_drzewa].potomek1;
potomek2=drzewo_kodowania[licznik_drzewa].potomek2;
}
}
}
}
printf("Liczba odczytanych bitow: %d, liczba zdekodowanych symboli %d. \n", suma_bitow, suma_symboli);
fclose(plik);
fclose(plik_dek);
return(suma_symboli);
}
}


unsigned long int crc32_tab[256] =
{/* CRC-32 sta³a tablica kodów ASCII */
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
  0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
  0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
  0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
  0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
  0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
  0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
  0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
  0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
  0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
  0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
  0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
  0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
  0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
  0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
  0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
  0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
  0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
  0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
  0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
  0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};
unsigned long int oblicz_CRC32(char * plik_wejsciowy, char * plik_crc, bool pytaj_o_zapis)
{
    unsigned char buf[1024];
    unsigned long int crc32;
    FILE * plik = NULL;
    int n;
    plik = fopen(plik_wejsciowy, "rb");

    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z danymi\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        crc32 = 0xffffffff;
        while(n = fread(buf, 1, 1024, plik))
        {
        	int i=0;
            for(i = 0; i < n; i++)
            {
                crc32 = (crc32 >> 8)^crc32_tab[(crc32 & 0xff) ^ buf[i]];
            }
        }
        crc32 ^= 0xffffffff;
        fclose(plik);
        printf("CRC-32 = %1x\n", crc32);
        if(pytaj_o_zapis)
        {
            printf("Czy chcesz zapisac obliczona wartosc CRC32 do pliku? 1 - TAK, inny znak - NIE");
            char c;
            scanf("%d", &c);
            if(c==1)
            {
                FILE* plik_crc32 = NULL;
                plik_crc32 = fopen(plik_crc, "wb");
                if(plik_crc32 == NULL)
                {
                    printf("Nie udalo sie otworzyc pliku do zapisu CRC32\n");
                    exit(EXIT_FAILURE);
                }
                fprintf(plik_crc32, "%lx\n", crc32);
                fclose(plik_crc32);
            }
        }
        return crc32;
    }
}


//funkcja odczytuj¹ca CRC32
unsigned long int odczytaj_CRC32(char *plik_crc)
{
	unsigned long int crc32; //crc32
	FILE *plik=NULL;
	plik=fopen(plik_crc,"rb");
	if(plik==NULL)
		{
		printf("Nie udalo sie otworzyc pliku z CRC32\n");
		exit(EXIT_FAILURE);
		}
	else //jezeli udalo sie otworzyc plik
		{
		fscanf(plik,"%lx\n", &crc32);
		fclose(plik);
		return crc32;
		}
}
 
void menu()
{
  printf("1.Kompresja metoda Huffmana\n");
  printf("2.Dekompresja metoda Huffmana\n");
  printf("3.Obliczenie CRC32 dla pliku\n");
  printf("4.Sprawdzenie CRC32\n");
  printf("5.Ponowne wyswietlenie menu\n");
  printf("Inny znak - wyjscie\n");
}
 
 
int main(int argc, char *argv[])
{
 int liczba_symboli;
 char opcja=1; 
 char znak; 
 char nazwa_pliku[30]; 
 char nazwa_pliku_skom[30]; 
 char nazwa_pliku_modelu[35]; 
 char nazwa_pliku_mod_modelu[35]; 
 char nazwa_pliku_sort[35]; 
 char nazwa_pliku_drzewo[35];
 char nazwa_pliku_tablica[35];
 char nazwa_pliku_tablica_mod[35];
 char nazwa_pliku_wynikowego[35];
 char nazwa_pliku_dekom[35];
 char rozszerzenie_modelu[] = "model"; 
 char rozszerzenie_mod_modelu[]="modmodel"; 
 char rozszerzenie_sort[] = "sort"; 
 char rozszerzenie_drzewo[] = "tree"; 
 char rozszerzenie_tablica[] = "tablica";
 char rozszerzenie_tablica_mod[] = "tablicamod";
 char rozszerzenie_wynikowego[] = "wow";
 char rozszerzenie_dekom[]="txt";

 printf("\t\t\t\t LABORATORIUM TEORII INFORMACJI I KODOWANIA\n");
 printf("\t\t\t Kompresja, dekompresja metoda Huffmana. Obliczanie CRC32.\n\n");

menu();
printf("\n");
 while(opcja==1 || opcja==2 || opcja==3 || opcja==4 || opcja==5)
 {

  		scanf("%d",&opcja);
  		
 if(opcja==1)
 {
		printf("\nPodaj nazwe pliku do kompresji wraz z rozszerzeniem\n");
		scanf("%s",&nazwa_pliku);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_modelu,nazwa_pliku_modelu);
		printf("Nazwa pliku z modelem: %s\n",nazwa_pliku_modelu);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_sort,nazwa_pliku_sort);
		printf("Nazwa pliku z posortowanym modelem: %s \n",nazwa_pliku_sort);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_mod_modelu,nazwa_pliku_mod_modelu);
		printf("Nazwa pliku ze zmodyfikowanym modelem: %s \n",nazwa_pliku_mod_modelu);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_drzewo,nazwa_pliku_drzewo);
		printf("Nazwa pliku z drzewem kodowania: %s \n",nazwa_pliku_drzewo);
		
		Oblicz_model_Huffman(nazwa_pliku,nazwa_pliku_modelu,nazwa_pliku_drzewo,model); 
		posortuj_model_Huffmana(model,licznik_symboli,nazwa_pliku_sort); 
		utworz_drzewo(model,drzewo_kodowania,licznik_symboli, nazwa_pliku_drzewo,nazwa_pliku_mod_modelu);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_tablica,nazwa_pliku_tablica);
		printf("Nazwa pliku z tablica kodowania: %s \n",nazwa_pliku_tablica);
		
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_tablica_mod,nazwa_pliku_tablica_mod);
		printf("Nazwa pliku ze zmodyfikowana tablica kodowania: %s \n",nazwa_pliku_tablica_mod);
		
		utworz_tablice_kodowania(drzewo_kodowania, licznik_symboli,tablica_kodowania,nazwa_pliku_tablica,nazwa_pliku_tablica_mod);
		
		int liczba_elementow_tablicy = odczytaj_tablice_kodowania(tablica_kodowania,nazwa_pliku_tablica_mod);
		nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_wynikowego,nazwa_pliku_wynikowego);
		printf("Nazwa pliku wynikowego: %s \n",nazwa_pliku_wynikowego);

		

		CopyFileA(nazwa_pliku_drzewo,nazwa_pliku_wynikowego, true );
		kompresja_Huffman(tablica_kodowania,nazwa_pliku, nazwa_pliku_wynikowego,liczba_elementow_tablicy);

} 
 	if(opcja==2)
 {
		printf("\nPodaj nazwe pliku do dekompresji wraz z rozszerzeniem\n");
		scanf("%s",&nazwa_pliku_skom);
		printf("Podaj nazwe pliku wynikowego wraz z rozszerzeniem\n");
		scanf("%s",&nazwa_pliku_dekom);
		dekompresja_Huffmana(nazwa_pliku_skom,nazwa_pliku_dekom,drzewo_kodowania);

 }
 

  	if(opcja==3)
 {
		char plik_wejsciowy[30], plik_crc[]="CRC.txt";
		printf("\nPodaj nazwe pliku do obliczenia CRC\n");
		scanf("%s",&plik_wejsciowy);
		oblicz_CRC32(plik_wejsciowy, plik_crc, 1);

}

    if(opcja==4)
 {
		char plik_wejsciowy[30],plik_crc[30];
		printf("\nPodaj nazwe pliku do obliczenia CRC\n");
		scanf("%s",&plik_wejsciowy);
		printf("Podaj nazwe pliku zawierajaca CRC\n");
		scanf("%s",&plik_crc);
		long stary = odczytaj_CRC32(plik_crc);
		int pytaj_o_zapis=0;
		long nowy = oblicz_CRC32(plik_wejsciowy, plik_crc, pytaj_o_zapis);
			
 	 if (nowy==stary)
 	{
		printf("CRC32 jest takie samo\n");
	}
  else
  {
		printf("CRC32 jest rozne\n");
	}  
}
	if(opcja==5)
	{
		printf("\n");
		menu();
		printf("\n");
	}
}
}

