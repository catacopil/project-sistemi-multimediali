
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "lodepng.h"



/**
 * 
 * La classe ImagePNG si occupa di gestire le immagini PNG, gestisce la lettura/scrittura dell'intero file (Header e Bitmap)
 * La classe utilizza la libreria lodepng.h (libreria all-in-one file)
 * 
 **/

class ImagePNG{

private:
	unsigned int larghezza, altezza;
	std::vector<unsigned char> png;			// tutti i byte del png
	std::vector<unsigned char> pixel; 		// tutti i byte dei pixel
	unsigned char* bitMapRGB; 				// contiene i valori RGB
	unsigned char* bitMapAlpha;				// contiene i valori Alpha
	int dimBitMap;
	lodepng::State state;
	
public:
	ImagePNG(char*);						// inizializza l'oggetto con un PNG da un nomeFile passato per argomento
	ImagePNG(unsigned char*, int, int);				// inizializza l'oggetto con una bitMap BMP già pronta e con le dimensioni dell'immagine
	int getLarghezza();
	int getAltezza();
	void ridimensiona(int, int);
	unsigned char* getBitMapRGB();				// ritorna la bitMap contenente i valori RGB dei pixel
	unsigned char* getBitMapAlpha();			// ritorna la bitMap contenente i valori Alpha dei pixel
	void setBitMapRGB(unsigned char*);
	void setBitMapAlpha(unsigned char*);
	
	void scriviPNG(char*);					// scrive il PNG in un determinato file di output
	void scriviBMPconHeader(char*);
};