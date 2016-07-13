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

int ImagePNG::getLarghezza(){
	return larghezza;
}

int ImagePNG::getAltezza(){
	return altezza;
}

void ImagePNG::ridimensiona(int newLarg, int newAlt){
	
	// TODO: ATTENZIONE, SI IPOTIZZA CHE IL RIDIMENSIONAMENTO VIENE FATTO PRIMA DEL SALVATAGGIO (dopo il ridimensionamento bitMapRGB e bitMapAlpha non sono aggiornati)
	if (larghezza==newLarg && altezza==newAlt)
			return;
	vector<unsigned char> newBitMap (newLarg*newAlt*4);
	double scaleWidth =  (double)newLarg / (double)larghezza;
    double scaleHeight = (double)newAlt / (double)altezza;
	int x2, y2;
	
	for (int i=0;i<newAlt;i++) {
		for (int j=0;j<newLarg;j++) {
			int punto = (i * (newLarg *4)) + (j*4);
			int nearestMatch =  (((int)(i / scaleHeight) * (larghezza *4)) + ((int)(j / scaleWidth) *4) );
						
			newBitMap[punto    ] =  pixel[nearestMatch    ];
			newBitMap[punto + 1] =  pixel[nearestMatch + 1];
			newBitMap[punto + 2] =  pixel[nearestMatch + 2];
			newBitMap[punto + 3] =  pixel[nearestMatch + 3];
			}                
		}
	larghezza = newLarg;
	altezza = newAlt;
	//cout << "size pixel: "<< pixel.size()<<endl;
	//cout << "size newBitMap: "<< newBitMap.size()<<endl;
	pixel = newBitMap;
	
	// reinizializzo le bitMap con i nuovi dati
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
	for(int i=0,j=0; i<dimBitMap; i++){				// reimposto il vector pixel con i byte RGBA
		if ((j+1)%4==0){	
			j++;
		}
		pixel[j] = newBitMapRGB[i];
		j++;
	}
}

void ImagePNG::setBitMapAlpha(unsigned char* newBitMapAlpha){
	// TODO: da implementare aggiornando pixel
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
	state.encoder.text_compression = 1;			// mi sa che comprime più del dovuto perché l'output è più piccolo
	unsigned error = lodepng::encode(png, pixel, larghezza, altezza, state);
	if(error)
		cout << "Errore nella codifica " << error << ": " << lodepng_error_text(error) << endl;
  
	lodepng::save_file(png, nomeFileOut);
}

void ImagePNG::scriviBMPconHeader(char* nomeFileOut){				// scrive l'immagine in un BMP con Header standard
	vector<unsigned char> bmp;
	
	//byte 0-13
	bmp.push_back('B'); bmp.push_back('M'); //0: bfType
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); //2: bfSize; size not yet known for now, filled in later.
	bmp.push_back(0); bmp.push_back(0); //6: bfReserved1
	bmp.push_back(0); bmp.push_back(0); //8: bfReserved2
	bmp.push_back(54 % 256); bmp.push_back(54 / 256); bmp.push_back(0); bmp.push_back(0); //10: bfOffBits (54 header bytes)

	//bytes 14-53 
	bmp.push_back(40); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //14: biSize
	bmp.push_back(larghezza % 256); bmp.push_back(larghezza / 256); bmp.push_back(0); bmp.push_back(0); //18: biWidth
	bmp.push_back(altezza % 256); bmp.push_back(altezza / 256); bmp.push_back(0); bmp.push_back(0); //22: biHeight
	bmp.push_back(1); bmp.push_back(0); //26: biPlanes
	bmp.push_back(24); bmp.push_back(0); //28: biBitCount
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //30: biCompression
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //34: biSizeImage
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //38: biXPelsPerMeter
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //42: biYPelsPerMeter
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //46: biClrUsed
	bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //50: biClrImportant
	
	/*
	Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
	-BMP stores the rows inversed, from bottom to top
	-BMP stores the color channels in BGR instead of RGB order
	-BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
	*/

	int imagerowbytes = 3 * larghezza;
	imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); //must be multiple of 4
	
	for(int y = altezza - 1; y >= 0; y--){			// le righe sono memorizzate al contrario
		int c = 0;
		for(int x = 0; x < imagerowbytes; x++){
		if(x < larghezza * 3){
			int inc = c;
			//Convert RGB(A) into BGR(A)
			if(c == 0) inc = 2;
			else if(c == 2) inc = 0;
			bmp.push_back(bitMapRGB[3 * (larghezza * y + x / 3) + inc]);
		}
		else bmp.push_back(0);
		c++;
		if(c >= 3) c = 0;
		}
	}

	// Sistema le dimensioni dell'intero file
	bmp[2] = bmp.size() % 256;
	bmp[3] = (bmp.size() / 256) % 256;
	bmp[4] = (bmp.size() / 65536) % 256;
	bmp[5] = bmp.size() / 16777216;
	
	lodepng::save_file(bmp, nomeFileOut);			// scrive il file
}
