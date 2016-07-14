
#include "BitMapRGB.h"

using namespace std;

/**
 * Il costruttore della classe BitMapRGB inizializza i campi della classe con la bitMap dell'immagine indicata da nomeFileIn 
 * 
 * @param nomeFileIn: nome del file BMP da leggere
 * @param offset: valore dell'offset dell'immagine
 * @param larg: larghezza in pixel dell'immagine
 * @param alt: altezza in pixel dell'immagine
 * 
 * Il costruttore si posiziona sul byte indicato dall'offset ed inizia a leggere la bitMap dal file; memorizza i dati dei pixel nella variabile bitMap.
 * Il costruttore tiene conto del fatto che le righe devono avere una lunghezza multipla di 4 byte, questo viene gestito tramite le variabili LungRow e LungRowData che indicano quanti byte utili bisogna leggere per ogni riga.
 */
BitMapRGB::BitMapRGB(char* nomeFileIn, int offset, int larg, int alt){
	offsetIMG = offset;
	larghezza = larg;
	altezza = alt;
	color = 24;
	int LungRow;
	int LungRowData = (larghezza*color)/8;												// Byte per riga, con solo i pixel (senza zeri)
	if (LungRowData%4 !=0)
		LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;						// Byte per riga, compresi gli zeri
	else
		LungRow = LungRowData;
	
	FILE *puntFile;
	puntFile = fopen(nomeFileIn,"rb");
	if(puntFile == NULL) {
		cout << "Errore nell'apertura del file " << nomeFileIn << " !\n";
		exit(1);
		}
	fseek(puntFile, offset, 0);
	unsigned char rigaBitMap[LungRow];
	bitMap = new unsigned char[LungRow*altezza];

	int l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);		// lettura della prima riga
	for(int i=0; i<LungRowData; i++){										// copio i dati che mi interessano nella bitMap
		bitMap[i] = rigaBitMap[i];
	}
	int riga=0;
	while ((l == LungRow)&&(riga<altezza)){ 				// lettura delle successive righe
		riga++;
		l=fread(rigaBitMap, sizeof(unsigned char), LungRow, puntFile);		// leggo la riga di dati dal file
		for(int i=0; i<LungRowData; i++){									// copio in bitMap i dati sui pixel
			bitMap[LungRowData*riga+i] = rigaBitMap[i];
		}
	}
	fclose(puntFile);
}

/**
 * Metodo che ritorna la bitMap
 */
unsigned char* BitMapRGB::getBitMap(){
	return bitMap;
}

/**
 * Metodo che sostituisce la bitMap con una nuova passata per argomento, questo metodo permette l'applicazione dei filtri della classe ImageFilter all'oggetto BitMapRGB
 * @param newBitMap: la nuova bitMap
 */
void BitMapRGB::setBitMap(unsigned char* newBitMap){
	bitMap = newBitMap;
}

/**
 * Metodo che scrive la bitMap in un file .bmp (si assume che solo lo header sia già stato scritto, perché scrive in fondo al file)
 * @param nomeFileOut: nome del file nel quale scrivere la bitMap
 */
void BitMapRGB::scriviBitMap(char* nomeFileOut){
	ofstream fileout;
	fileout.open(nomeFileOut, ios_base::app| ios::binary);
	int LungRow;
	int LungRowData = (larghezza*color)/8;										// Byte per riga, con solo i pixel (senza zeri)
	if (LungRowData%4 !=0)
		LungRow=( (larghezza*color)+(32-(larghezza*color)%32) )/8;				// Byte per riga, compresi gli zeri
	else
		LungRow = LungRowData;
	
	for(int i=0; i<altezza; i++){						// scorro tutte le righe
		unsigned char riga[LungRow];					// inizializzo a zero la riga della dimensione necessaria
		for(int j=0; j<LungRowData; j++)				// assegno i valori della bitMap ai primi byte (i rimanenti saranno a zero)
			riga[j] = bitMap[i*LungRowData+j];
		streamsize dimensioneOut = LungRow;
		fileout.write((const char*)riga, dimensioneOut);			// scrivo la riga nel file
	}
	fileout.close();
	cout << " Salvato output in "<<nomeFileOut<< "\n";
}

/**
 * Metodo che ridimensiona la bitMap in modo che rappresenti i pixel di una immagine con le nuove dimensioni passate per argomento
 * @param newLarg: la nuova larghezza
 * @param newAlt: la nuova altezza
 * Utilizza un algoritmo Bilineare per calcolare che valori assegnare ad ogni pixel. Il metodo permette di ingrandire e rimpicciolire le immagini.
 */
void BitMapRGB::ridimensiona(int newLarg, int newAlt){
	if (larghezza==newLarg && altezza==newAlt)
		return;
	unsigned char* newBitMap = new unsigned char[newLarg*newAlt*3];					// costruisce una nuova bitMap con le nuove dimensioni
	double scaleWidth =  (double)newLarg / (double)larghezza;
	double scaleHeight = (double)newAlt / (double)altezza;
	int x2, y2;
	for (int i=0;i<newAlt;i++) {
			for (int j=0;j<newLarg;j++) {
				int pixel = (i * (newLarg *3)) + (j*3);
				int nearestMatch =  (((int)(i / scaleHeight) * (larghezza *3)) + ((int)(j / scaleWidth) *3) );			// calcola la posizione de pixel opportuno da assegnare
				newBitMap[pixel    ] =  bitMap[nearestMatch    ];					// assegnazione dei valori RGB
				newBitMap[pixel + 1] =  bitMap[nearestMatch + 1];
				newBitMap[pixel + 2] =  bitMap[nearestMatch + 2];
			}                
		}
	larghezza = newLarg;						// aggiorna le dimensioni
	altezza = newAlt;
	delete[] bitMap;							// sostituisce la vecchia bitMap con la nuova
	bitMap = newBitMap;
	cout << " Immagine ridimensionata a "<<newLarg<<"x"<<newAlt<< " px\n";
}