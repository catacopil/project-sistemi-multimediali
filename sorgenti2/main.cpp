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

#include "Parser.h"
#include "Header.h"
#include "ImagePNG.h"
#include "BitMapRGB.h"
#include "ImageFilter.h"


using namespace std;

int main(int argc, char *argv[]){
  
    // considero gli argomenti passati da linea di comando
    // La sintassi e' la seguente: elaboraimmagine <fileinput> <comando> <dimensioni_x> <dimensioni_y> <fileoutput> <scala>
  
if(argc==7 || argc==6){
  
    
    double scala=0;
      
    if(argc==7)
      scala=atof(argv[6]);

      
    Parser* parser=new Parser(argv[1],argv[5],argv[2],atoi(argv[4]),atoi(argv[3]),scala);

	if (parser->getInputFormat() == 2){				// IMMAGINE PNG
		ImagePNG* mioPNG = new ImagePNG(parser->getInputFileName());
		
		int larg = mioPNG->getLarghezza();
		int alt = mioPNG->getAltezza();
		unsigned char* bitMapPNG = new unsigned char[larg*alt*3];
		unsigned char* bitMapOut = new unsigned char[larg*alt*3];
		bitMapPNG = mioPNG->getBitMapRGB();
		ImageFilter* img=new ImageFilter(bitMapPNG,alt,larg,parser->getScala());
		if(parser->getFiltro()!=-1){
			bitMapOut = img->eseguiFiltro(parser->getFiltro());
			mioPNG->setBitMapRGB(bitMapOut);
		}
		else
			cout<<"ERRORE! FILTRO NON RICONOSCIUTO"<<endl;		
		
		mioPNG->ridimensiona(parser->getLunghezzaOutput(), parser->getAltezzaOutput());		

		if (parser->getOutputFormat()==1){
			mioPNG->scriviBMPconHeader(parser->getOutputName());
		}
		else if (parser->getOutputFormat()==2)
			mioPNG->scriviPNG(parser->getOutputName());
	}
	else if (parser->getInputFormat() == 1){			// IMMAGINE BMP
		Header* mioHeader = new Header(parser->getInputFileName());
		mioHeader->stampaInfoHeader();

		// creo la bitMap che viene letta direttamente dal file indicato
		
		BitMapRGB* mioBitMap = new BitMapRGB(parser->getInputFileName(), mioHeader->getOffsetIMG(), mioHeader->getLarghezza(), mioHeader->getAltezza());

		ImageFilter* img=new ImageFilter(mioBitMap->getBitMap(),mioHeader->getAltezza(),mioHeader->getLarghezza(),parser->getScala());

		unsigned char*  bitMapFiltrata = new unsigned char[mioHeader->getAltezza()*mioHeader->getLarghezza()*3];
		
		if(parser->getFiltro()!=-1)
		bitMapFiltrata=img->eseguiFiltro(parser->getFiltro());
		else
		cout<<"ERRORE! FILTRO NON RICONOSCIUTO"<<endl;
			
		mioBitMap->setBitMap(bitMapFiltrata);
		
		if(mioHeader->getAltezza()!=parser->getAltezzaOutput() || mioHeader->getLarghezza()!=parser->getLunghezzaOutput()){   
			mioBitMap->ridimensiona(parser->getLunghezzaOutput(),parser->getAltezzaOutput());
			mioHeader->setLarghezza(parser->getLunghezzaOutput());
			mioHeader->setAltezza(parser->getAltezzaOutput());
		}
	
		char* nomeFileOut=parser->getOutputName();
		
		if (parser->getOutputFormat()==2){
			// faccio la conversione e scrivo il PNG
			ImagePNG* mioPNG = new ImagePNG(mioBitMap->getBitMap(), mioHeader->getLarghezza(), mioHeader->getAltezza());
			mioPNG->scriviPNG(parser->getOutputName());
		}
		else{
			mioHeader->scriviHeader(nomeFileOut);			// scrittura header (anche la Palette se c'è)
			mioBitMap->scriviBitMap(nomeFileOut);			// stampo nel file di destinazione la bitMap
		}
	}
	  
	  cout << " -----   FINE  PROGRAMMA  ----- \n";
	  
}else{
  
  cout<<"ATTENZIONE! Sinstassi non corretta. La sintassi e' la seguente: elaboraimmagine <fileinput> <comando> <dimensioni_x> <dimensioni_y> <fileoutput> <scala>"<<endl;
  
  
}
}