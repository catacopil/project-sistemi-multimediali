#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ImageFilter.h"



  ImageFilter::ImageFilter(unsigned char* bitMap,int altezza,int lunghezza){
    this->bitMap=bitMap;
    this->altezza=altezza;
    this->lunghezza=lunghezza;
  }
  
  
  
  unsigned char* ImageFilter::contrasto(double valore){
    
    double fattore=(259*(valore+255))/(255*(259-valore));
    unsigned char pixel;
    
    for(int i=0; i<this->altezza*this->lunghezza;i++){
    
      pixel=bitMap[i];
      
      pixel=(fattore*(pixel-128)+128);
      
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=pixel;
      
    }
    
    return bitMap;
    
  }
  
  unsigned char* ImageFilter::luminosita(double valore){
    
    unsigned char pixel;
    
 
    for(int i=0; i<this->altezza*this->lunghezza;i++){
    
      pixel=bitMap[i];
      pixel=(valore*pixel)/255;
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=pixel;
      
    }
    
    return bitMap;
    
  }
  
  
  unsigned char* ImageFilter::gamma(double valore){
    
    unsigned char pixel;
    
    if(valore!=0){
      double gamma= 1/valore;
      for(int i=0; i<this->altezza*this->lunghezza;i++){
	pixel=bitMap[i];
	pixel=pow(pixel/255.0f, gamma);
	pixel=pixel*255.0f;
	if(pixel>255)
	  pixel=255;
	if(pixel<0)
	  pixel=0;
	bitMap[i]=pixel;
      
    }
    
    return bitMap;
      
    }
    else
    {
      std::cout<<"Errore! Inserire un valore diverso da 0"<<std::endl;
      return bitMap;
    }
  }
  
  unsigned char* ImageFilter::coloreInverso(){
    
    unsigned char pixel;
    
    for(int i=0; i<this->altezza*this->lunghezza;i++){
    
      pixel=bitMap[i];
      pixel=255-pixel;
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=pixel;
      
    }
    
    return bitMap;
    
  }
  
  unsigned char* ImageFilter::solarise(double soglia){
    
    unsigned char pixel;
    
    for(int i=0; i<this->altezza*this->lunghezza;i++){
    
      pixel=bitMap[i];
      if(pixel<=soglia)
      {
      pixel=255-pixel;
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=pixel;
      }
    }
    
    return bitMap;
    
  }
  
  
  int ImageFilter::apply3x3Convolution(double kernel[3][3], int i, int j){
    
   int a=-1,b=-1,c=1,d=1;
   
   /*
    * Inizio i check sulla posizione attuale. 
    * Se sono sul bordo non posso applicare tutta la matrice di Convoluzione
    * Si noti che un BMP inizia da basso a sinistra e finisce in alto a destra
    */
   
   //Controllo se sono sulla riga in basso a sinistra
  if(i==0)
    a=0;
   //Controllo se mi trovo all'inizio di una riga
  if(j==0)
    b=0;
  //Controllo se mi trovo sull'ultima riga in alto
  if(i==this->altezza-1)
    c=0;
  //Controllo se sono alla fine di una riga
  if(j==this->lunghezza-1)
    d=0;
  
  int ris=0,val=0;
  
  for(a;a<c;a++){
   for(b;b<d;b++){
     val=bitMap[((i+a)*this->lunghezza)+(j+b)];
     ris += val*kernel[a][b];
   }    
  }
  
  return ris;
    
    
  }
 
 
 
 unsigned char* ImageFilter::blur(){

   double kernel[3][3] ={
     {1,1,1},
     {1,1,1},
     {1,1,1}
 };
 
    for(int i = 0;i < this->altezza;i++) {
	for(int j = 0;j < this->lunghezza;j++) {
	  bitMap[i*this->lunghezza+j] = apply3x3Convolution(kernel,i,j);
	}
    }
    
    return bitMap;
   
 }
 
 
 
 
 
 
 

  
  
  
  
  