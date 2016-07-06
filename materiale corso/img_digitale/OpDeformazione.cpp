/*
 * Operatori Puntuali/Locali
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

// operatori

int LeggiPixelSorgente(float x, float y);
int LeggiInterpolato(float x, float y);
void Deforma();

int main(void)
{
	CaricaBmp("/home/alien/Immagini/img400x300.bmp", ImmagineS1, SX, SY);

	Deforma();

	SalvaBmp("/home/alien/Immagini/output.bmp", ImmagineD, DX, DY);

	return 0;

}

// Implementazione

int LeggiPixelSorgente(float x, float y)
{
	int X = (int) x;
	int Y = (int) y;

	if(X < 0 || X >= SX || Y < 0 || Y >= SY) return(0);
	//if(X < 0) X = 0;
	//if(X >= SX) X = SX - 1;
	//if(Y < 0) Y = 0;
	//if(Y >= SY) Y = SY - 1;

	return(ImmagineS1[X + (SX * Y)]);
}

void Deforma()
{
	int x, y;
	int l;
	float a = 0.2f;
	float u, v;

	for(y = 0;y < DY;y++) {
		for(x = 0;x < DX;x++) {
			u = 0.7f * x;
			v = 0.7f * y;
			l = LeggiInterpolato(u, v);
			if(l > 255) l = 255;
			if(l < 0) l = 0;
			ImmagineD[x + (y * DX)] = l;
		}
	}

}

int LeggiInterpolato(float x, float y)
{
	float v1, v2, v3, v4;
	int X = (int) x;
	int Y = (int) y;

	x -= X;
	y -= Y;

	if(X < 0) X = 0;
	if(X >= SX - 1) X = SX - 1;
	if(Y < 0) Y = 0;
	if(Y >= SY - 1) Y = SY - 1;

	v1 = ImmagineS1[X + (SX * Y)];
	v2 = ImmagineS1[X + 1 + (SX * Y)];
	v3 = ImmagineS1[X + (SX * (Y + 1))];
	v4 = ImmagineS1[X + 1 + (SX * (Y + 1))];

	return( (v1 * (1 - x) * (1 - y)) +
			(v2 * x * (1 - y)) +
			(v3 * (1 - x) * y) +
			(v4 * x * y) );
}
