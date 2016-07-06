/*
 * Operatori FFT
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LARG	512
#define LP2		9

// immagine(i) sorgente

unsigned char ImmagineS[LARG * LARG];

// immagine destinazione

unsigned char ImmagineD[LARG * LARG];

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


// buffer x FFT

float Buffer[LARG * LARG][2];
float Temp[LARG * LARG][2];
float RigaR[LARG];
float RigaI[LARG];

// operatori

void FFT(int dir,int m,float *x,float *y);

void SorgenteInBuffer();
void BufferInDestinazione();
void ModuloInDestinazione();
void TrasfBufferFourier(int Direzione);

void FiltroPassaAlto(int);
void FiltroPassaBasso(int);
void FiltroEsaltaAlto(int);

float Campana(int x, int y, int Liv);


void SorgenteInBuffer()
{
	int x, y;
	float R;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			R = ImmagineS[x + (y * LARG)];
			if(((x + y) & 1) == 0) Buffer[x + (y * LARG)][0] = R;
			else Buffer[x + (y * LARG)][0] = -R;
			Buffer[x + (y * LARG)][1] = 0;
		}
	}

}

void BufferInDestinazione()
{
	int x, y;
	int u;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			if(((x + y) & 1) == 0) u = Buffer[x + (y * LARG)][0];
			else u = -Buffer[x + (y * LARG)][0];
			if(u < 0) u = 0;
			if(u > 255) u = 255;
			ImmagineD[x + (y * LARG)] = u;
		}
	}

}

void ModuloInDestinazione()
{
	int i;
	float V;
	//float Max = 0;

	//for(i = 0;i < LARG * LARG;i++) {
	//	V = sqrt((Buffer[i][1] * Buffer[i][1]) + (Buffer[i][0] * Buffer[i][0]));
	//	if(V > Max) Max = V;
	//}

	//printf("Max %f\n", Max);

	for(i = 0;i < LARG * LARG;i++) {
		V = sqrt((Buffer[i][1] * Buffer[i][1]) + (Buffer[i][0] * Buffer[i][0]));
		V = 500.0f * log(V);
		if(V > 255) V = 255;
		if(V < 0) V = 0;
		ImmagineD[i] = V;
	}

}

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

	//printf("Trasformata\n");

}

// filtri

void FiltroPassaAlto(int Liv)
{
	int x, y;
	int dx, dy;
	float k;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			dx = x - (LARG / 2);
			dy = y - (LARG / 2);

			k = 1.0f - (1.0f / (1.0f + (sqrt(dx * dx + dy * dy) / Liv)));
			Buffer[x + (y * LARG)][0] *= k;
			Buffer[x + (y * LARG)][1] *= k;
		}
	}

}

void FiltroSharpen(int Liv)
{
	int x, y;
	int dx, dy;
	float k;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			dx = x - (LARG / 2);
			dy = y - (LARG / 2);

			k = 3.0f - (2.0f / (1.0f + (sqrt(dx * dx + dy * dy) / Liv)));
			Buffer[x + (y * LARG)][0] *= k;
			Buffer[x + (y * LARG)][1] *= k;
		}
	}

}


void FiltroPassaBasso(int Liv)
{
	int x, y;
	int dx, dy;
	//float l;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
		/*
			l = Campana(x, y, Liv);
			Buffer[x + (y * LARG)][0] *= l;
			Buffer[x + (y * LARG)][1] *= l;
		*/

			dx = x - (LARG / 2);
			dy = y - (LARG / 2);
			if(sqrt(dx * dx + dy * dy) > Liv) {
				Buffer[x + (y * LARG)][0] = 0;
				Buffer[x + (y * LARG)][1] = 0;
			}

		}
	}

}

void FiltroEsaltaAlto(int Liv)
{
	int x, y;
	//int dx, dy;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			Buffer[x + (y * LARG)][0] *= 3 - 2 * Campana(x, y, Liv);
			Buffer[x + (y * LARG)][1] *= 3 - 2 * Campana(x, y, Liv);
		}
	}

}

float Campana(int x, int y, int Liv)
{
	float d;

	x -= LARG / 2;
	y -= LARG / 2;

	d = sqrt(x * x + y * y);

	return(exp(-d / Liv));
}


// main

int main(void)
{

	CaricaBmp("/home/alien/Immagini/dist512.bmp", ImmagineS, LARG, LARG);

	SorgenteInBuffer();
	TrasfBufferFourier(1);

	//FiltroSharpen(64);
	FiltroPassaBasso(230); //Dist
	//FiltroPassaBasso(32);

	TrasfBufferFourier(-1);
	BufferInDestinazione();

	//ModuloInDestinazione();

	SalvaBmp("/home/alien/Immagini/output.bmp", ImmagineD, LARG, LARG);

	return 0;

}

