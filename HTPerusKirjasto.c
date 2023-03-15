#include "HTPerusKirjasto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int valikko(){
    int valinta;

    printf("Valitse haluamasi toiminto:\n");
    printf("1) Lue tiedosto\n");
    printf("2) Analysoi tiedot\n");
    printf("3) Kirjoita tulokset\n");
    printf("0) Lopeta\n");
    printf("Anna valintasi: ");
    scanf("%d", &valinta);
    getchar();
    return valinta;
}

DATA* lueTiedosto(char nimi[30], DATA* pAlku){

    FILE *tiedosto;
    tiedosto = fopen(nimi, "r");
    char rivi[255];

    if(tiedosto == NULL){
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }
    
    fgets(rivi, 255, tiedosto);
    //vika merkki nullikaks
    while (fgets(rivi, 255, tiedosto) != NULL){

        if((pAlku = varaaMuisti(pAlku, rivi)) == NULL){

            perror("Muistinvaraus epäonnistui.\n");
            exit(0);
        }
        
    }

    printf("Tiedosto '%s' luettu.\n\n", nimi);
    fclose(tiedosto);
    return (pAlku);
}

char kysyTiedosto(int valinta, char nimi[30]){

    if(valinta==1){
        printf("Anna luettavan tiedoston nimi: ");
    } 
    else if (valinta==3){
        printf("Anna kirjoitettavan tiedoston nimi: ");
    }
    
    scanf("%s", nimi);
    getchar();
    return (*nimi);

}

DATA* varaaMuisti(DATA* pAlku, char rivi[]){

    DATA *pUusi;
    DATA *ptr;
    DATA *pLoppu=NULL;
    pUusi=(DATA*)malloc(sizeof(DATA));

    if(pUusi==NULL){
        return NULL;
    }
    else {

        strcpy(pUusi->aikaleima, strtok(rivi, ";"));
        pUusi->iViikko=atoi(strtok(NULL, ";"));
        pUusi->iKulutus=atoi(strtok(NULL, ";"));
        pUusi->iAurinkovoima=atoi(strtok(NULL, ";"));
        pUusi->iTuulivoima=atoi(strtok(NULL, ";"));
        pUusi->iVesivoima=atoi(strtok(NULL, ";"));
        pUusi->iYdinvoima=atoi(strtok(NULL, ";"));
        pUusi->iYhteistuotanto=atoi(strtok(NULL, ";"));
        pUusi->iLampovoima=atoi(strtok(NULL, ";"));
        pUusi->pSeuraava=NULL;

    }

    if(pAlku==NULL){

        pAlku=pUusi;
        pLoppu=pUusi;

    } else {

        ptr = pAlku;
        while(ptr->pSeuraava != NULL){
            ptr=ptr->pSeuraava;
        }
        ptr->pSeuraava=pUusi;
    }
    return pAlku;
}

TAULUKKO* dataAnalyysi(DATA *pAlku, TAULUKKO *pTaulukko){

    DATA *ptr;
    ptr = pAlku;
    long apu = 0;

    if((pTaulukko=(TAULUKKO*)malloc(sizeof(TAULUKKO)))==NULL){
        perror("Muistin varaus epäonnistui.\n");
        exit(0);
    }

    pTaulukko->iYhteis=0;
    pTaulukko->iMittaukset=0;
    pTaulukko->iKulutus=0;
    pTaulukko->iPienin = ptr->iKulutus;
    pTaulukko->iSuurin = ptr->iKulutus;
    strcpy(pTaulukko->pKulutus, ptr->aikaleima);
    strcpy(pTaulukko->sKulutus, ptr->aikaleima);
    

    while(ptr != NULL){

        pTaulukko->iMittaukset++;
        apu += ptr->iKulutus;

        if(ptr->iKulutus < pTaulukko->iPienin){

            pTaulukko->iPienin = ptr->iKulutus;
            strcpy(pTaulukko->pKulutus, ptr->aikaleima);

        }
        if(ptr->iKulutus>pTaulukko->iSuurin){

            pTaulukko->iSuurin = ptr->iKulutus;
            strcpy(pTaulukko->sKulutus, ptr->aikaleima);

        }
        ptr=ptr->pSeuraava;
    }
    pTaulukko->iYhteis=apu;
    pTaulukko->keskiarvo=(double)pTaulukko->iYhteis/pTaulukko->iMittaukset;

    printf("Analysoitu %ld mittaustulosta.\n", pTaulukko->iMittaukset);
    printf("Kokonaiskulutus oli yhteensä %ld kWh.\n", pTaulukko->iYhteis);
    printf("Kuukausittaiset tuotannot analysoitu.\n");
    printf("\n");

    return pTaulukko;
    
}

TILASTO tilastoAnalyysi(DATA* pAlku, TILASTO tilastot[]){
    int i, iOsoitin;
    char *osoitin;
    char aOsoitin[30];
    DATA* ptr;
    ptr = pAlku;
    
    
    for(i=0; i<12; i++){
        tilastot[i].iKuukausi=i+1;
        tilastot[i].iTuotanto=0;
    }
    while(ptr!=NULL){
        strcpy(aOsoitin, ptr->aikaleima);
        osoitin=strtok(aOsoitin, ".");
        osoitin=strtok(NULL, ".");
        iOsoitin=atoi(osoitin);
        tilastot[iOsoitin-1].iTuotanto += (ptr->iAurinkovoima + ptr->iTuulivoima + ptr->iVesivoima + ptr->iYdinvoima + ptr->iYhteistuotanto + ptr->iLampovoima);
        ptr=ptr->pSeuraava;
    }
    return *tilastot;
}

void kirjoitaTiedosto(TAULUKKO* pTaulukko, TILASTO tilastot[], char nimi[]){

    tulostus(stdout, pTaulukko, tilastot);
    FILE* tiedosto;
    tiedosto=fopen(nimi, "w");

    if(tiedosto==NULL){
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }
    tulostus(tiedosto, pTaulukko, tilastot);

    fclose(tiedosto);
    printf("Tiedosto '%s' kirjoitettu.\n", nimi);
    printf("\n");
    return;
}

DATA* vapautaMuisti(DATA *pAlku){

    DATA *ptr;
    ptr=pAlku;

    while(ptr != NULL){

        pAlku=ptr->pSeuraava;
        free(ptr);
        ptr=pAlku;

    }
    return pAlku;
}

void tulostus(FILE* tiedosto, TAULUKKO* pTaulukko, TILASTO tilastot[]){

    int i;
    float gwh = 0;
    fprintf(tiedosto, "Tilastotiedot %ld mittaustuloksesta:\n", pTaulukko->iMittaukset);
    fprintf(tiedosto, "Kulutus oli yhteensä %ld kWh, ja keskimäärin %.1f kWh.\n", pTaulukko->iYhteis, pTaulukko->keskiarvo);
    fprintf(tiedosto, "Suurin kulutus, %d kWh, tapahtui %s.\n", pTaulukko->iSuurin, pTaulukko->sKulutus);
    fprintf(tiedosto, "Pienin kulutus, %d kWh, tapahtui %s.\n\n", pTaulukko->iPienin, pTaulukko->pKulutus);
    fprintf(tiedosto, "Pvm;Tuotanto (GWh)\n");

    for(i=0; i<12; i++){
        gwh=tilastot[i].iTuotanto/1000000;
        fprintf(tiedosto,"Kk %d;%4.2f\n", tilastot[i].iKuukausi, gwh);
    }
    return;
}
/* eof */

