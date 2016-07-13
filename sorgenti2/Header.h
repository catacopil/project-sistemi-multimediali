
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define DIM_HEADER_INIZIALE 54
#define DIM_PALETTE 1024

class Header{
private:
	unsigned char headerContent[DIM_HEADER_INIZIALE];       // contiene tutti i byte dell'Header (ad eccezione della Palette)
	unsigned char paletteContent[DIM_PALETTE];				// contiene tutti i byte della Palette
	char firma[2],piani[2];
	short color;
	int dimensione_totale, riservato, offset, header_size, larghezza, altezza, compressione, dimensione_immagine;
	int ris_orizzontale,ris_verticale,pallete1,pallete2;
	bool esistePalette;
    
public:
	Header(char*);
	int getDimensioneTot();
	int getLarghezza();
	int getAltezza();
	int getColor();
	int getOffsetIMG();
	void setLarghezza(int);
	void setAltezza(int);
	void setOffset(int);
	void setDimensioneTot(int);
	void setColor(int);
	void stampaInfoHeader();
	void scriviHeader(char*);
	
	
};