
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

class BitMapRGB{
	/*
	 * La classe BitMapRGB rappresenta una bitMap RGB a 24 bit. Il costruttore riceve in input il nome del file da dove prendere la bitMap, l'offset e le dimensioni in px
	 * Il metodo getBitMap restituisce un array di unsigned char contenente la bitMap
	 */
private:
	unsigned char* bitMap;
	int offsetIMG, larghezza, altezza;
	int color = 24;			// 24 bit per ogni pixel
public:
	BitMapRGB(char* nomeFileIn, int offset, int larg, int alt){
		offsetIMG = offset;
		larghezza = larg;
		altezza = alt;
		int LungRow;
		int LungRowData = (larghezza*color)/8;												// Byte per riga, con solo i pixel (senza zeri)
		if (LungRowData%4 !=0)
			LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;				// Byte per riga, compresi gli zeri
		else
			LungRow = LungRowData;
	
		cout << "LungRow: " << LungRow << " LungRowData: " << LungRowData <<" \t larghezza in px: "<< larghezza <<endl;
		
		FILE *puntFile;
		puntFile = fopen(nomeFileIn,"rb");
		if(puntFile == NULL) {
			cout << "Errore nell'apertura del file " << nomeFileIn << " !\n";
			exit(1);
			}
		fseek(puntFile, offset, 0);
		unsigned char rigaBitMap[LungRow];
		bitMap = new unsigned char[LungRowData*altezza];
		
		int l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);
		for(int i=0; i<LungRowData; i++)			// copio i dati che mi interessano nella bitMap
			bitMap[i] = rigaBitMap[i];
		
		int riga=0;
		while ((l == LungRow)&&(riga<altezza)){ 	// controllo anche che non vada oltre l'altezza, altrimenti rischia di crashare perché scrive fuori dalla bitMap!
			riga++;
			l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);		// leggo la riga di dati dal file
			for(int i=0; i<LungRowData; i++){									// copio in bitMap i dati sui pixel
				bitMap[LungRowData*riga+i] = rigaBitMap[i];
			}
		}
		fclose(puntFile);
	}
	
	unsigned char* getBitMap(){
			return bitMap;
	}
	
	void setBitMap(unsigned char* newBitMap){
		delete[] bitMap;
		bitMap = newBitMap;
	}
	
	void scriviBitMap(char* nomeFileOut){
		ofstream fileout;
		fileout.open(nomeFileOut, ios_base::app| ios::binary);
		int LungRow;
		int LungRowData = (larghezza*color)/8;												// Byte per riga, con solo i pixel (senza zeri)
		if (LungRowData%4 !=0)
			LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;				// Byte per riga, compresi gli zeri
		else
			LungRow = LungRowData;
		
		// per ogni riga devo aggiungere alla fine degli zeri
		for(int i=0; i<altezza; i++){						// scorro tutte le righe
			unsigned char riga[LungRow];					// inizializzo a zero la riga della dimensione necessaria
			for(int j=0; j<LungRowData; j++)				// assegno i valori della bitMap ai primi byte (i rimanenti saranno a zero)
				riga[j] = bitMap[i*LungRowData+j];
			streamsize dimensioneOut = LungRow;
			fileout.write((const char*)riga, dimensioneOut);			// scrivo la riga nel file
		
		}
		fileout.close();
	}
	
	void ridimensiona(int newLarg, int newAlt){				// RIDIMENSIONA L'IMMAGINE CON I NUOVI PIXEL
		if (larghezza==newLarg && altezza==newAlt)
			return;
		unsigned char* newBitMap = new unsigned char[newLarg*newAlt*3];
		double scaleWidth =  (double)newLarg / (double)larghezza;
        double scaleHeight = (double)newAlt / (double)altezza;
		int x2, y2;
		for (int i=0;i<newAlt;i++) {
				for (int j=0;j<newLarg;j++) {
					int pixel = (i * (newLarg *3)) + (j*3);
					int nearestMatch =  (((int)(i / scaleHeight) * (altezza *3)) + ((int)(j / scaleWidth) *3) );
					newBitMap[pixel    ] =  bitMap[nearestMatch    ];
					newBitMap[pixel + 1] =  bitMap[nearestMatch + 1];
					newBitMap[pixel + 2] =  bitMap[nearestMatch + 2];
				}                
			}
		larghezza = newLarg;
		altezza = newAlt;
		delete[] bitMap;
		bitMap = newBitMap;
		
	/*	public int[] resizePixels(int[] pixels,int w1,int h1,int w2,int h2) {
			int[] temp = new int[w2*h2] ;
			// EDIT: added +1 to account for an early rounding problem
			int x_ratio = (int)((w1<<16)/w2) +1;
			int y_ratio = (int)((h1<<16)/h2) +1;
			//int x_ratio = (int)((w1<<16)/w2) ;
			//int y_ratio = (int)((h1<<16)/h2) ;
			int x2, y2 ;
			for (int i=0;i<h2;i++) {
				for (int j=0;j<w2;j++) {
					x2 = ((j*x_ratio)>>16) ;
					y2 = ((i*y_ratio)>>16) ;
					temp[(i*w2)+j] = pixels[(y2*w1)+x2] ;
				}                
			}                
			return temp ;
		}	*/

		
	}
};