
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define DIM_HEADER_INIZIALE 54
#define DIM_PALETTE 1024

class Header{
	/**
	* 
	* La classe Header si occupa di gestire il componente Header delle immagini BMP, gestisce la lettura/scrittura nel file dell'intero Header e la modifica di alcuni valori contenuti in esso
	* 
	**/
private:
	/**
	*  Campi privati della classe Header

	* @param headerContent: contiene tutti i byte letti per lo Header
	* @param paletteContent: contiene tutti i byte della Palette
	* @param firma: contiene la firma per il formato del file, di solito 'BM'
	* @param piani: numero di piani
	* @param color: numero di bit per ogni pixel detto anche profondita di colore (bit per pixel)
	* @param dimensione_totale: dimensione totale del file (compresi header, Palette e bitMap)
	* @param riservato: campo dello header riservato
	* @param offset: byte dove inizia la bitMap nel file BMP
	* @param header_size: dimensione (in byte) della parte header che descrive le caratteristiche dell'immagine (titolo dell'immagine)
	* @param larghezza: larghezza dell'immagine in pixel
	* @param altezza: altezza dell'immagine in pixel
	* @param compressione: metodo di compressione
	* @param dimensione_immagine: dimensione della bitMap in byte
	* @param ris_orizzontale: risoluzione orizzontale dell'immagine, espressa in pixel per metro
	* @param ris_verticale: risoluzione verticale dell'immagine, espressa in pixel per metro
	* @param pallete1: numero di colori della Palette
	* @param pallete2: numero di colori della Palette utilizzati
	* @param esistePalette: indica se il file dell'immagine contiene anche la Palette o meno
	**/
	
	unsigned char headerContent[DIM_HEADER_INIZIALE];       // contiene tutti i byte dell'Header (ad eccezione della Palette)
	unsigned char paletteContent[DIM_PALETTE];				// contiene tutti i byte della Palette
	char firma[2],piani[2];
	short color;
	int dimensione_totale, riservato, offset, header_size, larghezza, altezza, compressione, dimensione_immagine;
	int ris_orizzontale,ris_verticale,pallete1,pallete2;
	bool esistePalette;
    
public:
	/**
	 * Metodi pubblici della classe Header
	 * 
	 * Header: costruttore della classe.
	 * getDimensioneTot: restituisce la dimensione totale del file
	 * getLarghezza: restituisce la larghezza dell'immagine in pixel
	 * getAltezza: restituisce l'altezza dell'immagine in pixel
	 * getColor: restituisce la profondita di colore (bit utilizzati per ogni pixel)
	 * getOffsetIMG: restituisce l'offset dell'immagine
	 * setLarghezza: imposta un nuovo valore per la larghezza dell'immagine
	 * setAltezza: imposta un nuovo valore per l'altezza dell'immagine
	 * setOffset: imposta un nuovo valore di offset
	 * setColor: imposta un nuovo valore per color
	 * stampaInfoHeader: stampa le informazioni lette nello header
	 * scriviHeader: scrive lo header su un determinato file di output
	 **/
	Header(char*);
	int getDimensioneTot();
	int getLarghezza();
	int getAltezza();
	int getColor();
	int getOffsetIMG();
	void setLarghezza(int);
	void setAltezza(int);
	void setOffset(int);
	void setColor(int);
	void stampaInfoHeader();
	void scriviHeader(char*);
};