
#include "Header.h"

using namespace std;

/**
 * Il costruttore della classe Header legge l'intero header dell'immagine BMP indicata da nomeFileIn e memorizza nei campi dati della classe le informazioni
 * 
 * @param nomeFileIn: nome del file BMP da leggere
 * 
 */
	Header::Header(char* nomeFile){				// Costruttore che legge l'intero Header (si presume sia quello standard da 54 Byte)
        
		FILE *puntFile;
        puntFile = fopen(nomeFile,"rb");
		if(puntFile == NULL) {
			cout << "Errore nell'apertura del file " << nomeFile << " !\n";
			exit(1);
			}
        
		fseek(puntFile, 0, 0);
		fread(headerContent, DIM_HEADER_INIZIALE, 1, puntFile);		// leggo l'intero header e lo metto in headerContent

		fseek(puntFile, 0, 0);
		fread(firma,2, 1, puntFile);
	
		fseek(puntFile,2,0);
		fread(&dimensione_totale, 1, sizeof(int), puntFile);			// legge la dimensione totale dell'immagine
		
		fseek(puntFile,6,0);
		fread(&riservato, 1, sizeof(int), puntFile);
		
		fseek(puntFile,10,0);
		fread(&offset, 1, sizeof(int), puntFile);					// legge il punto in cui inizia la bitmap
		
		fseek(puntFile,14,0);
		fread(&header_size,sizeof(int), 1, puntFile);
		
		fseek(puntFile,18,0);
		fread(&larghezza,sizeof(int), 1, puntFile);
		
		fseek(puntFile,22,0);
		fread(&altezza,sizeof(int), 1, puntFile);
		
		fseek(puntFile,26,0);
		fread(piani,2, 1, puntFile);
		
		fseek(puntFile,28,0);
		fread(&color,sizeof(int), 1, puntFile);
		if (color==24)
			esistePalette = false;
		else 
			esistePalette = true;
		
		fseek(puntFile,30,0);
		fread(&compressione,sizeof(int), 1, puntFile);
		
		fseek(puntFile,34,0);
		fread(&dimensione_immagine,sizeof(int), 1, puntFile);
		
		fseek(puntFile,38,0);
		fread(&ris_orizzontale,sizeof(int), 1, puntFile);
		
		fseek(puntFile,42,0);
		fread(&ris_verticale,sizeof(int), 1, puntFile);
		
		fseek(puntFile,46,0);
		fread(&pallete1,sizeof(int), 1, puntFile);
		
		fseek(puntFile,50,0);
		fread(&pallete2,sizeof(int), 1, puntFile);
		
		// lettura palette
		if (esistePalette){
			bool palette_standard = (offset == (14+header_size+DIM_PALETTE));
			if (palette_standard){
				cout << "Ok, palette standard \n";
				fseek(puntFile, DIM_PALETTE, 0);
				fread(paletteContent, DIM_PALETTE, 1, puntFile);				// legge la palette e la memorizza in paletteContent
			}
			else
				cout << "Attenzione: dimensione Palette diversa da "<<DIM_PALETTE<<endl;
		}
		
		fclose(puntFile);
	}

	int Header::getDimensioneTot(){					// get e set per le dimensioni e l'offset immagine
		return dimensione_totale;
	}
	
	int Header::getLarghezza(){
		return larghezza;
	}
	
	int Header::getAltezza(){
		return altezza;
	}
	
	int Header::getColor(){
		return color;
	}
	
	int Header::getOffsetIMG(){
		return offset;
	}
	
	void Header::setLarghezza(int newLarg){
		larghezza = newLarg;
		dimensione_totale = larghezza*altezza*color/8;				// aggiorno anche la dimensione_totale dell'immagine
	}
	
	void Header::setAltezza(int newAlt){
		altezza = newAlt;
		dimensione_totale = larghezza*altezza*color/8;				// aggiorno anche la dimensione_totale dell'immagine
	}

	void Header::setOffset(int newOffset){
		offset = newOffset;
	}
	
	void Header::setColor(int newColor){
		color = newColor;
	}
	
	void Header::stampaInfoHeader(){					// stampa nella console le informazioni dell'header
		cout<<"---------  Informazioni Header   --------\n";
		cout<<"Firma: "<<firma[0]<<firma[1]<<endl;
		cout<<"Dimensione totale: "<<dimensione_totale<<endl;
		cout<<"Reserved: "<<riservato<<endl;
		cout<<"Offset: "<<offset<<endl;
		cout<<"Header size: "<<header_size<<endl;
		cout<<"Larghezza: "<<larghezza<< " px"<<endl;
		cout<<"Altezza: "<<altezza<< " px"<<endl;
		cout<<"Piani: "<<piani[0]<<piani[1]<<endl;
		cout<<"Colori: "<<color <<" bit"<<endl;
		cout<<"Compressione: "<<compressione<<endl;
		cout<<"Dimensione immagine: "<<dimensione_immagine<<endl;
		cout<<"Risoluzione orizzontale: "<<ris_orizzontale<<endl;
		cout<<"Risoluzione verticale: "<<ris_verticale<<endl;
		cout<<"Pallette: "<<pallete1<<endl;
		cout<<"Pallette used: "<<pallete2<<endl;
		cout << "---------------------------------------\n";
	}
	
/**
 * Metodo che scrive lo Header in un file .bmp indicato dall'argomento
 * @param nomeFile: nome del file nel quale scrivere la bitMap
 * 
 * Il metodo riscrive l'intero header memorizzato alla lettura dentro la variabile headerContent, successivamente sovrascrive le informazioni relative a dimensioni, offset e color
 */
	void Header::scriviHeader(char* nomeFile){
	
		ofstream fileout;
		fileout.open(nomeFile, ios::out | ios::binary);
		long positionIniziale = fileout.tellp();
		
		streamsize dimensioneOut = header_size+14;
		fileout.write((const char*)headerContent, dimensioneOut);
		
		// riscrive i campi dimensione_totale, offset, larghezza e altezza nelle giuste posizioni
		fileout.seekp(positionIniziale+2);
		fileout.write(reinterpret_cast<const char *>(&dimensione_totale), sizeof(dimensione_totale));
		
		fileout.seekp(positionIniziale+10);
		fileout.write(reinterpret_cast<const char *>(&offset), sizeof(offset));

		fileout.seekp(positionIniziale+18);
		fileout.write(reinterpret_cast<const char *>(&larghezza), sizeof(larghezza));

		fileout.seekp(positionIniziale+22);
		fileout.write(reinterpret_cast<const char *>(&altezza), sizeof(altezza));
		
		fileout.seekp(positionIniziale+28);
		fileout.write(reinterpret_cast<const char *>(&color), sizeof(color));
		
		if (esistePalette){
			dimensioneOut = DIM_PALETTE;
			fileout.write((const char*)paletteContent, dimensioneOut);
		  }
		fileout.close();
	}