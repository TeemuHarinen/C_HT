#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data{

    char aikaleima[30];
    int iViikko;
    int iKulutus;
    int iAurinkovoima;
    int iTuulivoima;
    int iVesivoima;
    int iYdinvoima;
    int iYhteistuotanto;
    int iLampovoima;
    struct data *pSeuraava;

}DATA;

typedef struct tilasto{

    int iKuukausi;
    double iTuotanto;

}TILASTO;

typedef struct taulukko{

    long iYhteis;
    int iKulutus;
    int iMittaukset;
    int iPienin;
    int iSuurin;
    char sKulutus[30];
    char pKulutus[30];
    double keskiarvo;

}TAULUKKO;

int valikko();
DATA* lueTiedosto(char nimi[], DATA* pAlku);
char kysyTiedosto(int valinta, char nimi[]);
DATA* varaaMuisti(DATA* pAlku, char rivi[]);
TAULUKKO* dataAnalyysi(DATA *pAlku, TAULUKKO *pTaulukko);
TILASTO tilastoAnalyysi(DATA* pAlku, TILASTO tilastot[]);
void kirjoitaTiedosto(TAULUKKO* pTaulukko, TILASTO tilastot[], char nimi[]);
void tulostus(FILE* tiedosto, TAULUKKO* pTaulukko, TILASTO tilastot[]);
DATA* vapautaMuisti(DATA *pAlku);

