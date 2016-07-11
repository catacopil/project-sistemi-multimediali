#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iterator>
#include "ImageFilter.h"



  ImageFilter::ImageFilter(unsigned char* bitMap,int altezza,int lunghezza){
    this->bitMap=bitMap;
    this->altezza=altezza;
    this->lunghezza=lunghezza;
  }
  
  
  
  unsigned char* ImageFilter::contrasto(double valore){
    
    double fattore=(259*(valore+255))/(255*(259-valore));
    double pixel;
    
    

    for(int i=0; i<this->altezza*this->lunghezza*3;i++){
    
      pixel=(int)bitMap[i];
      
      pixel=(fattore*(pixel-128)+128);
      
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=(unsigned char)pixel;
      
    }
    
    return bitMap;
    
  }
  
  unsigned char* ImageFilter::luminosita(double valore){
    
    double pixel;
    
 
    for(int i=0; i<this->altezza*this->lunghezza*3;i++){
    
      pixel=(int)bitMap[i];
      pixel+=(valore*pixel)/255.0;
     // pixel=pixel+valore;
      if(pixel>=255)
	pixel=255;
      if(pixel<=0)
	pixel=0;
      
      bitMap[i]=(unsigned char)pixel;
      
    }
    
    return bitMap;
    
  }
  
  
  unsigned char* ImageFilter::gamma(double valore){
    
   double pixel;
    
    if(valore!=0){
      double gamma=1/valore;
      for(int i=0; i<this->altezza*this->lunghezza*3;i++){
	pixel=(double)bitMap[i];;
	pixel=pow(pixel/255.0, gamma);
	pixel*=255.0;
	if(pixel>255)
	  pixel=255;
	if(pixel<0)
	  pixel=0;
	
	bitMap[i]=(unsigned char)pixel;
      
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
    
    int pixel;
    
    for(int i=0; i<this->altezza*this->lunghezza*3;i++){
    
      pixel=(int)bitMap[i];
      pixel=255-pixel;
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=(unsigned char)pixel;
      
    }
    
    return bitMap;
    
  }
  
  unsigned char* ImageFilter::solarise(double soglia){
    
    int pixel;
    
    for(int i=0; i<this->altezza*this->lunghezza*3;i++){
    
      pixel=(int)bitMap[i];
      if(pixel<=soglia)
      {
      pixel=255-pixel;
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      bitMap[i]=(unsigned char)pixel;
      }
    }
    
    return bitMap;
    
  }
  
  
  unsigned char ImageFilter::returnPixel(unsigned char* bit,int i,int j){
    
    if(i>=0 && i<= this->altezza && j>=0 && j<= this->lunghezza*3)
      return bit[i*this->lunghezza*3+j];
    if(i<0 && j<0)
      return bit[0];
    if(i<0)
      return bit[j];
    if(j<0)
      return bit[i*this->lunghezza*3];
    if(j>this->lunghezza*3)
      return bit[i*this->lunghezza*3+this->lunghezza*3];
    if(i>this->altezza && j<0)
      return bit[i*this->lunghezza*3];
    if(i>this->altezza)
      return bit[i*this->lunghezza*3+j];
    
    

  }
  
  int getKernelPosition(int x){
    if(x==-1)
      return 0;
    else
      if(x==0)
	return 1;
      else
	return 2;

  }
  
  
  int ImageFilter::apply3x3Convolution(unsigned char* copia,float kernel[3][3], int i, int j,int sum){
    
   int a=-1,b=-1,c=1,d=1;
   
   /*
    * Inizio i check sulla posizione attuale. 
    * Se sono sul bordo non posso applicare tutta la matrice di Convoluzione
    * Si noti che un BMP inizia da basso a sinistra e finisce in alto a destra
    */
   
  float val, ris=0;
  
  for(a;a<=c;a++){
   for(int z=b;z<=d;z++){   
     val=returnPixel(copia,i+a,j+(z*3));
     ris += val*kernel[getKernelPosition(a)][getKernelPosition(z)];
   }    
  }
  ris/=sum;
  if(ris < 0) 
    ris = 0;
  if(ris > 255) 
    ris = 255;

  return (unsigned char)ris;
    
    
  }
 
 
 
 
 unsigned char* ImageFilter::blur(){

   float kernel[3][3] ={
     {1,1,1},
     {1,1,1},
     {1,1,1}
   };    
 
 int div=9;

 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }
 

 
  unsigned char* ImageFilter::gaussianBlur(){

 float kernel[3][3] ={{0.045, 0.122, 0.045},{ 0.122, 
  0.332, 0.122},{ 0.045, 0.122, 0.045}};
 
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }
 


  
  unsigned char* ImageFilter::sharpen(){

 float kernel[3][3] ={{-1, -1, -1},{ -1, 16, -1},{ -1, -1, -1}};
 int div=8;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }

  unsigned char* ImageFilter::unsharpen(){

 float kernel[3][3] ={{-1, -1, -1},{ -1, 9, -1},{ -1, -1, -1}};
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }
 
 
  unsigned char* ImageFilter::edgeDetect(){

 float kernel[3][3] ={{-0.125, -0.125, -0.125},{ -0.125, 
  1, -0.125},{ -0.125, -0.125, -0.125}};
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }

  
  unsigned char* ImageFilter::boxBlur(){

 float kernel[3][3] ={{0.111, 0.111, 0.111},{ 0.111, 
  0.111, 0.111},{ 0.111, 0.111, 0.111}};
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }  
 
 

  unsigned char* ImageFilter::sobelHorizontal(){

 float kernel[3][3] ={{1, 2, 1},{ 0, 0, 0}, {-1, -2, -1 }};
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }  
 
  unsigned char* ImageFilter::sobelVertical(){

 float kernel[3][3] ={{1, 0, -1},{ 2, 0, -2},{ 1, 0, -1 }};
 int div=1;
 
 int arr_size = this->altezza*this->lunghezza*3;
 unsigned char copia[this->altezza*this->lunghezza*3];
 
 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
    for(int i = 1;i < this->altezza-1;i++) {
	for(int j = 1;j < this->lunghezza*3-1;j++) {
	  bitMap[i*this->lunghezza*3+j] =apply3x3Convolution(copia,kernel,i,j,div);
	}
    }
    
    return bitMap;
   
 }  
  

  
  
  