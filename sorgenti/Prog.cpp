/*
 * PROGETTO SISTEMI MULTIMEDIALI - a.a. 2015/1016
 * 
 * File programma principale, contiene il main
 * 
 * Autori: Catalin Copil, Umberto Martinati
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Header.cpp"
#include "BitMapRGB.cpp"
#include "ImageFilter.cpp"

using namespace std;

int main(int argc, char *argv[]){
    // considero gli argomenti passati da linea di comando
    
    short headerColor;
    int headerOffset, headerLarg, headerAlt, headerDimIMG;
    
    char* nomeFileIn = "img/kimi512.bmp";
    //char* nomeFileIn = "kimi512.bmp";
	char* nomeFileOut = "img/output.bmp";
    Header* mioHeader = new Header(nomeFileIn);
	mioHeader->stampaInfoHeader();

	// creo la bitMap che viene letta direttamente dal file indicato
	BitMapRGB* mioBitMap = new BitMapRGB(nomeFileIn, mioHeader->getOffsetIMG(), mioHeader->getLarghezza(), mioHeader->getAltezza());


	
	ImageFilter* img=new ImageFilter(mioBitMap->getBitMap(),mioHeader->getAltezza(),mioHeader->getLarghezza(),mioHeader->getColor());
	
	unsigned char*  bm = new unsigned char[mioHeader->getAltezza()*mioHeader->getLarghezza()*3];
	bm=img->sobelVertical();
	
	mioBitMap->setBitMap(bm);
	
//	mioHeader->setColor(8);
	
	
	
	mioHeader->scriviHeader(nomeFileOut);			// scrittura header (anche la Palette se c'è)
	
	mioBitMap->scriviBitMap(nomeFileOut);			// stampo nel file di destinazione la bitMap
	
	
	cout << " -----   FINE  PROGRAMMA  ----- \n";
}