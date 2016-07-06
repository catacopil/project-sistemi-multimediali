/*
 * Operatori Puntuali
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// immagine(i) sorgente

#define SX	400
#define SY	300

unsigned char ImmagineS1[SX * SY];

// immagine destinazione

#define DX	400
#define	DY	300

unsigned char ImmagineD[DX * DY];

// operatori

void AumentaLuminosita(int Scala);
void Gamma(float Esponente);
void Contrasto();

// header per file BitMap monocromatici

#define DIM_HEAD_BMP 1078

unsigned char Header[DIM_HEAD_BMP];

static void CaricaBmp(const char *Nome, unsigned char *Dove, int x, int y)
{
	FILE *fHan = fopen(Nome, "rb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fread(Header, DIM_HEAD_BMP, 1, fHan);
	fread(Dove, x * y, 1, fHan);
	fclose(fHan);
}

static void SalvaBmp(const char *Nome, unsigned char *DaDove, int x, int y)
{
	FILE *fHan = fopen(Nome, "wb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fwrite(Header, DIM_HEAD_BMP, 1, fHan);
	fwrite(DaDove, x * y, 1, fHan);
	fclose(fHan);
}


int main(void)
{
	CaricaBmp("/home/alien/Immagini/img400x300.bmp", ImmagineS1, SX, SY);

	Gamma(0.5f);
	//AumentaLuminosita(128);
	//Contrasto();

	SalvaBmp("/home/alien/Immagini/output.bmp", ImmagineD, DX, DY);

	return 0;

}

// Implementazione

void Contrasto()
{
	int i;
	int l;

	for(i = 0;i < DX * DY;i++) {

		l = ImmagineS1[i];

		l = (l * 255) / 120;
		l -= (255 * 80) / 120;

		if(l > 255) l = 255;
		if(l < 0) l = 0;

		ImmagineD[i] = l;
	}
}



void AumentaLuminosita(int Scala)
{
	int x, y;
	int v;

	for(y = 0;y < DY;y++) {
		for(x = 0;x < DX;x++) {
			v = ImmagineS1[x + (y * DX)];
			v = (v * Scala) / 255;
			if(v > 255) v = 255;
			if(v < 0) v = 0;
			ImmagineD[x + (y * DX)] = v;
		}
	}

}

void Gamma(float Esponente)
{
	int x, y;
	float v;

	for(y = 0;y < DY;y++) {
		for(x = 0;x < DX;x++) {
			v = ImmagineS1[x + (y * DX)];
			v = pow(v / 255.0f, Esponente);
			v *= 255.0f;
			if(v > 255) v = 255;
			if(v < 0) v = 0;
			ImmagineD[x + (y * DX)] = v;
		}
	}

}
