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
    
    int headerOffset, headerLarg, headerAlt, headerDimIMG;
	int nuovaLarg, nuovaAlt;
    
    char* nomeFileIn = "img/kimi512.bmp";
    //char* nomeFileIn = "kimi512.bmp";
	char* nomeFileOut = "img/output.bmp";
    Header* mioHeader = new Header(nomeFileIn);
	headerLarg = mioHeader->getLarghezza();
	headerAlt = mioHeader->getAltezza();
	mioHeader->stampaInfoHeader();
	
	// creo la bitMap che viene letta direttamente dal file indicato
	BitMapRGB* mioBitMap = new BitMapRGB(nomeFileIn, mioHeader->getOffsetIMG(), mioHeader->getLarghezza(), mioHeader->getAltezza());
	
	// MODIFICO LE DIMENSIONI
	nuovaLarg = 1024; nuovaAlt=1024;
	// TODO: da ricalcolare la dimensione_totale dell'immagine
	mioHeader->setLarghezza(nuovaLarg);
	mioHeader->setAltezza(nuovaAlt);
	mioBitMap->ridimensiona(nuovaLarg,nuovaAlt);
	
	
	mioHeader->scriviHeader(nomeFileOut);			// scrittura header (anche la Palette se c'è)
	mioBitMap->scriviBitMap(nomeFileOut);			// stampo nel file di destinazione la bitMap
	
	
	cout << " -----   FINE  PROGRAMMA  ----- \n";
}