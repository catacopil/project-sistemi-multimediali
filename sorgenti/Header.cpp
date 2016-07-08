
#include <stdio.h>
#include <stdlib.h>

class Header{
private:
    unsigned char* headerContent;       // contiene tutti i byte dell'Header
    int dimHeaderTotale;
    
public:
    void Header(String nomeFile){
        FILE *puntFile;
        puntFile = fopen(nomeFile,"rb");
        
        fread(headerContent, 1, 54, puntFile);
        
        for(int i=0; i<54; i++)
        fprint(" %c",*(headerContent+i));
    }
}

TODO: fare classe Header con lettura/scrittura su file
    vedere il resize e applicarlo sul codice