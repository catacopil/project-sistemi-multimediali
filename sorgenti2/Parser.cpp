#include "Parser.h"
#include <iostream>

using namespace std;


/**Costruttore della classe ImageFilter.
 * 
 * @param input: contiene l'indirizzo dell'immagine in input
 * @param output: contiene l'indirizzo dell'immagine di output
 * @param filtro: contiene il filtro richiesto
 * @param outAltezza: contiene l'altezza richiesta dell'immagine in output
 * @param outLunghezza contiene la lunghezza richiesta dell'immagine in output
 * @param scala: fattore di scala da applicare al filtro.

Il costruttore semplicemente copia nei capi dati della classe i valori ononimi.


**/
Parser::Parser(char* input, char* output, char* filtro, int outAltezza, int outLunghezza,double scale){
 
  string inp(input), out(output), filt(filtro);
  
  this->input=inp;
  this->output=out;
  this->filtro=filt;
  this->outAltezza=outAltezza;
  this->outLunghezza=outLunghezza;
  this->scala=scale;
  
}

  /**
 * Metodo che ritorna un array di caratteri che contiene l'indirizzo dell'immagine data in input
 * nessun parametro
 * 
 *
 **/    
char* Parser::getOutputName(){
   return (char*)output.c_str();
 }
 
  /**
 * Metodo che ritorna un array di caratteri che contiene il nome da dare all'immagine data in output
 * nessun parametro
 * 
 *
 **/     
 char* Parser::getInputFileName(){
   return (char*)input.c_str(); 
 }
 
   /**
 * Metodo che ritorna il filtro da effettuare sull'immagine
 * nessun parametro
 * 
 * Il metodo restiusce un numero corrispondente al filtro da eseguire. Tale filtro sarà utilizzato dal metodo eseguiFiltro(filtro) della classe ImageFilter 
 * 
 * 0 Nessun filtro
 * 1 luminosita
 * 2 gamma
 * 3 contrasto
 * 4 coloreInverso
 * 5 solarise
 * 6 scalaDiGrigi
 * 7 blur
 * 8 gaussianBlur
 * 9 boxBlur
 * 10 sharpen
 * 11 unsharpen
 * 12 edgeDetect
 * 13 sobelHorizontal
 * 14 sobelVertical
 * 
 * -1 errore
 * 
 **/    
 int Parser::getFiltro(){
   
   if(this->filtro=="nessuno")
     return 0;
   if(this->filtro=="luminosita")
     return 1;
   if(this->filtro=="gamma")
     return 2;
   if(this->filtro=="contrasto")
     return 3;
   if(this->filtro=="coloreinverso")
     return 4;
   if(this->filtro=="solarise")
     return 5;
   if(this->filtro=="scaladigrigi")
     return 6;
   if(this->filtro=="blur")
     return 7;
   if(this->filtro=="gaussianblur")
     return 8;
   if(this->filtro=="boxBlur")
     return 9;
   if(this->filtro=="sharpen")
     return 10;
   if(this->filtro=="unsharpen")
     return 11;
   if(this->filtro=="edgedetect")
     return 12;
   if(this->filtro=="sobelhorizontal")
     return 13;
   if(this->filtro=="sobelvertical")
     return 14;
   
   std::cout<<"ERRORE. Il comando inserito non e' stato riconosciuto"<<std::endl;
   return -1;
   
 }
 

 /**
 * Metodo che ritorna il valore dell'altezza da dare all'immagine di output
 * nessun parametro
 * 
 *
 **/     
 int Parser::getAltezzaOutput(){
   return outAltezza;
 }
 
 /**
 * Metodo che ritorna il valore di lunghezza da dare all'immagine di output
 * nessun parametro
 * 
 *
 **/     
 int Parser::getLunghezzaOutput(){
   return outLunghezza;
 }
 
 
 
 /**
 * Metodo che ritorna un numero corrisponde al formato dell'immagine data in input
 * nessun parametro
 * 
 *
 **/     
 
 int Parser::getInputFormat(){
   
   string format=input.substr(input.size()-4,4);
  
   if(format==".bmp")
     return 1;
   if(format==".png")
     return 2;
   
   std::cout<<"ERRORE. Il formato di input non e' stato riconosciuto"<<std::endl;
   return -1;
   
 }
 
 /**
 * Metodo che ritorna un numero corrisponde al formato dell'immagine da dare in output
 * nessun parametro
 * 
 *
 **/      
  int Parser::getOutputFormat(){
   
   string format=output.substr(output.size()-4,4);
  
   if(format==".bmp")
     return 1;
   if(format==".png")
     return 2;
   
   std::cout<<"ERRORE. Il formato di input non e' stato riconosciuto"<<std::endl;
   return -1;
   
 }
 
  /**
 * Metodo che ritorna il valore di scala da applicare al filtro
 * nessun parametro
 * 
 *
 **/     
 double Parser::getScala(){
   return scala;
 }
 
 
 