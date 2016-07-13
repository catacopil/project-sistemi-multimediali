
#include "BitMapRGB.h"

using namespace std;

BitMapRGB::BitMapRGB(char* nomeFileIn, int offset, int larg, int alt){
	offsetIMG = offset;
	larghezza = larg;
	altezza = alt;
	color = 24;
	int LungRow;
	int LungRowData = (larghezza*color)/8;												// Byte per riga, con solo i pixel (senza zeri)
	if (LungRowData%4 !=0)
		LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;				// Byte per riga, compresi gli zeri
	else
		LungRow = LungRowData;
	
	//cout << "LungRow: " << LungRow << " LungRowData: " << LungRowData <<" \t larghezza in px: "<< larghezza <<endl;
	FILE *puntFile;
	
	puntFile = fopen(nomeFileIn,"rb");
	if(puntFile == NULL) {
		cout << "Errore nell'apertura del file " << nomeFileIn << " !\n";
		exit(1);
		}
	fseek(puntFile, offset, 0);
	unsigned char rigaBitMap[LungRow];
	bitMap = new unsigned char[LungRow*altezza];

	int l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);
	for(int i=0; i<LungRowData; i++){			// copio i dati che mi interessano nella bitMap
		bitMap[i] = rigaBitMap[i];
	}
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
	
unsigned char* BitMapRGB::getBitMap(){
	return bitMap;
}
	
void BitMapRGB::setBitMap(unsigned char* newBitMap){
	bitMap = newBitMap;
}
	
void BitMapRGB::scriviBitMap(char* nomeFileOut){
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
	
void BitMapRGB::ridimensiona(int newLarg, int newAlt){				// RIDIMENSIONA L'IMMAGINE CON I NUOVI PIXEL
	if (larghezza==newLarg && altezza==newAlt)
		return;
	unsigned char* newBitMap = new unsigned char[newLarg*newAlt*3];
	double scaleWidth =  (double)newLarg / (double)larghezza;
	double scaleHeight = (double)newAlt / (double)altezza;
	int x2, y2;
	for (int i=0;i<newAlt;i++) {
			for (int j=0;j<newLarg;j++) {
				int pixel = (i * (newLarg *3)) + (j*3);
				int nearestMatch =  (((int)(i / scaleHeight) * (larghezza *3)) + ((int)(j / scaleWidth) *3) );
				newBitMap[pixel    ] =  bitMap[nearestMatch    ];
				newBitMap[pixel + 1] =  bitMap[nearestMatch + 1];
				newBitMap[pixel + 2] =  bitMap[nearestMatch + 2];
			}                
		}
	larghezza = newLarg;
	altezza = newAlt;
	delete[] bitMap;
	bitMap = newBitMap;
	
}