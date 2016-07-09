#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ImageFilter.h"



  ImageFilter::ImageFilter(unsigned char* bitMap,int altezza,int lunghezza){
    this->bitMap=bitMap;
    this->altezza=altezza;
    this->larghezza=larghezza;
  }
  
  
  
  unsigned char* ImageFilter::contrasto(double valore){
    
    double fattore=(259*(valore+255))/(255*(259-valore));
    unsigned char pixel;
    
    for(int i=0; i<this->altezza*this->larghezza;i++){
    
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
    
 
    for(int i=0; i<this->altezza*this->larghezza;i++){
    
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
      for(int i=0; i<this->altezza*this->larghezza;i++){
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
    
    for(int i=0; i<this->altezza*this->larghezza;i++){
    
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
    
    for(int i=0; i<this->altezza*this->larghezza;i++){
    
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
  
  
  
  