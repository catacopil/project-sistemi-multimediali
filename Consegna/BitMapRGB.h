
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

class BitMapRGB{
	/*
	 * La classe BitMapRGB rappresenta una bitMap RGB a 24 bit di una determinata immagine.
	 * La classe gestisce le operazioni di lettura/scrittura della bitMap su file, inoltre consente la modifica della bitMap (tramite appositi metodi) per l'applicazione dei filtri della classe ImageFilter
	 */
private:
	/**
	*  Campi privati della classe BitMapRGB

	* @param bitMap: contiene la bitMap dell'immagine, i colori sono memorizzati nel formato BGR
	* @param offsetIMG: contiene il valore dell'offset dell'immagine, ovvero la posizione nel file dove inizia la bitMap
	* @param larghezza: contiene la larghezza in pixel dell'immagine
	* @param altezza: contiene l'altezza in pixel dell'immagine
	* @param color: contiene i bit utilizzati per ogni pixel, utilizzeremo lo standard RGB con 24 bit per pixel
	**/ 
	unsigned char* bitMap;
	int offsetIMG, larghezza, altezza;
	int color;
public:
	/**
	 * Metodi pubblici della classe BitMapRGB
	 * BitMapRGB: costruttore della classe 
	 * getBitMap: ritorna la bitMap dell'immagine 
	 * setBitMap: imposta la bitMap con una nuova passata come argomento
	 * scriviBitMap: scrive la bitMap in un determinato file .bmp
	 * ridimensiona: ridimensiona la bitMap facendola corrispondere ad un'immagine con le nuove dimensioni
	 **/
	
	BitMapRGB(char*, int, int, int);
	unsigned char* getBitMap();
	void setBitMap(unsigned char*);
	void scriviBitMap(char*);
	void ridimensiona(int, int);
};