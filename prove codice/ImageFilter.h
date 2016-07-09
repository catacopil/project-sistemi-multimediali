

class ImageFilter{
  
private:
  
   unsigned char* bitMap;
   int altezza,larghezza;
   
public:
  
  ImageFilter(unsigned char*,int,int);
  unsigned char* luminosita(double);
  unsigned char* gamma(double);
  unsigned char* contrasto(double);
  unsigned char* coloreInverso();
  unsigned char* solarise(double);
  
};