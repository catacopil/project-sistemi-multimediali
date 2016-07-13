#include "ImagePNG.h"

using namespace std;

/**
 * Il costruttore della classe ImagePNG legge il file PNG passato per argomento, ne estrae i pixel ed inizializza le bitMap dei colori e della trasparenza
 * 
 */

ImagePNG::ImagePNG(char* nomeFileIn){
	state.decoder.color_convert = 0;
	state.decoder.remember_unknown_chunks = 1; 			//make it reproduce even unknown chunks in the saved image
	
	unsigned error = lodepng::load_file(png, nomeFileIn);
	if(error) 
		cout << "Errore decodifica: " << error << ": " << lodepng_error_text(error) << endl;
	else
		error = lodepng::decode(pixel, larghezza, altezza, png);
	if (error) 
		cout << "Errore decodifica: " << error << ": " << lodepng_error_text(error) << endl;
	
	dimBitMap = larghezza*altezza*3;
	bitMapRGB = new unsigned char[dimBitMap];
	bitMapAlpha = new unsigned char[larghezza*altezza];
	for(int i=0,j=0; i<dimBitMap; i++){				// prendo i valori RGB
		if ((j+1)%4==0){					// se il numero successivo è divisibile per 4 (Alpha è il quarto elemento, ma l'indice parte da zero)
			bitMapAlpha[j-i] = pixel[j];		// salvo il dato in bitMapAlpha
			j++;
		}
		bitMapRGB[i] = pixel[j];				// salvo il dato in bitMapRGB
		j++;
	}
}

int ImagePNG::getLarghezza(){
	return larghezza;
}

int ImagePNG::getAltezza(){
	return altezza;
}

void ImagePNG::setLarghezza(int newLarg){
	// TODO: da cancellare...si usa ridimensiona(larg, alt)
	larghezza = newLarg;
}

void ImagePNG::setAltezza(int newAlt){
	// TODO: da cancellare...si usa ridimensiona(larg, alt)
	altezza = newAlt;
}

void ImagePNG::ridimensiona(int newLarg, int newAlt){
	// TODO: implementare la soluzione utilizzata nella bitMapRGB
	// ATTENZIONE, SI IPOTIZZA CHE IL RIDIMENSIONAMENTO VIENE FATTO PRIMA DEL SALVATAGGIO (dopo il ridimensionamento bitMapRGB e bitMapAlpha non sono aggiornati)
	if (larghezza==newLarg && altezza==newAlt)
			return;
	vector<unsigned char> newBitMap (newLarg*newAlt*4);
	double scaleWidth =  (double)newLarg / (double)larghezza;
    double scaleHeight = (double)newAlt / (double)altezza;
	int x2, y2;
	for (int i=0;i<newAlt;i++) {
		for (int j=0;j<newLarg;j++) {
			int punto = (i * (newLarg *4)) + (j*4);
			int nearestMatch =  (((int)(i / scaleHeight) * (altezza *4)) + ((int)(j / scaleWidth) *4) );
			newBitMap[punto    ] =  pixel[nearestMatch    ];
			newBitMap[punto + 1] =  pixel[nearestMatch + 1];
			newBitMap[punto + 2] =  pixel[nearestMatch + 2];
			newBitMap[punto + 3] =  pixel[nearestMatch + 3];
			}                
		}
	larghezza = newLarg;
	altezza = newAlt;
	cout << "size pixel: "<< pixel.size()<<endl;
	cout << "size newBitMap: "<< newBitMap.size()<<endl;
	pixel = newBitMap;
	// reinizializzo le bitMap con le nuove
	dimBitMap = larghezza*altezza*3;
	delete[] bitMapRGB;
	delete[] bitMapAlpha;
	bitMapRGB = new unsigned char[dimBitMap];
	bitMapAlpha = new unsigned char[larghezza*altezza];
	for(int i=0,j=0; i<dimBitMap; i++){				// prendo i valori RGB
		if ((j+1)%4==0){					// se il numero successivo è divisibile per 4 (Alpha è il quarto elemento, ma l'indice parte da zero)
			bitMapAlpha[j-i] = pixel[j];		// salvo il dato in bitMapAlpha
			j++;
		}
		bitMapRGB[i] = pixel[j];				// salvo il dato in bitMapRGB
		j++;
	}
}

unsigned char* ImagePNG::getBitMapRGB(){
	return bitMapRGB;
}

unsigned char* ImagePNG::getBitMapAlpha(){
	return bitMapAlpha;
}

void ImagePNG::setBitMapRGB(unsigned char* newBitMapRGB){
	bitMapRGB = newBitMapRGB;
}

void ImagePNG::setBitMapAlpha(unsigned char* newBitMapAlpha){
	bitMapAlpha = newBitMapAlpha;
}

void ImagePNG::scriviPNG(char* nomeFileOut){					// scrive il PNG in un determinato file di output
	
	for(int i=0,j=0; i<dimBitMap; i++){				// reimposto il vector pixel con i byte RGBA
		if ((j+1)%4==0){	
			pixel[j] = bitMapAlpha[j-i];
			j++;
		}
		pixel[j] = bitMapRGB[i];
		j++;
	}
	
	png.clear();			// funziona anche senza, però penso sia necessario per eseguire l'encoding con lo state
	state.encoder.text_compression = 1;			// mi sa che comprime più del dovuto perché l'output è più grande
	unsigned error = lodepng::encode(png, pixel, larghezza, altezza, state);
	if(error)
		cout << "Errore nella codifica " << error << ": " << lodepng_error_text(error) << endl;
  
	lodepng::save_file(png, nomeFileOut);
	
}
