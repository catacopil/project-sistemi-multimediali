
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "lodepng.h"

/**
 * 
 * La classe ImagePNG si occupa di gestire le immagini PNG, gestisce la lettura/scrittura dell'intero file (Header e Bitmap)
 * La classe utilizza la libreria lodepng.h (libreria compresa in un singolo file)
 * 
 **/

class ImagePNG{

private:
	/**
	*  Campi privati della classe ImagePNG

	* @param larghezza: contiene la larghezza in pixel dell'immagine, può essere modificato dal metodo ridimensiona()
	* @param altezza: contiene l'altezza in pixel dell'immagine, può essere modificato dal metodo ridimensiona()
	* @param dimBitMap: indica la dimensione in pixel dell'immagine, corrisponde a larghezza*altezza
	* @param png: variabile utilizzata per immagazzinare tutti i byte dell'immagine PNG, compreso lo header
	* @param pixel: variabile utilizzata per contenere la bitMap RGBA ottenuta dalla lettura dell'immagine (raw pixel)
	* @param bitMapRGB: contiene la bitMap con i soli valori RGB dei pixel, molto utile perché permette di applicare i filtri costruiti tramite la classe ImageFilter
	* @param bitMapAlpha: contiene i valori Alpha dei pixel
	* @param state: variabile utilizzata dalla libreria per impostazioni inerenti la codifica/decodifica del file PNG
	*
	**/ 
	unsigned int larghezza, altezza;
	std::vector<unsigned char> png;
	std::vector<unsigned char> pixel;
	unsigned char* bitMapRGB;
	unsigned char* bitMapAlpha;
	int dimBitMap;
	lodepng::State state;
	
public:
	/**
	 * Metodi pubblici della classe ImagePNG
	 * 
	 * ImagePNG(char*): costruttore della classe che costruisce il nuovo oggetto leggendo un file il cui nome viene passato per argomento
	 * ImagePNG(unsigned char*, int, int): costruttore della classe che costruisce il nuovo oggetto con una bitMap estratta da un file BMP e con le dimensioni dell'immagine
	 * getLarghezza: restituisce la larghezza dell'immagine
	 * getAltezza: restituisce l'altezza dell'immagine
	 * ridimensiona: ridimensiona l'immagine con le nuove dimensioni indicate, aggiorna la variabile pixel
	 * getBitMapRGB: restituisce la bitMap in formato RGB dell'immagine 
	 * getBitMapAlpha: restituisce i valori Alpha dei pixel dell'immagine
	 * setBitMapRGB: imposta la bitMap (@param bitMapRGB) in formato RGB con una nuova bitMap
	 * scriviPNG: scrive l'immagine PNG nel file con il nome indicato dall'argomento passato
	 * scriviBMPconHeader: scrive l'immagine in formato BMP in un file con il nome indicato dall'argomento passato
	 **/
	
	ImagePNG(char*);
	ImagePNG(unsigned char*, int, int);
	int getLarghezza();
	int getAltezza();
	void ridimensiona(int, int);
	unsigned char* getBitMapRGB();
	unsigned char* getBitMapAlpha();
	void setBitMapRGB(unsigned char*);
	void scriviPNG(char*);					// scrive il PNG in un determinato file di output
	void scriviBMPconHeader(char*);
};