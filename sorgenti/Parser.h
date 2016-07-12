#include <string>
using namespace std;

class Parser{
  
private:
/**
 *
 *  Parametri privati della classe Parser

 * @param input: contiene l'indirizzo dell'immagine in input
 * @param output: contiene l'indirizzo dell'immagine di output
 * @param filtro: contiene il filtro richiesto
 * @param outAltezza: contiene l'altezza richiesta dell'immagine in output
 * @param outLunghezza contiene la lunghezza richiesta dell'immagine in output
 * @param scale: fattore di scala da applicare al filtro.
 *
**/  
  string input, output, filtro;
  int outAltezza, outLunghezza;
  double scala;
  
  
  
public: 
  /**
   * 
   * Metodi pubblici della classe ImageFilter
   * 
   * Parser: costruttore della classe
   * getOutputName: restiuisce il nome dell'immagine di output
   * getInputFileName: restiuisce il nome dell'immagine di input
   * getFiltro: restiusce il filtro da utilizzare
   * getAltezzaOutput: restiuisce l'altezza richiesta per l'immagine di output
   * getLunghezzaOutput: restiuisce la lunghezza richiesta per l'immagine di output
   * getInputFormat: restiuisce il formato dell'immagine di input
   * getOutputFormat: restiuisce il formato dell'immagine di input
   * 
   */
  
  Parser(char*,char*,char*,int,int,double);  
  char* getOutputName();
  char* getInputFileName();
  int getFiltro();
  int getAltezzaOutput();
  int getLunghezzaOutput();
  int getInputFormat();
  int getOutputFormat();
  double getScala();
  
};