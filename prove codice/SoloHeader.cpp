
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

/* 
 * File che legge solo lo Header iniziale (primi 54 byte) e ne stampa il contenuto
 * 
 */

int main(int argc, char *argv[]){
    
    char firma[2],piani[2];
    short color;
    int dimensione_totale, riservato, offset,header_size,larghezza, altezza,compressione,dimensione_immagine;
    int ris_orizzontale,ris_verticale,pallete1,pallete2,l;
    
    char* nomeFile = "output.bmp";
    
    FILE *puntFile;
        puntFile = fopen(nomeFile,"rb");
    if(puntFile == NULL) {
			cout << "Errore nell'apertura del file " << nomeFile << " !\n";
			exit(1);
			}
	cout << "Leggo Header del file '"<< nomeFile <<"'"<< endl;
	
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

	cout << " -----   FINE  PROGRAMMA  ----- \n";
}