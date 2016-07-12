#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iterator>
#include "ImageFilter.h"



/**Costruttore della classe ImageFilter.
Il costruttore di ImageFilter ha i seguenti paramentri:
@param bitMap: contiene la bitMap dell'immagine (RGB)
@param altezza: contiene l'altezza dell'immagine
@param lunghezza: contiene la lunghezza dell'immagine
@param scale: parametro opzionale di scala per il filtro

Il costruttore semplicemente copia nei capi dati della classe i valori ononimi.


**/
  ImageFilter::ImageFilter(unsigned char* bitMap,int altezza,int lunghezza,double scale){
    this->bitMap=bitMap;
    this->altezza=altezza;
    this->lunghezza=lunghezza;
    this->scala=scale;
  }
  
  
/**
 *Filtro di contrasto
 * @param valore: valore richiesto di aumento
 * 
 * Questo filtro aumenta il contrasto dell'immagine per un fattore uguale a (259*(valore_richiesto+255))/(255*(259-valore_richiesto))
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/  
  unsigned char* ImageFilter::contrasto(double valore){
    
    if(valore!=0){    
    //calcolo il fattore
    double fattore=(259*(valore+255))/(255*(259-valore));
    double pixel;
    int size=this->altezza*this->lunghezza*3;
 
    for(int i=0; i<size;i++){
      pixel=(int)bitMap[i];
      
      //eseguo la modifica
      pixel=(fattore*(pixel-128)+128);
      
      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      //aggiorno la bitMap
      bitMap[i]=(unsigned char)pixel;
      
    }
    
    cout<<"Eseguito filtro contrasto'"<<endl;
    return bitMap;
      
    }
    else
    {
      std::cout<<"Errore! E' stato inserito un valore di scala non valido"<<std::endl;
      return bitMap;
    }
  }
  
 /**
 *Filtro di luminosita
 * @param valore: valore richiesto di aumento
 * 
 * Questo filtro aumenta la luminosita dell'immagine per un fattore uguale a (valore_richiesto*valore_colore)/255
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/  
  
  unsigned char* ImageFilter::luminosita(double valore){
    
    if(valore!=0){  
    
    double pixel;
    int size=this->altezza*this->lunghezza*3;
 
    for(int i=0; i<size;i++){
    
      pixel=(int)bitMap[i];
      
      //eseguo la modifica      
      pixel+=(valore*pixel)/255.0;

      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]
      if(pixel>=255)
	pixel=255;
      if(pixel<=0)
	pixel=0;
      
      //aggiorno la bitMap
      bitMap[i]=(unsigned char)pixel;
      
    }
    
    cout<<"Eseguito filtro luminosita'"<<endl;    
    
    return bitMap;
      
    }
    else
    {
      std::cout<<"Errore! E' stato inserito un valore di scala non valido"<<std::endl;
      return bitMap;
    }
  }
 
 /**
 *Filtro di gamma
 * @param valore: valore richiesto di modifica
 * 
 * Questo filtro esegue un filtro gamma sull'immagine. Si calcola il valore di gamma che è uguale a 1/valore_richiesto. 
 * Successivamente si esegue (valore_colore/255)^gamma e si moltiplica per 255 il risultato
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/   
  
  unsigned char* ImageFilter::gamma(double valore){
    
    if(valore!=0){
    
    double pixel, gamma=1/valore;
      
    int size=this->altezza*this->lunghezza*3;
 
    for(int i=0; i<size;i++){
	pixel=(double)bitMap[i];
	
	//eseguo la modifica
	pixel=pow(pixel/255.0, gamma);
	pixel*=255.0;

      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]
	if(pixel>255)
	  pixel=255;
	if(pixel<0)
	  pixel=0;
	
      //aggiorno la bitMap
      bitMap[i]=(unsigned char)pixel;
      
    }

    cout<<"Eseguito filtro gamma'"<<endl;    
    
    return bitMap;
      
    }
    else
    {
      std::cout<<"Errore! E' stato inserito un valore di scala non valido"<<std::endl;
      return bitMap;
    }
  }
  
  /**
 *Filtro di inversione del colore
 * nessun parametro
 * 
 * Questo filtro esegue un filtro che inverte i colori dell'immagine. Per ogni colore, per ogni pixel, si calcola il valor inverso (255-valore_colore)
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/    
  
  unsigned char* ImageFilter::coloreInverso(){
    
    int pixel;
    int size=this->altezza*this->lunghezza*3;
 
    for(int i=0; i<size;i++){
      pixel=(int)bitMap[i];
      
      //eseguo la modifica
      pixel=255-pixel;

      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      //aggiorno la bitMap
      bitMap[i]=(unsigned char)pixel;
      
    }
 
    cout<<"Eseguito filtro colore inverso'"<<endl;
    
    return bitMap;
    
  }

  /**
 *Filtro solarise
 * @param soglia: soglia sotto cui eseguire il filtro
 * 
 * Questo filtro esegue un filtro che inverte i colori dell'immagine se essi si trovano sotto una certa soglia.
 * Per ogni colore, per ogni pixel, se il valore del colore e' minore della soglia si calcola il valor inverso (255-valore_colore)
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/
  
  unsigned char* ImageFilter::solarise(double soglia){
    
     
    if(soglia!=0){   
    int pixel, size=this->altezza*this->lunghezza*3;
 
    for(int i=0; i<size;i++){
    
      pixel=(int)bitMap[i];
      if(pixel<=soglia){
	
      //eseguo la modifica	
      pixel=255-pixel;

      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;
      
      //aggiorno la bitMap
      bitMap[i]=(unsigned char)pixel;
      }
    }
 
    cout<<"Eseguito filtro solarise'"<<endl;
    
    return bitMap;
      
    }
    else
    {
      std::cout<<"Errore! E' stato inserito un valore di scala non valido"<<std::endl;
      return bitMap;
    }
  }
  
  /**
 *Filtro che esegue la trasformazione da un immagine RGB in una immagine in scala di grigi.
 * nessun parametro
 * 
 * Questo filtro esegue la trasformazione di un immagine RGB in una immagine in scala di grigi.
 * L'immagine risultate continuerà ad essere una immagine RGB
 * Il filtro è di tipo Puntuale. Viene effettuato su ogni pixel dell'immagine effettuando la modifica sui tre valori R G B del pixel.
 *
 **/
  
  unsigned char* ImageFilter::scalaDiGrigi(){
    
    double pixel;
    int size=size=this->altezza*this->lunghezza*3;
    int r,g,b;
    
    for(int i=0; i<size;i=i+3){
    
      //eseguo la modifica
      b=(int)bitMap[i];
      g=(int)bitMap[i+1];
      r=(int)bitMap[i+2];
      //calcolo il nuovo valore in modo pesato (per ottenere una scala di grigi che tenga conto della risposta diversa dei colori)
      pixel=0.299*r+0.587*g+0.114*b;


      //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]      
      if(pixel>255)
	pixel=255;
      if(pixel<0)
	pixel=0;

      //aggiorno la bitMap      
      bitMap[i]=(unsigned char)pixel;
      bitMap[i+1]=(unsigned char)pixel;
      bitMap[i+2]=(unsigned char)pixel;
      }

    cout<<"Eseguito filtro scala di grigi'"<<endl;    
    
    return bitMap;
    
  }  
  
  /**
 * Metodo che ritorna il pixel in posizione i,j della bitMap passata in input. Il metodo implementa l'edge handling Extend.
 * @param bit: bitMap da cui estrarre il valore
 * @param i: riga
 * @param j: colonna
 * 
 * Questo metodo è di supporto al metodo applyRGBConvolution. Estrae il valore i,j della bitMap dell'immagine (RGB)
 * Il metodo gestisce l'edge handling. Questo metodo implementa la strategia Extend. cioè la copia del valore dell'ultimo pixel
 * 
 * NB: Nella versione attualmente implementata si utilizza la strategia CROP (si ignora il bordo).
 * Se si vuole utilizzare è necessario abilitare l'utilizzo del bordo nei vari filtri di convoluzione:
 *   for(int i = 1;i < this->altezza;i++) {
 *	for(int j = 1;j < this->lunghezza*3;j++) {
 *
 **/  
  unsigned char ImageFilter::returnRGBPixel(unsigned char* bit,int i,int j){
    
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
  
/**
 * Metodo che ritorna gli indici del kernel associati con la posizione data in input
 * @param i: posizione
 * 
 * Questo metodo è di supporto al metodo applyRGBConvolution. Questa funzione e' utilizzata in quanto
 * gli indici utilizzati per estrarre i valori dei colori RGB vanno da -1 a 1 invece che da 0 a 3.
 * 
 */
  int getKernelPosition(int x){
    if(x==-1)
      return 0;
    else
      if(x==0)
	return 1;
      else
	return 2;

  }
  
 /**
 * Metodo che esegue una convoluzione di matrice kernel (data in input) nella posizione i,j della bitMap (data in input)
 * @param copia: copia della bitMap dell'immagine. Necessaria per estrarre i valori originali e non quelli gia' modificati
 * @param kernel: matrice di convoluzione
 * @param i: riga
 * @param j: colonna
 * @param div: divisore del kernel (uguale alla somma degli elementi del kernel)
 * 
 * Questo metodo applica la convoluzione di matrice kernel sul valore del colore in posizione i,j della bitMap data in input.
 * Si estrae il valore, si applica la matrice di convoluzione e infine si divide il risultato con il divisore dato in input.
 * 
 */ 
  int ImageFilter::applyRGBConvolution(unsigned char* copia,float kernel[3][3], int i, int j,int div){
    
  //Indici della posizione del colore dei pixel e della matrice kernel  
  int a=-1,b=-1,c=1,d=1; 
  float val, ris=0;
  
  for(a;a<=c;a++){
   for(int z=b;z<=d;z++){   
     //estraggo il colore del pixel in esame
     val=returnRGBPixel(copia,i+a,j+(z*3));	//z*3 mi permette di considerare esattamente lo stesso colore del pixel richiesto
     //moltipico per la posizione corrispondente della matrice di covoluzione e sommo il risultato
     ris += val*kernel[getKernelPosition(a)][getKernelPosition(z)];
   }    
  }
  
  //divido per il divisore
  ris/=div;
  
  //Controllo che la modifica non abbia fatto uscire il valore dal bound[0,255]  
  if(ris < 0) 
    ris = 0;
  if(ris > 255) 
    ris = 255;

  return (unsigned char)ris;
    
    
  }
  
 /**
 *Filtro di convoluzione blur
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione blur base.
 * 
 *
 **/
   
 unsigned char* ImageFilter::blur(){

   //matrice di convoluzione blur semplice
   float kernel[3][3] ={
     {1,1,1},
     {1,1,1},
     {1,1,1}
   };    
 
 //Divisore uguale a 9
 int div=9;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
 
    cout<<"Eseguito filtro di convoluzione blur'"<<endl; 
 
 return bitMap;
   
 }
 

 /**
 *Filtro di convoluzione blur gaussiano
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione blur gaussiano.
 * 
 *
 **/ 
  unsigned char* ImageFilter::gaussianBlur(){

  //matrice di convoluzione di blur gaussiano  
 float kernel[3][3] ={{0.045, 0.122, 0.045},{ 0.122, 
  0.332, 0.122},{ 0.045, 0.122, 0.045}};
  
 //Divisore uguale a 1
 int div=1;

 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
 cout<<"Eseguito filtro di convoluzione gaussian blur'"<<endl; 
    
 return bitMap;
   
 }
 


 /**
 *Filtro di convoluzione sharpening
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione di sharpening
 * 
 *
 **/  
  unsigned char* ImageFilter::sharpen(){

 //matrice di convoluzione di convoluzione di sharpening
 float kernel[3][3] ={{-1, -1, -1},{ -1, 16, -1},{ -1, -1, -1}};
 
 //Divisore uguale a 8
 int div=8;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
 cout<<"Eseguito filtro di convoluzione sharpen'"<<endl;  
 
 return bitMap;
   
 }
 
 /**
 *Filtro di convoluzione di unsharpening (contrasto)
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione unsharpening (contrasto)
 * 
 *
 **/
  unsigned char* ImageFilter::unsharpen(){

 //Matrice di convoluzione di unsharpening   
 float kernel[3][3] ={{-1, -1, -1},{ -1, 9, -1},{ -1, -1, -1}};
 
 //Divisore uguale a 1
 int div=1;

 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
 cout<<"Eseguito filtro di convoluzione unsharpen'"<<endl; 
    
 return bitMap;
   
 }
 
 /**
 *Filtro di convoluzione di edge detect
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione edge detect che esegue l'individuazione dei bordi
 * 
 *
 **/ 
  unsigned char* ImageFilter::edgeDetect(){
    
 //Matrice di convoluzione di edge detect
 float kernel[3][3] ={{-0.125, -0.125, -0.125},{ -0.125, 
  1, -0.125},{ -0.125, -0.125, -0.125}};
  
 //Divisore uguale a 1
 int div=1;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }

 cout<<"Eseguito filtro di convoluzione edge detect'"<<endl;   
  
 return bitMap;
   
 }
 /**
 *Filtro di convoluzione box blur
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione box blur.
 * 
 *
 **/  
  unsigned char* ImageFilter::boxBlur(){

 //matrice di convoluzione di box blur   
 float kernel[3][3] ={{0.111, 0.111, 0.111},{ 0.111, 
  0.111, 0.111},{ 0.111, 0.111, 0.111}};
  
 //Divisore uguale a 1
 int div=1;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
 cout<<"Eseguito filtro di convoluzione box blur'"<<endl; 
    
 return bitMap;
   
 }
 
 /**
 *Filtro di convoluzione sobel orizzontale
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione sobel di tipo orizzontale
 * 
 *
 **/
  unsigned char* ImageFilter::sobelHorizontal(){
    
 //matrice di convoluzione sobel orizzontale  
 float kernel[3][3] ={{1, 2, 1},{ 0, 0, 0}, {-1, -2, -1 }};
 
 //Divisore uguale a 1
 int div=1;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }

 cout<<"Eseguito filtro di convoluzione sobel orizzontale'"<<endl;     
    
 return bitMap;
   
 }
 
 /**
 *Filtro di convoluzione sobel verticale
 * nessun parametro
 * 
 * Questo filtro esegue un filtro di convoluzione sobel di tipo verticale
 * 
 *
 **/ 
  unsigned char* ImageFilter::sobelVertical(){    
    
 //matrice di convoluzione sobel verticale  
 float kernel[3][3] ={{1, 0, -1},{ 2, 0, -2},{ 1, 0, -1 }};
 
  //Divisore uguale a 1
 int div=1;
 
 int arr_size=this->altezza*this->lunghezza*3;
 unsigned char copia[arr_size];
 
 /**Eseguo una copia profonda della bitMap. Questo serve a poter eseguire correttamente il filtro di convoluzione.
  * Se lo eseguissi direttamente con la bitMap il filtro estraerebbe valori già modificati
  * 
  */

 for(int i=0; i<arr_size;i++)
   copia[i]=bitMap[i];
 
       for(int i = 1;i < this->altezza-3;i++) {
	for(int j = 1;j < this->lunghezza*3-3;j++) {
	  //eseguo la convoluzione
	  bitMap[i*this->lunghezza*3+j] =applyRGBConvolution(copia,kernel,i,j,div);
	}
    }
  cout<<"Eseguito filtro di convoluzione sobel verticale'"<<endl;  
 
 return bitMap;
   
 }
 

/**
 *
 * Metodo che ritorna una bitMap a cui è stato eseguito il filtro richiesto 
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
 */ 
 
 unsigned char* ImageFilter::eseguiFiltro(int filtro){
  
   switch(filtro){
     
     case 0:
       return bitMap;
     break;
     
     case 1:  
       return luminosita(scala);
     break;
     
     case 2:  
       return gamma(scala);
     break;
     
     case 3:  
       return contrasto(scala);
     break;
     
     case 4:  
       return coloreInverso();
     break;
     
     case 5:  
       return solarise(scala);
     break;
     
     case 6:  
       return scalaDiGrigi();
     break;
     
     case 7:  
       return blur();
     break;
     
     case 8:  
       return gaussianBlur();
     break;
     
     case 9:  
       return boxBlur();
     break;
     
     case 10:  
       return sharpen();
     break;
     
     case 11:  
       return unsharpen();
     break;
     
     case 12:  
       return edgeDetect();
     break;
     
     case 13:  
       return sobelHorizontal();
     break;
     
     case 14:  
       return sobelVertical();
     break;
     
     
   }
   
   
   
 }

  
  
  