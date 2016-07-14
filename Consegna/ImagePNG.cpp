
#include "ImagePNG.h"

using namespace std;

/**
 * Il costruttore della classe ImagePNG legge il file PNG passato per argomento, ne estrae i pixel ed inizializza la bitMap dei colori in formato RGB; estrae i valori Alpha e li memorizza in bitMapAlpha
 * 
 * @param nomeFileIn: nome del file PNG da leggere
 */
ImagePNG::ImagePNG(char* nomeFileIn){
	state.decoder.color_convert = 0;
	state.decoder.remember_unknown_chunks = 1; 			//make it reproduce even unknown chunks in the saved image
	
	unsigned error = lodepng::load_file(png, nomeFileIn);
	if(error) {
		cout << "Errore decodifica: " << error << ": " << lodepng_error_text(error) << endl;
		cout << "Il programma verrà terminato "<< endl;
		exit(1);	
	}
	else
		error = lodepng::decode(pixel, larghezza, altezza, png);
	
	if (error){
		cout << "Errore decodifica: " << error << ": " << lodepng_error_text(error) << endl;
		cout << "Il programma verrà terminato "<< endl;
		exit(1);
	  
	}
	
	dimBitMap = larghezza*altezza*3;
	bitMapRGB = new unsigned char[dimBitMap];
	bitMapAlpha = new unsigned char[larghezza*altezza];
	for(int i=0,j=0; i<dimBitMap; i++){	
				// prendo i valori RGB
		if ((j+1)%4==0){					// se il numero successivo è divisibile per 4 (Alpha è il quarto elemento, ma l'indice parte da zero)
			bitMapAlpha[j-i] = pixel[j];		// salvo il dato in bitMapAlpha
			j++;
		}
		bitMapRGB[i] = pixel[j];				// salvo il dato in bitMapRGB
		j++;
	}
}

/**
 * Il secondo costruttore della classe ImagePNG inizializza l'immagine PNG tramite bitMapBMP che sono i dati dei pixel in formato BMP (pixel espressi in BGR e righe in ordine inverso)
 * 
 * @param bitMapBMP: array contenente la bitMap in formato BMP con la quale inizializzare l'immagine PNG
 * @param larg: la larghezza in pixel dell'immagine da costruire
 * @param alt: l'altezza in pixel dell'immagine da costruire
 */
ImagePNG::ImagePNG(unsigned char* bitMapBMP, int larg, int alt){
	  larghezza = larg;
	  altezza = alt;
	  state.decoder.color_convert = 0;
	  state.decoder.remember_unknown_chunks = 1;
	  
	  // inizializza il vettore pixel con la giusta dimensione
	  pixel.resize(larghezza*altezza*4);
	  for(int i=0; i<altezza; i++)
		  for(int j=0; j<larghezza; j++){
			  int posBMP = (altezza-i-1)*larghezza*3 + j*3;			// calcola la posizione del prossimo pixel nella bitMapBMP
			  int posPNG = i*larghezza*4 + j*4;
			  pixel[posPNG] = bitMapBMP[posBMP+2];
			  pixel[posPNG+1] = bitMapBMP[posBMP+1];
			  pixel[posPNG+2] = bitMapBMP[posBMP];
			  pixel[posPNG+3] = 255;
		  } 

	  dimBitMap = larghezza*altezza*3;
	  bitMapRGB = new unsigned char[dimBitMap];
	  bitMapAlpha = new unsigned char[larghezza*altezza];
	  for(int i=0,j=0; i<dimBitMap; i++){					// prendo i valori RGB 
		  if ((j+1)%4==0){					// se il numero successivo è divisibile per 4 (Alpha è il quarto elemento, ma l'indice parte da zero)
			  bitMapAlpha[j-i] = pixel[j];		// salvo il dato in bitMapAlpha
			  j++;
		  }
		  bitMapRGB[i] = pixel[j];				// salvo il dato in bitMapRGB
		  j++;
	  }
}

/**
 * Metodo che ritorna la larghezza in pixel dell'immagine
 */
int ImagePNG::getLarghezza(){
	return larghezza;
}

/**
 * Metodo che ritorna l'altezza in pixel dell'immagine
 */
int ImagePNG::getAltezza(){
	return altezza;
}

/**
 * Metodo che ridimensiona l'immagine con le nuove dimensioni passate per argomento. Può ingrandire o rimpicciolire l'immagine
 * @param newLarg: la nuova larghezza dell'immagine
 * @param newAlt: la nuova altezza dell'immagine
 * Il metodo crea una nuova bitMap che popola utilizzando un algoritmo Bilineare. Dopo aver creato la nuova bitMap essa viene sostituita a pixel, successivamente reimposta i valori di bitMapRGB e bitMapAlpha
 */
void ImagePNG::ridimensiona(int newLarg, int newAlt){
	
  if(newAlt>0 && newLarg>0){
	if (larghezza==newLarg && altezza==newAlt)
			return;
	vector<unsigned char> newBitMap (newLarg*newAlt*4);
	double scaleWidth =  (double)newLarg / (double)larghezza;
	double scaleHeight = (double)newAlt / (double)altezza;
	int x2, y2;
	
	for (int i=0;i<newAlt;i++) {
		for (int j=0;j<newLarg;j++) {
			int punto = (i * (newLarg *4)) + (j*4);
			int nearestMatch =  (((int)(i / scaleHeight) * (larghezza *4)) + ((int)(j / scaleWidth) *4) );		// calcola il pixel più vicino nella bitMap originale
			newBitMap[punto    ] =  pixel[nearestMatch    ];			// assegna i valori del pixel in formato RGBA
			newBitMap[punto + 1] =  pixel[nearestMatch + 1];
			newBitMap[punto + 2] =  pixel[nearestMatch + 2];
			newBitMap[punto + 3] =  pixel[nearestMatch + 3];
			}                
		}
	larghezza = newLarg;
	altezza = newAlt;
	pixel = newBitMap;									// assegno la nuova bitMap con i dati dei pixel
	
	dimBitMap = larghezza*altezza*3;					// reinizializzo bitMapRGB e bitMapAlpha con i nuovi dati
	delete[] bitMapRGB;
	delete[] bitMapAlpha;
	bitMapRGB = new unsigned char[dimBitMap];
	bitMapAlpha = new unsigned char[larghezza*altezza];
	for(int i=0,j=0; i<dimBitMap; i++){				// prendo i valori RGB e Alpha
		if ((j+1)%4==0){
			bitMapAlpha[j-i] = pixel[j];		// salvo il dato in bitMapAlpha
			j++;
		}
		bitMapRGB[i] = pixel[j];				// salvo il dato in bitMapRGB
		j++;
	}
	cout << " Immagine ridimensionata a "<<newLarg<<"x"<<newAlt<< " px\n"; 
  }else{
       cout<<"ERRORE! Sono state inserite dimensioni negative o uguali a zero. Ridimensionamento non eseguito"<<endl; 
  }
}

/**
 * Metodo che ritorna bitMapRGB, contenente i pixel in formato RGB
 */
unsigned char* ImagePNG::getBitMapRGB(){
	return bitMapRGB;
}

/**
 * Metodo che ritorna bitMapAlpha, contenente i valori Alpha dei pixel
 */
unsigned char* ImagePNG::getBitMapAlpha(){
	return bitMapAlpha;
}

/**
 * Metodo che reimposta la bitMapRGB con un nuovo array passato come argomento
 * @param newBitMapRGB: la nuova bitMapRGB
 * Il metodo è richiamato dopo l'applicazione dei filtri alla bitMapRGB estratta dall'immagine PNG. Dopo aver sostituito la variabile bitMapRGB, la bitMap pixel viene aggiornata con i nuovi valori dei pixel (la componente Alpha viene trascurata)
 */
void ImagePNG::setBitMapRGB(unsigned char* newBitMapRGB){
	bitMapRGB = newBitMapRGB;
	for(int i=0,j=0; i<dimBitMap; i++){			// reimposto il vector pixel con i byte RGBA
		if ((j+1)%4==0)
			j++;								// non modifico Alpha
		pixel[j] = newBitMapRGB[i];
		j++;
	}
}

/**
 * Metodo che scrive l'immagine PNG in un file di output PNG indicato
 * @param nomeFileOut: nome del file di output nel quale scrivere l'immagine
 * Per la codifica del file PNG viene utilizzato il metodo encode della libreria lodepng
 */
void ImagePNG::scriviPNG(char* nomeFileOut){
	for(int i=0,j=0; i<dimBitMap; i++){				// reimposto il vector pixel con i byte RGBA
		if ((j+1)%4==0){	
			pixel[j] = bitMapAlpha[j-i];
			j++;
		}
		pixel[j] = bitMapRGB[i];
		j++;
	}
	
	png.clear();
	state.encoder.text_compression = 1;				// applica una compressione per il file di output
	unsigned error = lodepng::encode(png, pixel, larghezza, altezza, state);
	if(error)
		cout << "Errore nella codifica " << error << ": " << lodepng_error_text(error) << endl;
	lodepng::save_file(png, nomeFileOut);
	cout << " Salvato output in "<<nomeFileOut<< "\n";
}

/**
 * Metodo che scrive effetua una conversione dell'immagine PNG in BMP, scrive l'immagine in un file di output .bmp
 * @param nomeFileOut: nome del file di output nel quale scrivere l'immagine
 * Il metodo costruisce tramite il vector bmp il file da scrivere. Inizialmente viene scritto lo header standard e poi aggiunti i pixel che formano la bitMap (cambiando l'ordine dei colori e delle righe) 
 */
void ImagePNG::scriviBMPconHeader(char* nomeFileOut){
	vector<unsigned char> bmp;									// vector utilizzato per memorizzare tutti i byte da scrivere nel file bmp
	
	//byte 0-13
	bmp.push_back('B'); bmp.push_back('M'); //0: bfType
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); //2: bfSize; size not yet known for now, filled in later.
	bmp.push_back(0); bmp.push_back(0); //6: bfReserved1
	bmp.push_back(0); bmp.push_back(0); //8: bfReserved2
	bmp.push_back(54 % 256); bmp.push_back(54 / 256); bmp.push_back(0); bmp.push_back(0); //10: bfOffBits (54 header bytes)

	//bytes 14-53 
	bmp.push_back(40); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//14: Size
	bmp.push_back(larghezza % 256); bmp.push_back(larghezza / 256); bmp.push_back(0); bmp.push_back(0); 	//18: Width
	bmp.push_back(altezza % 256); bmp.push_back(altezza / 256); bmp.push_back(0); bmp.push_back(0); 		//22: Height
	bmp.push_back(1); bmp.push_back(0); 																	//26: Planes
	bmp.push_back(24); bmp.push_back(0); 																	//28: BitCount
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//30: Compression
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//34: SizeImage
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//38: XPelsPerMeter
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//42: YPelsPerMeter
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//46: ClrUsed
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  								//50: ClrImportant
	
	//ogni riga deve avere un numero di byte multiplo di 4, se non è così vengono aggiunti degli zero a fine riga
	int imagerowbytes = 3 * larghezza;
	imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); 
	
	for(int y = altezza - 1; y >= 0; y--){			// le righe sono memorizzate al contrario
		int c = 0;
		for(int x = 0; x < imagerowbytes; x++){
		if(x < larghezza * 3){
			int inc = c;							// conversione da RGB a BGR
			if(c == 0) inc = 2;
			else if(c == 2) inc = 0;
			bmp.push_back(bitMapRGB[3 * (larghezza * y + x / 3) + inc]);
		}
		else bmp.push_back(0);
		c++;
		if(c >= 3) c = 0;
		}
	}
	bmp[2] = bmp.size() % 256;						// Scrive le dimensioni dell'intero file, un intero quindi 4 byte
	bmp[3] = (bmp.size() / 256) % 256;
	bmp[4] = (bmp.size() / 65536) % 256;
	bmp[5] = bmp.size() / 16777216;
	
	lodepng::save_file(bmp, nomeFileOut);
	cout << " Salvato output in "<<nomeFileOut<< "\n";
}
