

class ImageFilter{
  
private:
  
   unsigned char* bitMap;
   int altezza,lunghezza;
   
   int apply3x3Convolution(double kernel[3][3],int,int);
   
   
public:
  
  ImageFilter(unsigned char*,int,int);
  unsigned char* luminosita(double);
  unsigned char* gamma(double);
  unsigned char* contrasto(double);
  unsigned char* coloreInverso();
  unsigned char* solarise(double);
  
  unsigned char* blur();
  

  
};