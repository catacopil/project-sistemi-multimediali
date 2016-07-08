#include <stdio.h>
#include <stdlib.h>

// Variabili globali parametri BMP
int iOffSet, i_w, i_h, headers_size,nBit;
unsigned char Palette[1024]; // 4 byte x 256 colori

void bin8(int n, char bina[])
{
     int i;
     for (i=0 ; i < 8 ; i++)
     {
         bina[7-i]=(n % 2)+'0';
         n = (n - (n % 2) ) /2;
     }
}

void DumpFile(char *Nf)
{
	unsigned char BitMap[64000L];
	char dig[3], bina[8];
    unsigned char d;
    int LungRow, nByte;
    int n=0, b=0,l;
    // Dimensione riga: multiplo di 4 byte
    LungRow=( (i_w*nBit)+(32-(i_w*nBit)%32) )/8;
	FILE *fpo;
    if ( (fpo=fopen(Nf,"rb"))==NULL )
    {
         printf("File non trovato!!");
         return;
    }
    fseek(fpo,iOffSet,0);
    printf("---------------------------------------------\n");
    printf("SEZIONE --> Bitmap data dell'immagine\n");
    printf("---------------------------------------------\n");
    nByte=nBit/8;
    if (nByte<1) nByte=1;
		//  da notare che fread legge un array di elementi dal file puntato da fpo 
    l=fread(BitMap, sizeof(unsigned char), LungRow, fpo);
    while (l == LungRow)
    {
        for ( n=0 ; n < LungRow ; n++)
        {
            if ( (nBit==24) || (nBit==8) )
            {
                if (b==0) printf("[");
                printf("%02X",BitMap[n]);
                if (b==(nByte-1)) printf("]");
                b=(b+1) % nByte;
            }
            else if (nBit==4)
            {
                sprintf(dig, "%02X", BitMap[n]);
                printf("[%c] [%c] ",dig[0],dig[1]);
            }
            else if (nBit==1)
            {
                l=(int)BitMap[n];
                bin8(l,bina);
                printf("[%s]",bina);
            }
        }
        printf("\n");
        l=fread(BitMap, sizeof(unsigned char), LungRow, fpo);
    }
    printf("------------------------------------------------");
    fclose(fpo);
}

void InfoBMP(char *Nf)
{
    int n, j;
    FILE *fpo;

    fpo=fopen(Nf,"rb");
    printf("################################################\n");
    printf("# File: %s\n",Nf);
    printf("############################### vers. 1.1 ######\n");

    // Variabili per leggere
    int int_var;
    short short_var;
    float float_var;
    char char_2[2];
    
    printf("---------------------------------------------\n");
    printf("SEZIONE --> Titolo del File\n");
    printf("---------------------------------------------\n");
    fseek(fpo,0,0);
    n = fread(char_2,2, 1, fpo);
    printf(" - Firma                      : %c%c\n",char_2[0],char_2[1]);
    fseek(fpo,2,0);
    fread(&int_var, 1, sizeof(int), fpo);
    printf(" - Dimensione File            : %d\n",int_var);
    fseek(fpo,10,0);
    fread(&int_var, 1, sizeof(int), fpo);
    printf(" - Offset                     : %d\n",int_var);
    iOffSet=int_var;

    printf("---------------------------------------------\n");
    printf("SEZIONE --> Titolo dell'immagine\n");
    printf("---------------------------------------------\n");
    fseek(fpo,14,0);
    n = fread(&int_var,sizeof(int), 1, fpo);
    printf(" - Dimensione Header Immagine : %d\n",int_var);
    headers_size=int_var+14;
    n = fread(&int_var,sizeof(int), 1, fpo);
    printf(" - Larghezza                  : %d\n",int_var);
    i_w=int_var;
    n = fread(&int_var,sizeof(int), 1, fpo);
    printf(" - Altezza                    : %d\n",int_var);
    i_h=int_var;
    fseek(fpo,28,0);
    n = fread(&short_var,sizeof(short), 1, fpo);
    printf(" - nr. bit per colore         : %hd\n",short_var);
    nBit=short_var;
    n = fread(&int_var,sizeof(int), 1, fpo);
    if (int_var==0)
       printf(" - tipo compressione          : Nessuna\n");
    else if (int_var==1)
       printf(" - tipo compressione          : RLE 8 Bit\n");
    else if (int_var==2)
       printf(" - tipo compressione          : RLE 4 Bit\n");
    else if (int_var==3)
       printf(" - tipo compressione          : Bitfields\n");
    else
       printf(" - tipo compressione          : %d\n",int_var);
    fread(&int_var, 1, sizeof(int), fpo);
    printf(" - bytes immagine senza header: %d\n",int_var);
    printf("---------------------------------------------\n");
    printf("SEZIONE --> Palette dell'immagine\n");
    printf("---------------------------------------------\n");
    if (headers_size==iOffSet)
       printf(" - Palette                     : Non presente\n");
    else
    {
        fseek(fpo,headers_size,0);
        fread(Palette, sizeof(unsigned char), iOffSet-headers_size, fpo);
        for (n=0, j=0 ; n <( (iOffSet-headers_size)/4 ); n++)
        {
           printf("%02X^col. :%02lX",n, Palette[j++]);
           printf("%02lX",Palette[j++]);
           printf("%02lX | ",Palette[j++]);
           if ( ( (n+1) % 4)==0) printf("\n");
           j++;
        }
    }
    printf("\n");
    fclose(fpo);
    DumpFile(Nf);
}

int main(int argc, char *argv[])
{
   nBit=atoi(argv[1]);
   if (nBit==1)
      InfoBMP("bmp7x7_01bit.bmp");
   else if (nBit==4)
      InfoBMP("bmp7x7_04bit.bmp");
   else if (nBit==8)
      InfoBMP("bmp7x7_08bit.bmp");
   else if (nBit==24)
      InfoBMP("bmp7x7_24bit.bmp");
   else
      printf("Numeri ammessi: 1, 4, 8, 24");
}
