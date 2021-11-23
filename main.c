#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include <string.h>

typedef struct Esemeny{
    int ev, ho, nap;
    int ora, perc;
    char* helyszin;
    char* nev ;
    char* megj;
    struct Esemeny* kov;
}Esemeny;

void esemeny_init(Esemeny *p) {
    p -> ev = 0;
    p -> ho = 0;
    p -> nap = 0;
    p -> helyszin = NULL;
    p -> nev = NULL;
    p -> megj = NULL;
    p -> kov = NULL;
}

void esemeny_destroy(Esemeny *e) {
    if(e -> helyszin != NULL)
        free(e -> helyszin);
    if(e -> nev != NULL)
        free(e -> nev);
    if(e -> megj != NULL)
        free(e -> megj);
    free(e);
}

int fomenu() {
    int valasz = -1;
    printf("1) Uj esemeny letrehozasa\n");
    printf("2) Esemeny nev szerinti keresese\n");
    printf("3) Intervallum szerinti listazas\n");
    printf("4) Adatbazis fajlba mentese\n");
    printf("5) Adatbazis visszatoltese fajlbol\n");
    printf("6) Teljes kiiras\n");
    printf("0) Kilepes\n");
    printf("Adja meg a valaszat: ");
    int bemenet = scanf("%d", &valasz);
    while( valasz < 0 || valasz > 6 || bemenet == 0) {
        printf("Helytelen ertek!\n");
        printf("Adja meg a valaszat: ");
        fflush(stdin);
        bemenet = scanf("%d", &valasz);
    }

    return valasz;
}


void esemeny_kiir(Esemeny *e) {
    printf("%4d.%02d.%02d. %02d:%02d", e -> ev, e -> ho, e -> nap, e -> ora, e -> perc);
    if(e -> helyszin != NULL)
        printf(", Helyszin: %s", e -> helyszin);
    if(e -> nev != NULL)
        printf(", Nev: %s", e -> nev);
    if(e -> megj != NULL)
        printf(", Megjegyzes: %s", e -> megj);
    printf("\n");
}

char *beolvas() {
    char *tomb;
    int i = 0;
    char be;
    tomb = (char*) malloc(sizeof(char));
    if(tomb != NULL) {
        be = getc(stdin);
        while(be != '\n') {
            tomb[i++] = be;
            tomb = (char*) realloc(tomb, (i + 1) * sizeof(char));
            be = getc(stdin);
        }
        tomb[i] = '\0';
    }
    else printf("Sikertelen foglalas!\n");
    if(tomb[0] == '\0') {
        free(tomb);
        return NULL;
    }
    return tomb;
}


void esemeny_beolvas(Esemeny *mibe) {
    //esemeny_init(mibe);
    Esemeny e;
    printf("Datum, idopont (EEEE.HH.NN. oo:pp): ");
    scanf("%d.%d.%d. %d:%d", &e.ev, &e.ho, &e.nap, &e.ora, &e.perc);
    mibe -> ev = e.ev;
    mibe -> ho = e.ho;
    mibe -> nap = e.nap;
    mibe -> ora = e.ora;
    mibe -> perc = e.perc;
    fflush(stdin);
    printf("Helyszin(opcionalis): ");
    e.helyszin = beolvas();
    if (mibe -> helyszin != NULL)
      free(mibe -> helyszin);
    mibe -> helyszin = e.helyszin;

    printf("Elnevezes(opcionalis): ");
    e.nev = beolvas();
    if (mibe -> nev != NULL)
      free(mibe -> nev);
    mibe -> nev = e.nev;

    printf("Megjegyzes(opcionalis): ");
    e.megj = beolvas();
    if (mibe -> megj != NULL)
      free(mibe -> megj);
    mibe -> megj = e.megj;

}

char *dinamikus_beolvas(FILE *fp) {


}

//uj esemeny
Esemeny *hozzafuz(Esemeny* lista) {
    printf("Uj esemeny letrehozasa\n\n");
    Esemeny *uj = (Esemeny*) malloc(sizeof(Esemeny));
    esemeny_init(uj);
    esemeny_beolvas(uj);
    if(lista == NULL)
        return uj;

    Esemeny *mozgo = lista;
    while(mozgo -> kov != NULL)
        mozgo = mozgo -> kov;
    mozgo -> kov = uj;
    return lista;
}

/*Esemeny *fajlbol_beolvas(Esemeny* eleje, FILE *f) {
    Esemeny *mozgo = eleje;
    while(!EOF) {
        esemeny_init(mozgo);

    }
}
*/

//dinamikus tombbe tolti a talalatok indexeit

int* keres(Esemeny *eleje, char *mit) {
    int meret = 0;
    int *talalatok = (int*) malloc(sizeof(int));
    Esemeny *mozgo = eleje;

    for(int i = 0; mozgo != NULL; i ++) {
        if(strcmp(mozgo -> nev, mit) == 0) {
            talalatok[meret ++] = i;
            talalatok = (int*) realloc(talalatok, (meret + 1) * sizeof(int));
        }
        mozgo = mozgo -> kov;
    }

    if(meret == 0) {
        free(talalatok);
        return NULL;
    }

    talalatok[meret] = -1;
    return talalatok;
}

int talalat_kiir(Esemeny *eleje, int *tomb) {
    if (tomb == NULL) {
        printf("Nincs talalat!\n");
        return 0;
    }
    Esemeny *mozgo = eleje;
    if(tomb[1] == -1) {
        for(int i = 0; i < tomb[0]; i ++)
            mozgo = mozgo -> kov;
        printf("Talalat:\n");
        esemeny_kiir(mozgo);
        return 1;
    }


    int j = 0;
    for(int i = 0; mozgo != NULL && tomb[j] != -1; i ++) {
        if(i == tomb[j]) {
            j ++;
            printf("%d) ", j);
            esemeny_kiir(mozgo);
        }
        mozgo = mozgo -> kov;
    }
    return j;
}



/*
int intervallum_valaszt() {

}

void nap() {

}

void honap() {

}

void het() {

}
*/

void teljeskiir(Esemeny *lista) {
    if(lista == NULL) {
      printf("Ures lista!\n");
      return;
    }

    Esemeny *mozgo = lista;
    while(mozgo != NULL) {
      esemeny_kiir(mozgo);
      mozgo = mozgo -> kov;
    }
}

void felszabadit(Esemeny *lista) {
    Esemeny *felsz = lista;
    while(felsz != NULL) {
        Esemeny *next = felsz -> kov;
        esemeny_destroy(felsz);
        felsz = next;
    }
}

int main()
{
    Esemeny* naplo = (Esemeny*) malloc(sizeof(Esemeny)) ;
    Esemeny* mozgo, *seged;
    free(naplo);
    esemeny_init(naplo);
    naplo = NULL;
    FILE *fp;


    char *mitkeres;

    int *talalat;
    int i, n, db = 0, talalatvalaszt, naplomeret = 0;
    int v = fomenu();
    fflush(stdin);
    while(v != 0) {
        switch(v) {
            case 1:
                system("cls");
                naplo = hozzafuz(naplo);
                naplomeret ++;
                break;
            case 2:
                system("cls");
                printf("Adja meg a keresett esemeny nevet: ");

                fflush(stdin);

                mitkeres = beolvas();

                system("cls");

                talalat = keres(naplo, mitkeres);

                db = talalat_kiir(naplo, talalat);

                if(db > 1) {
                    printf("\nTalalat kivalasztasa: ");
                    scanf("%d", &talalatvalaszt);
                }
                talalatvalaszt = talalat[talalatvalaszt - 1];

                printf("Opciok:\n");
                printf("1) Modositas\n");
                printf("2) Torles\n");
                printf("0) Vissza\n");

                scanf("%d", &n);

                switch(n) {
                    case 0:
                        break;
                    case 1:
                        mozgo = naplo;
                        for(i = 0; i < talalatvalaszt; i ++)
                            mozgo = mozgo -> kov;

                        esemeny_beolvas(mozgo);
                        free(seged);
                        break;
                    case 2:
                        if(naplomeret == 1) {
                          felszabadit(naplo);
                          break;
                        }
                        mozgo = naplo;
                        for(int i = 0; i < talalatvalaszt - 1; i ++)
                          mozgo = mozgo -> kov;
                        if(naplomeret - 1 == talalatvalaszt) {
                          felszabadit(mozgo -> kov);
                          mozgo -> kov = NULL;
                          break;
                        }
                        mozgo -> kov = mozgo -> kov -> kov;
                        felszabadit(mozgo -> kov);
                        break;
                    default:
                        break;
                }
                free(talalat);
                free(mitkeres);
                break;
            case 3:
                //intervallum_valaszt();
                break;
            case 4:
                //fajla ment
                break;
            case 5:
                //fajlbol visszat�lt
                break;
              case 6:
                system("cls");
                teljeskiir(naplo);
                n = 0;
                scanf("%d", &n);
                break;
        }
        fflush(stdin);
        system("cls");
        v = fomenu();
    }
    mozgo = naplo;
    while(mozgo != NULL) {
        esemeny_kiir(mozgo);
        mozgo = mozgo -> kov;
    }
    felszabadit(naplo);
    return 0;
}
