

/**
 * 
 * La classe ImageFilter è la classe che si occupa di eseguire i filtri sulle immagini
 * 
 **/
class ImageFilter{
  
private:
  
/** 
Parametri privati della classe Image Filter

@param bitMap: contiene la Bit Map dell'immagine
@param altezza: contiene l'altezza dell'immagine
@param lunghezza: contiene la lunghezza dell'immagine
@param bitColor: contiene il numero di bit per colore utilizzati dall'immagine
**/  
   unsigned char* bitMap;
   int altezza,lunghezza,bitColor;
/**
 * Funzioni private della classe
 *
 * applyRGBConvolution: applica una convoluzione, utilizzando la matrice kernel 3x3, su un immagine applyRGBConvolution
 * returnRGBPixel: ritorna il pixel in posizione i,j di una immagine RGB
 *
 */   
   int applyRGBConvolution(unsigned char*,float kernel[3][3],int,int,int);
   unsigned char returnRGBPixel(unsigned char*,int,int);
   
public:
  
  /**
   * 
   * Metodi pubblici della classe ImageFilter
   * 
   * ImageFilter: costruttore della classe
   * luminosita: applica un filtro di luminosita di valore uguale alla scala data in input
   * gamma: applica un filtro gamma di valore uguale alla scala data in input
   * contrasto: applica un filtro di luminosita di valore uguale alla scala data in input
   * coloreInverso: applica un filtro colore inverso sull'immagine
   * solarise: applica un filtro solarise sull'immagine con soglia data in input
   * scalaDiGrigi: trasforma un immagine RGB in una immagine in scala di grigi
   * 
   * blur: applica un filtro blur sull'immagine
   * gaussianBlur: applica un filtro di blur gaussiano sull'immagine
   * boxBlur: applica un filtro box blur sull'immagine
   * sharpen: applica un filtro di sharpening sull'immagine
   * unsharpen: applica un filtro di unsharpening sull'immagine
   * edgeDetect: applica un filtro che esegue l'edge detect sull'immagine
   * sobelHorizontal: applica un filtro di sobel orizzontale sull'immagine
   * sobelVertical: applica un filtro di sobel verticale sull'immagine
   */
  
  ImageFilter(unsigned char*,int,int,int);
  
  //Filtri puntuali
  unsigned char* luminosita(double);
  unsigned char* gamma(double);
  unsigned char* contrasto(double);
  unsigned char* coloreInverso();
  unsigned char* solarise(double);
  unsigned char* scalaDiGrigi();
  
  // Filtri di convoluzione
  unsigned char* blur();
  unsigned char* gaussianBlur();
  unsigned char* boxBlur();
  unsigned char* sharpen();
  unsigned char* unsharpen();
  unsigned char* edgeDetect();
  unsigned char* sobelHorizontal();
  unsigned char* sobelVertical();

};