/*
 * FFT su img a colori
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// immagine(i) sorgente

#define LARG	512
#define LP2		9

unsigned char ColoriS[LARG * LARG][3];

// immagine destinazione

unsigned char ColoriD[LARG * LARG][3];

// buffer x FFT

float Buffer[LARG * LARG][2];
float Temp[LARG * LARG][2];
float RigaR[LARG];
float RigaI[LARG];

// operatori

void FFT(int dir,int m,float *x,float *y);

void SorgenteInBuffer(int canale);
void BufferInDestinazione(int canale);
void ModuloInDestinazione();
void TrasfBufferFourier(int Direzione);

void FiltroPassaBasso(int);

// header per file BitMap a colori

#define DIM_HEAD_BMP24 54

unsigned char Header[DIM_HEAD_BMP24];

static void CaricaBmp24(const char *Nome, unsigned char *Dove, int x, int y)
{
	FILE *fHan = fopen(Nome, "rb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fread(Header, DIM_HEAD_BMP24, 1, fHan);
	fread(Dove, x * y * 3, 1, fHan);
	fclose(fHan);
}

static void SalvaBmp24(const char *Nome, unsigned char *DaDove, int x, int y)
{
	FILE *fHan = fopen(Nome, "wb");
	if(fHan == NULL) {
		printf("errore!\n");
		exit(1);
	}

	fwrite(Header, DIM_HEAD_BMP24, 1, fHan);
	fwrite(DaDove, x * y * 3, 1, fHan);
	fclose(fHan);
}

// copia il canale i-esimo dal/al buffer

void SorgenteInBuffer(int c)
{
	int x, y;
	float R;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			R = ColoriS[x + (y * LARG)][c];
			if(((x + y) & 1) == 0) Buffer[x + (y * LARG)][0] = R;
			else Buffer[x + (y * LARG)][0] = -R;
			Buffer[x + (y * LARG)][1] = 0;
		}
	}

}

void BufferInDestinazione(int c)
{
	int x, y;
	int u;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			if(((x + y) & 1) == 0) u = Buffer[x + (y * LARG)][0];
			else u = -Buffer[x + (y * LARG)][0];
			if(u < 0) u = 0;
			if(u > 255) u = 255;
			ColoriD[x + (y * LARG)][c] = u;
		}
	}

}

// trasf MONOCROMATICA di F

void TrasfBufferFourier(int Dir)
{
	int x, y;

	// parte a: trasformo le righe
	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			RigaR[x] = Buffer[x + (y * LARG)][0];
			RigaI[x] = Buffer[x + (y * LARG)][1];
		}
		FFT(Dir, LP2, RigaR, RigaI);
		for(x = 0;x < LARG;x++) {
			Temp[x + (y * LARG)][0] = RigaR[x];
			Temp[x + (y * LARG)][1] = RigaI[x];
		}
	}

	// parte b: le colonne
	for(x = 0;x < LARG;x++) {
		for(y = 0;y < LARG;y++) {
			RigaR[y] = Temp[x + (y * LARG)][0];
			RigaI[y] = Temp[x + (y * LARG)][1];
		}
		FFT(Dir, LP2, RigaR, RigaI);
		for(y = 0;y < LARG;y++) {
			Buffer[x + (y * LARG)][0] = RigaR[y];
			Buffer[x + (y * LARG)][1] = RigaI[y];
		}
	}

}

void FiltroPassaBasso(int Liv)
{
	int x, y;
	int dx, dy;
	float k;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			dx = x - (LARG / 2);
			dy = y - (LARG / 2);
			k = 0.1f / (0.1f + sqrt(dx * dx + dy * dy) / Liv);
			Buffer[x + (y * LARG)][0] *= k;
			Buffer[x + (y * LARG)][1] *= k;
		}
	}
}

int main(void)
{
	int c;

	CaricaBmp24("/home/alien/Immagini/kimi512.bmp", &ColoriS[0][0], LARG, LARG);

	for(c = 0;c < 3;c++) {
		SorgenteInBuffer(c);
		TrasfBufferFourier(1);
		FiltroPassaBasso(200);
		TrasfBufferFourier(-1);
		BufferInDestinazione(c);
	}

	SalvaBmp24("/home/alien/Immagini/output.bmp", &ColoriD[0][0], LARG, LARG);

	return 0;

}
