
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

class BitMapRGB{
	/*
	 * La classe BitMapRGB rappresenta una bitMap RGB a 24 bit. Il costruttore riceve in input il nome del file da dove prendere la bitMap, l'offset e le dimensioni in px
	 * Il metodo getBitMap restituisce un array di unsigned char contenente la bitMap
	 */
private:
	unsigned char* bitMap;
	int offsetIMG, larghezza, altezza;
	int color;			// 24 bit per ogni pixel
public:
	BitMapRGB(char*, int, int, int);
	unsigned char* getBitMap();
	void setBitMap(unsigned char*);
	void scriviBitMap(char*);
	void ridimensiona(int, int);
};