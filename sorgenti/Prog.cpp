/*

File programma principale

*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
#include "Header.cpp"
//#include "ImageMono.cpp"
//#include "ImageRGB.cpp"

/*
String FILE_IN;
String FILE_OUT;
String COMANDO;
int larghezzaPX, altezzaPX, larghezzaOUT, altezzaOUT;
int offsetIMG;


Header readHeader(String nomeFile){
    // ritorna l'oggetto Header creato con i dati letti dalla linea di comando
}

ImageMono readMonoColorImage(String nomeFile, Header* header){
    // crea un oggetto ImageMono che contiene la Bitmap dell'immagine monocolore
}

ImageRGB readRGBImage(String nomeFile, Header* header){
    // crea un oggetto ImageRGB che contiene la Bitmap dell'immagine a colori 
}*/

void bin8(int n, char bina[])
{
     int i;
     for (i=0 ; i < 8 ; i++)
     {
         bina[7-i]=(n % 2)+'0';
         n = (n - (n % 2) ) /2;
     }
}


int main(int argc, char *argv[]){
    // considero gli argomenti passati da linea di comando
    
    short headerColor;
    int headerOffset, headerLarg, headerAlt, headerDimIMG;
    
    char* nomeFileIn = "img/kimi512.bmp";
	char* nomeFileOut = "img/output.txt";
    Header* mioHeader = new Header(nomeFileIn);
	mioHeader->stampaInfoHeader();
	
	// prendo i dati dello header
	headerLarg = mioHeader->getLarghezza();
	headerAlt = mioHeader->getAltezza();
	headerColor = mioHeader->getColor();
	headerOffset = mioHeader->getOffsetIMG();
	
	// lettura della bitMap
	int LungRow;
	int LungRowData = (headerLarg*headerColor)/8;												// Byte per riga, con solo i pixel (senza zeri)
	if (LungRowData%4 !=0)
		LungRow=( (headerLarg*headerColor)+(32-(headerLarg*headerColor)%32) )/8;				// Byte per riga, compresi gli zeri aggiuntivi se LungRow non è divisibile per 4 byte
	else
		LungRow = LungRowData;
	
	// TODO: attenzione, LungRow e LungRowData rappresentano byte, definiti così vanno bene se sono nel caso RGB (perché ho 1 o 3 byte per pixel)
	
	FILE *puntFile;
    puntFile = fopen(nomeFileIn,"rb");
	if(puntFile == NULL) {
		cout << "Errore nell'apertura del file " << nomeFileIn << " !\n";
		exit(1);
		}
	fseek(puntFile, headerOffset, 0);
	unsigned char rigaBitMap[LungRow];
	unsigned char bitMap[LungRowData*headerAlt];
	
	int l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);
	for(int i=0; i<LungRowData; i++)			// copio i dati che mi interessano nella bitMap
		bitMap[i] = rigaBitMap[i];
		
	int riga=0;
	while (l == LungRow){
		riga++;
		l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);		// leggo la riga di dati dal file
		for(int i=0; i<LungRowData; i++){									// copio in bitMap i dati sui pixel
			bitMap[LungRowData*riga+i] = rigaBitMap[i];
			//cout << " "<< bitMap[LungRowData*riga+i];
		}
		//cout <<"\n";
	}
	fclose(puntFile);
    
    // applicazione di qualche filtro by Umberto
	// eventuali modifiche nello header
    
    // scrittura dello header e della bitMap
	mioHeader->scriviHeader(nomeFileOut);			// scrittura header (anche la Palette)
	puntFile = fopen(nomeFileOut, "wb");
	if(puntFile == NULL) {
		cout << "Errore nell'apertura del file " << nomeFileIn << " !\n";
		exit(1);
		}
	cout << "LungRow: " << LungRow << " LungRowData: " << LungRowData <<" \t larghezza in px: "<< headerLarg <<endl;
	fseek(puntFile, headerOffset, 0);
	
	// per ogni riga devo aggiungere alla fine degli zeri
	for(int i=0; i<headerAlt; i++){						// scorro tutte le righe
		unsigned char riga[LungRow] = {0};				// inizializzo a zero
		for(int j=0; j<LungRowData; j++)				// assegno i valori della bitMap ai primi byte (i rimanenti saranno a zero)
			riga[j] = bitMap[i*LungRowData+j];
		fwrite(riga, LungRow, 1, puntFile);				// scrivo la riga nel file
	}
	
	fclose(puntFile);
	
	
	
	
    
    // leggo Header e creo la corrispondente classe di immagine (ImageRGB o ImageMono)
	// prendo l'offset dell'immagine e leggo la bitmap
	cout << " -----   FINE  PROGRAMMA  ----- \n";
    }