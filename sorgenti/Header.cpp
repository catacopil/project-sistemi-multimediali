
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define DIM_HEADER_INIZIALE 54
#define DIM_PALETTE 1024
#define DIM_HEAD_BMP24 54

static void CaricaBmp24(unsigned char* Headd, const char *Nome, unsigned char *Dove, int x, int y)
{
	FILE *fHan = fopen(Nome, "rb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fread(Headd, DIM_HEAD_BMP24, 1, fHan);
	fread(Dove, x * y * 3, 1, fHan);
	fclose(fHan);
}

static void SalvaBmp24(unsigned char* Headd, const char *Nome, unsigned char *DaDove, int x, int y)
{
	FILE *fHan = fopen(Nome, "wb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fwrite(Headd, DIM_HEAD_BMP24, 1, fHan);
	fwrite(DaDove, x * y * 3, 1, fHan);
	fclose(fHan);
}

class Header{	
private:
	unsigned char Headd[DIM_HEAD_BMP24];
	unsigned char Dove[512*512*3];
    unsigned char headerContent[DIM_HEADER_INIZIALE];       // contiene tutti i byte dell'Header (ad eccezione della Palette)
    unsigned char paletteContent[DIM_PALETTE];				// contiene tutti i byte della Palette
	char firma[2],piani[2];
    short color;
    int dimensione_totale, riservato, offset, header_size, larghezza, altezza, compressione, dimensione_immagine;
    int ris_orizzontale,ris_verticale,pallete1,pallete2,l;
	bool esistePalette;
    
public:
    Header(char* nomeFile){				// Costruttore che legge l'intero Header (si presume sia quello standard da 54 Byte)
        
		FILE *puntFile;
        puntFile = fopen(nomeFile,"rb");
		if(puntFile == NULL) {
			cout << "Errore nell'apertura del file " << nomeFile << " !\n";
			exit(1);
			}
        
		fseek(puntFile, 0, 0);
		fread(headerContent, DIM_HEADER_INIZIALE, 1, puntFile);		// leggo l'intero header e lo metto in headerContent
		cout << "Header letto: \n";
		for (int i=0; i<DIM_HEADER_INIZIALE; i++)
			cout << i<<") "<< headerContent[i]<<endl;
		
		fseek(puntFile, 0, 0);
		fread(firma,2, 1, puntFile);
	
		fseek(puntFile,2,0);
		fread(&dimensione_totale, 1, sizeof(int), puntFile);			// legge la dimensione totale dell'immagine
		
		fseek(puntFile,6,0);
		fread(&riservato, 1, sizeof(int), puntFile);
		
		fseek(puntFile,10,0);
		fread(&offset, 1, sizeof(int), puntFile);					// legge il punto in cui inizia la bitmap
		
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
		if (color==24)
			esistePalette = false;
		else 
			esistePalette = true;
		
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
		
		// lettura palette
		if (esistePalette){
			bool palette_standard = (offset == (14+header_size+DIM_PALETTE));
			if (palette_standard){
				cout << "Ok, palette standard \n";
				fseek(puntFile, DIM_PALETTE, 0);
				fread(paletteContent, DIM_PALETTE, 1, puntFile);
			}
			else
				cout << "Attenzione: dimensione Palette diversa da "<<DIM_PALETTE<<endl;
		}
		
		fclose(puntFile);											// chiusura del file
		
		// PROVA LETTURA/SCRITTURA CON LE FUNZIONI DEL PROF
		CaricaBmp24(Headd, nomeFile, Dove, larghezza, altezza);
		
		
	}

	int getDimensioneTot(){					// get e set per le dimensioni, i bit impiegati per ogni pixel e l'offset immagine
		return dimensione_totale;
	}
	
	int getLarghezza(){
		return larghezza;
	}
	
	int getAltezza(){
		return altezza;
	}
	
	int getColor(){
		return color;
	}
	
	int getOffsetIMG(){
		return offset;
	}
	
	void setLarghezza(int newLarg){
		larghezza = newLarg;
	}
	
	void setAltezza(int newAlt){
		altezza = newAlt;
	}

	void setOffset(int newOffset){
		offset = newOffset;
	}
		
	void setDimensioneTot(int newDim){
		dimensione_totale = newDim;
	}
	
	void stampaInfoHeader(){					// stampa nella console le informazioni dell'header
		cout<<"------  Informazioni Header letto  ------\n";
		cout<<"Firma: "<<firma[0]<<firma[1]<<endl;
		cout<<"Dimensione totale: "<<dimensione_totale<<endl;
		cout<<"Reserved: "<<riservato<<endl;
		cout<<"Offset: "<<offset<<endl;
		cout<<"Header size: "<<header_size<<endl;
		cout<<"Larghezza: "<<larghezza<< " px"<<endl;
		cout<<"Altezza: "<<altezza<< " px"<<endl;
		cout<<"Piani: "<<piani[0]<<piani[1]<<endl;
		cout<<"Colori: "<<color <<" bit"<<endl;
		cout<<"Compressione: "<<compressione<<endl;
		cout<<"Dimensione immagine: "<<dimensione_immagine<<endl;
		cout<<"Risoluzione orizzontale: "<<ris_orizzontale<<endl;
		cout<<"Risoluzione verticale: "<<ris_verticale<<endl;
		cout<<"Pallette: "<<pallete1<<endl;
		cout<<"Pallette used: "<<pallete2<<endl;
		cout << "---------------------------------------\n";
	}
	
	void scriviHeader(char* nomeFile){	
	// scrive l'Header nel nuovo file, a partire da quello che aveva letto inizialmente, riscrive sempre le dimensioni e l'offset immagine
		/*
		FILE *puntFile;
        puntFile = fopen(nomeFile,"wb");			// apre il file in modalità wb, se non esiste lo crea
		if(puntFile == NULL) {
			cout << "Errore nell'apertura del file " << nomeFile << " !\n";
			exit(1);
			}
		fseek(puntFile, 0, 0);
		fwrite(headerContent, DIM_HEADER_INIZIALE, 1, puntFile);			// scrive nel file tutto lo Header
		if (esistePalette)
			fwrite(paletteContent, DIM_PALETTE, 1, puntFile);					// scrive nel file la Palette
		
		// qua devo riscrivere le informazioni che potrebbero essere cambiate, ad es. le dimensioni
		
		
		fclose(puntFile);
		*/
		SalvaBmp24(Headd, nomeFile, Dove, larghezza, altezza);
		
	}
};


//TODO: fare classe Header con lettura/scrittura su file
//TODO: vedere il resize e applicarlo sul codice