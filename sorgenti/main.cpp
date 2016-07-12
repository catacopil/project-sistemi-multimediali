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
#include "Parser.cpp"
#include "Header.cpp"
#include "BitMapRGB.cpp"
#include "ImageFilter.cpp"


using namespace std;

int main(int argc, char *argv[]){
  
    // considero gli argomenti passati da linea di comando
    // La sintassi e' la seguente: elaboraimmagine <fileinput> <comando> <dimensioni_x> <dimensioni_y> <fileoutput> <scala>
  
if(argc==7 || argc==6){
  
    
    double scala=0;
      
    if(argc==7)
      scala=atof(argv[6]);

      
    Parser* parser=new Parser(argv[1],argv[5],argv[2],atoi(argv[3]),atoi(argv[4]),scala);

	  
    Header* mioHeader = new Header(parser->getInputFileName());
    mioHeader->stampaInfoHeader();

      // creo la bitMap che viene letta direttamente dal file indicato
      
      
      //TODO aggiungere caso PNG
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
    
    
    mioHeader->scriviHeader(nomeFileOut);			// scrittura header (anche la Palette se c'è)
  //TODO aggiungere caso PNG	
    mioBitMap->scriviBitMap(nomeFileOut);			// stampo nel file di destinazione la bitMap
	  
	  
	  cout << " -----   FINE  PROGRAMMA  ----- \n";
	  
}else{
  
  cout<<"ATTENZIONE! Sinstassi non corretta. La sintassi e' la seguente: elaboraimmagine <fileinput> <comando> <dimensioni_x> <dimensioni_y> <fileoutput> <scala>"<<endl;
  
  
}
}