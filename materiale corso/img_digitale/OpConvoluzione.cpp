/*
 * Operatori a convoluzione
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

// kernel

#define KD	3
#define OFS	((KD - 1) / 2)


int Kernel[KD * KD] = {
	0,	-2,	0,
	-2,	8,	-2,
	0,	-2,	0
};

/*
int Kernel[KD * KD] = {
	0,	0,	1,	0,	0,
	0,	1,	2,	1,	0,
	1,	2,	3,	2,	1,
	0,	1,	2,	1,	0,
	0,	0,	1,	0,	0
};
*/

int Scala = 1;

// operatori

int Pixel(int x, int y);
void Convoluzione();

int main(void)
{
	CaricaBmp("/home/alien/Immagini/img400x300.bmp", ImmagineS1, SX, SY);

	Convoluzione();

	SalvaBmp("/home/alien/Immagini/output.bmp", ImmagineD, DX, DY);

	return 0;

}

// Implementazione

int Pixel(int x, int y)
{
	int u, v;
	int a;
	int p = 0;

	for(v = -OFS;v <= OFS;v++) {
		if(y + v < 0 || y + v >= SY) continue;
		for(u = -OFS;u <= OFS;u++) {
			if(x + u < 0 || x + u >= SX) continue;

			a = ImmagineS1[x + u + ((y + v) * SX)];
			p += (a * Kernel[u + OFS + ((v + OFS) * KD)]);
		}
	}

	p /= Scala;

	if(p < 0) p = 0;
	if(p > 255) p = 255;

	return(p);
}

void Convoluzione()
{
	int x, y;

	for(y = 0;y < SY;y++) {
		for(x = 0;x < SX;x++) {
			ImmagineD[x + (y * DX)] = Pixel(x, y);
		}
	}

}
