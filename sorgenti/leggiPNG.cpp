/*
 * Test per la lettura di PNG utilizzando la libreria libpng
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "lodepng.h"

using namespace std;

int main(int argc, char *argv[]) {
	char* nomeFileIn = "img/pngtest.png";
	char* nomeFileOut = "img/output.png";
	
	unsigned int larghezza, altezza;
	vector<unsigned char> png;			// tutti i byte del png
	vector<unsigned char> pixel; 		// tutti i byte dei pixel
	unsigned char* bitMap; 				// contiene i valori RGB
	int dimBitMap;
	lodepng::State state;
	
	// funziona anche senza le opzioni dello state ma non comprime, quindi l'output verrebbe fuori più grande
	state.decoder.color_convert = 0;
	state.decoder.remember_unknown_chunks = 1; //make it reproduce even unknown chunks in the saved image
	
	unsigned error = lodepng::load_file(png, nomeFileIn);
	if(!error) error = lodepng::decode(pixel, larghezza, altezza, png);
	
	if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
	
	dimBitMap = larghezza*altezza*3;
	bitMap = new unsigned char[dimBitMap];
	for(int i=0,j=0; i<dimBitMap; i++){				// prendo i valori RGB
		if (i==3) j++;
		bitMap[i] = pixel[j];
		j++;
	}
	
	// invocazione di qualche filtro
	
	for(int i=0, j=0; i<dimBitMap;i++){				// rimetto i valori RGB
		if (i==3) j++;
		pixel[j] = bitMap[i];
		j++;
	}
	
	png.clear();			// funziona anche senza, però penso sia necessario per eseguire l'encoding con lo state
	state.encoder.text_compression = 1;			// mi sa che comprime più del dovuto perché l'output è più grande
	error = lodepng::encode(png, pixel, larghezza, altezza, state);
	if(error){
		cout << "encoder error " << error << ": " << lodepng_error_text(error) << endl;
		return 0;
	}
  
	lodepng::save_file(png, nomeFileOut);
	
	// TODO: lettura del file, in caso di PNG spezzare in due array, uno per RGB e l'altro per Alpha
	
	cout << "FINE PROGRAMMA" << endl;
}