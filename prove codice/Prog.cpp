/*

File programma principale

*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
//#include "Header.cpp"
//#include "ImageMono.cpp"
//#include "ImageRGB.cpp"

/*
String FILE_IN;
String FILE_OUT;
String COMANDO;
int larghezzaPX, altezzaPX, larghezzaOUT, altezzaOUT;
int offsetIMG;


Header readHeader(String nomeFile){
    // ritorna l'oggetto Header creato con i dati letti dalla linea di comando
    
    
}

ImageMono readMonoColorImage(String nomeFile, Header* header){
    // crea un oggetto ImageMono che contiene la Bitmap dell'immagine monocolore
}

ImageRGB readRGBImage(String nomeFile, Header* header){
    // crea un oggetto ImageRGB che contiene la Bitmap dell'immagine a colori 
}*/

void bin8(int n, char bina[])
{
     int i;
     for (i=0 ; i < 8 ; i++)
     {
         bina[7-i]=(n % 2)+'0';
         n = (n - (n % 2) ) /2;
     }
}


int main(int argc, char *argv[]){
    // considero gli argomenti passati da linea di comando
    
    char firma[2],piani[2];
    short color;
    int dimensione_totale, riservato, offset,header_size,larghezza, altezza,compressione,dimensione_immagine;
    int ris_orizzontale,ris_verticale,pallete1,pallete2,l;
    
    char dig[3], bina[8];
    
    
    unsigned char headerContent[54];
    char* nomeFile = "bmp7x7_04bit.bmp";
    
    FILE *puntFile;
        puntFile = fopen(nomeFile,"rb");
        
	fseek(puntFile, 0, 0);
	fread(firma,2, 1, puntFile);
	
	fseek(puntFile,2,0);
	fread(&dimensione_totale, 1, sizeof(int), puntFile);
	
	fseek(puntFile,6,0);
	fread(&riservato, 1, sizeof(int), puntFile);
	
	fseek(puntFile,10,0);
	fread(&offset, 1, sizeof(int), puntFile);
	
	fseek(puntFile,14,0);
	fread(&header_size,sizeof(int), 1, puntFile);
	
	fseek(puntFile,18,0);
	fread(&larghezza,sizeof(int), 1, puntFile);
	
	fseek(puntFile,22,0);
	fread(&altezza,sizeof(int), 1, puntFile);
	
	fseek(puntFile,26,0);
	fread(piani,2, 1, puntFile);
	
	fseek(puntFile,28,0);
	fread(&color,sizeof(int), 1, puntFile);
	
	fseek(puntFile,30,0);
	fread(&compressione,sizeof(int), 1, puntFile);
	
	fseek(puntFile,34,0);
	fread(&dimensione_immagine,sizeof(int), 1, puntFile);
	
	fseek(puntFile,38,0);
	fread(&ris_orizzontale,sizeof(int), 1, puntFile);
	
	fseek(puntFile,42,0);
	fread(&ris_verticale,sizeof(int), 1, puntFile);
	
	fseek(puntFile,46,0);
	fread(&pallete1,sizeof(int), 1, puntFile);
	
	fseek(puntFile,50,0);
	fread(&pallete2,sizeof(int), 1, puntFile);

	
	
	
	cout<<"Firma: "<<firma[0]<<firma[1]<<endl;
	cout<<"Dimensione totale: "<<dimensione_totale<<endl;
	cout<<"Reserved: "<<riservato<<endl;
	cout<<"Offset: "<<offset<<endl;
	cout<<"Header size: "<<header_size<<endl;
	cout<<"Larghezza: "<<larghezza<<endl;
	cout<<"Altezza: "<<altezza<<endl;
	cout<<"Piani: "<<piani[0]<<piani[1]<<endl;
	cout<<"Colori: "<<color<<endl;
	cout<<"Compressione: "<<compressione<<endl;
	cout<<"Dimensione immagine: "<<dimensione_immagine<<endl;
	cout<<"Risoluzione orizzontale: "<<ris_orizzontale<<endl;
	cout<<"Risoluzione verticale: "<<ris_verticale<<endl;
	cout<<"Pallette: "<<pallete1<<endl;
	cout<<"Pallette used: "<<pallete2<<endl;
	
	int LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;
	fseek(puntFile,offset,0);
	
	unsigned char bitMap[LungRow*altezza];
	
	l=fread(bitMap, sizeof(unsigned char), LungRow, puntFile);
	int riga=0,n=0,b=0;
	 while (l == LungRow){
/*
	for ( n=0 ; n < LungRow ; n++){
		if ( (color==24) || (color==8) )
		{
		    if (b==0) printf("[");
		    printf("%02X",bitMap[n+LungRow*riga]);
		    if (b==(color-1)) printf("]");
		    b=(b+1) % color;
		}
		else if (color==4)
		{
		    sprintf(dig, "%02X", bitMap[n+LungRow*riga]);
		    printf("[%c] [%c] ",dig[0],dig[1]);
		}
		else if (color==1)
		{
		    l=(int)bitMap[n+LungRow*riga];
		    bin8(l,bina);
		    printf("[%s]",bina);
		}
	}
	    printf("\n");
*/
	    riga++;
	    l=fread(bitMap+LungRow*riga, sizeof(unsigned char), LungRow, puntFile);

	 }
	
	
	
	
	
	
	/*
        fseek(puntFile, 0, 0);
        fread(headerContent, 54, 1, puntFile);
        
        for(int i=0; i<54; i++)
        printf(headerContent[i]);
        
        */
    
    // leggo Header e creo la corrispondente classe di immagine (ImageRGB o ImageMono)
		
    // prendo l'offset dell'immagine e leggo la bitmap
        
    }