/*
 * Esempio di calcolo della DCT separabile
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LARG 256

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

// buffer x DCT

float CoeffCos[4 * LARG];

float Buffer[LARG * LARG];
float Temp[LARG * LARG];

// operatori

void SorgenteInBuffer();
void BufferInDestinazione();
void ModuloInDestinazione();
void TrasfBufferCos(int Direzione);

void FiltroPassaAlto(int);
void FiltroPassaBasso(int);

void SorgenteInBuffer()
{
	int x, y;
	float R;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			R = ImmagineS[x + (y * LARG)];
			//if(((x + y) & 2) == 0) Buffer[x + (y * LARG)] = R;
			//else Buffer[x + (y * LARG)] = -R;
			Buffer[x + (y * LARG)] = R;
		}
	}

}

void BufferInDestinazione()
{
	int x, y;
	int u;

	for(y = 0;y < LARG;y++) {
		for(x = 0;x < LARG;x++) {
			//if(((x + y) & 2) == 0) u = Buffer[x + (y * LARG)];
			//else u = -Buffer[x + (y * LARG)];
			u = Buffer[x + (y * LARG)];
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
	float Max = 0;

	for(i = 0;i < LARG * LARG;i++) {
		V = Buffer[i];
		if(V > Max) Max = V;
		if(-V > Max) Max = -V;
	}

	printf("Max %f\n", Max);

	for(i = 0;i < LARG * LARG;i++) {
		V = Buffer[i];
		if(V < 0) V = -V;
		if(V > 255) V = 255;
		if(V < 0) V = 0;
		ImmagineD[i] = V;
	}

}

void TrasfBufferCos(int Dir)
{
	int x, y;
	int u, v;
	int i;
	float R;

	printf("Creo Tavola Coseni\n");

	for(x = 0;x < 4 * LARG;x++) {
		CoeffCos[x] = cos(x * 6.2831853 / (4 * LARG));
	}

	// normalizzazione

	if(Dir <= 0) {
		for(y = 0;y < LARG;y++) {
			for(x = 0;x < LARG;x++) {
				if(x == 0) Buffer[x + (y * LARG)] *= sqrt(1.0f / LARG);
				else Buffer[x + (y * LARG)] *= sqrt(2.0f / LARG);
				if(y == 0) Buffer[x + (y * LARG)] *= sqrt(1.0f / LARG);
				else Buffer[x + (y * LARG)] *= sqrt(2.0f / LARG);
			}
		}
	}

	// parte a: trasformo righe

	for(y = 0;y < LARG;y++) {

		for(u = 0;u < LARG;u++) {
			R = 0.0f;
			for(x = 0;x < LARG;x++) {
				if(Dir > 0) i = (((2 * x) + 1) * u) % (4 * LARG);
				else i = (((2 * u) + 1) * x) % (4 * LARG);
				R += Buffer[x + (y * LARG)] * CoeffCos[i];
			}
			Temp[u + (y * LARG)] = R;
		}
	}

	printf("Trasformate Righe\n");

	// parte b: trasformo le colonne

	for(x = 0;x < LARG;x++) {

		for(v = 0;v < LARG;v++) {
			R = 0.0f;
			for(y = 0;y < LARG;y++) {
				if(Dir > 0) i = (((2 * y) + 1) * v) % (4 * LARG);
				else i = (((2 * v) + 1) * y) % (4 * LARG);
				R += Temp[x + (y * LARG)] * CoeffCos[i];
			}
			Buffer[x + (v * LARG)] = R;
		}
	}

	printf("Trasformate Colonne\n");

	// parte c: eventuale normalizzazione

	if(Dir > 0) {
		for(y = 0;y < LARG;y++) {
			for(x = 0;x < LARG;x++) {
				if(x == 0) Buffer[x + (y * LARG)] *= sqrt(1.0f / LARG);
				else Buffer[x + (y * LARG)] *= sqrt(2.0f / LARG);
				if(y == 0) Buffer[x + (y * LARG)] *= sqrt(1.0f / LARG);
				else Buffer[x + (y * LARG)] *= sqrt(2.0f / LARG);
			}
		}
	}

}

void EliminaCoefficienti(float Livello);

void EliminaCoefficienti(float Livello)
{
	int i, ct = 0;

	for(i = 0;i < LARG * LARG;i++) {
		if(Buffer[i] < Livello && Buffer[i] > -Livello) {
			Buffer[i] = 0;
			ct++;
		}
	}

	printf("Eliminati %d coeff su %d (compr %f)\n", ct, LARG * LARG, (float) ct / (float) (LARG * LARG));
}

// main

int main(void)
{

	CaricaBmp("/home/alien/Immagini/img256.bmp", ImmagineS, LARG, LARG);

	SorgenteInBuffer();

	TrasfBufferCos(1);

	EliminaCoefficienti(64.0f);	// 8.0f

	TrasfBufferCos(-1);

	BufferInDestinazione();

	SalvaBmp("/home/alien/Immagini/output.bmp", ImmagineD, LARG, LARG);

	printf("Finito!\n");

	return 0;

}

