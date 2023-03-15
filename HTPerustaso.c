
 
#include "HTPerusKirjasto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){


int valinta;
char nimi[30];
DATA* pAlku = NULL;
TAULUKKO *pTaulukko = NULL;
TILASTO tilastot[12];
do {

valinta = valikko();

switch (valinta) {
    
    case 1:
        
        kysyTiedosto(valinta, nimi);
        pAlku=vapautaMuisti(pAlku);
        pAlku = lueTiedosto(nimi, pAlku);

        break;

    case 2:

        if(pAlku==NULL){
            printf("Ei analysoitavaa, lue tiedosto ennen analyysiä.\n");
            printf("\n");
        } else {

            free(pTaulukko);
            pTaulukko = dataAnalyysi(pAlku, pTaulukko);
            tilastoAnalyysi(pAlku, tilastot);
        }
        break;
        

    case 3:
        
        if(pTaulukko==NULL){
            printf("Ei kirjoitettavia tietoja, analysoi tiedot ennen tallennusta.\n\n");
        } else {
        
        kysyTiedosto(valinta, nimi);
        kirjoitaTiedosto(pTaulukko, tilastot, nimi);
        }
        break;
        

    case 0:
        printf("\n");
        pAlku = vapautaMuisti(pAlku);
        
        break;
    
    default:
        printf("Tuntematon valinta, yritä uudestaan.\n\n");
        break;
}

} while (valinta!=0);
free(pTaulukko);
pAlku = vapautaMuisti(pAlku);
printf("Kiitos ohjelman käytöstä.\n");
return 0;
}

/* eof */

