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
    e = NULL;
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
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    while( (valasz < 0 || valasz > 6 || bemenet == 0) ) {
        printf("Helytelen ertek!\n");
        printf("Adja meg a valaszat: ");
        bemenet = scanf("%d", &valasz);
        while ((c = getchar()) != '\n' && c != EOF) { }
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

char *szovegfajlbeolvas(FILE *fp) {
    char *tomb;
    int i = 0;
    char be;
    tomb = (char*) malloc(sizeof(char));
    if(tomb != NULL) {
        be = fgetc(fp);
        while(be != '\n') {
            tomb[i++] = be;
            tomb = (char*) realloc(tomb, (i + 1) * sizeof(char));
            be = fgetc(fp);
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
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
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

void fajlbol_beolvas(Esemeny *mibe, FILE *fp) {
    Esemeny e;
    fscanf(fp, "%d.%d.%d. %d:%d", &e.ev, &e.ho, &e.nap, &e.ora, &e.perc);
    mibe -> ev = e.ev;
    mibe -> ho = e.ho;
    mibe -> nap = e.nap;
    mibe -> ora = e.ora;
    mibe -> perc = e.perc;
    fgetc(fp);
    e.helyszin = szovegfajlbeolvas(fp);
    if (mibe -> helyszin != NULL)
      free(mibe -> helyszin);
    mibe -> helyszin = e.helyszin;

    e.nev = szovegfajlbeolvas(fp);
    if (mibe -> nev != NULL)
      free(mibe -> nev);
    mibe -> nev = e.nev;

    e.megj = szovegfajlbeolvas(fp);
    if (mibe -> megj != NULL)
      free(mibe -> megj);
    mibe -> megj = e.megj;

}

Esemeny *fajlbol_hozzafuz(Esemeny *lista, FILE *fp) {
    Esemeny* uj = (Esemeny*) malloc (sizeof(Esemeny));
    esemeny_init(uj);
    fajlbol_beolvas(uj, fp);
    if(lista == NULL)
      return uj;

    Esemeny *mozgo = lista;
    while(mozgo -> kov != NULL)
      mozgo = mozgo -> kov;
    mozgo -> kov = uj;
    return lista;
}

Esemeny *fajlbol_keszit(Esemeny *lista, FILE *fp) {
    int hossz;
    fscanf(fp ,"%d", &hossz);
    for(int i = 0; i < hossz; i ++ ){
      lista = fajlbol_hozzafuz(lista, fp);
    }
    return lista;
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



int napkiir(Esemeny *lista, int ev, int ho, int nap) {
    int db = 0;
    Esemeny *mozgo = lista;
    while(mozgo != NULL) {
      if(mozgo -> nap == nap && mozgo -> ho == ho && mozgo -> ev == ev) {
        esemeny_kiir(mozgo);
        db ++;
      }
      mozgo = mozgo -> kov;
    }
    return db;
}

int hokiir(Esemeny *lista, int ev, int ho) {
    int db = 0;
    Esemeny *mozgo = lista;
    while(mozgo != NULL) {
      if(mozgo -> ho == ho && mozgo -> ev == ev) {
        esemeny_kiir(mozgo);
        db ++;
      }
      mozgo = mozgo -> kov;
    }
    return db;
}

void het(Esemeny *lista, int ev, int ho, int nap) {

}

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

void fajlba_kiir(Esemeny *e, FILE *fp) {
    fprintf(fp, "%4d.%02d.%02d. %02d:%02d\n", e -> ev, e -> ho, e -> nap, e -> ora, e -> perc);
    if(e -> helyszin != NULL)
        fprintf(fp, "%s\n", e -> helyszin);
    else fprintf(fp,"0\n");

    if(e -> nev != NULL)
        fprintf(fp, "%s\n", e -> nev);
    else fprintf(fp,"0\n");

    if(e -> megj != NULL)
        fprintf(fp, "%s\n", e -> megj);
    else fprintf(fp,"0\n");
}


int main()
{
    Esemeny* naplo = (Esemeny*) malloc(sizeof(Esemeny));
    esemeny_init(naplo);
    Esemeny* mozgo;
    free(naplo);
    esemeny_init(naplo);
    naplo = NULL;
    FILE *fp;
    int ev, ho, nap;
    int c;

    char *mitkeres, *fajlnev;

    int *talalat;
    int i, n, db = 0, talalatvalaszt, naplomeret = 0;
    int v = fomenu();
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
                mitkeres = beolvas();
                system("cls");
                talalat = keres(naplo, mitkeres);
                db = talalat_kiir(naplo, talalat);
                if(db > 1) {
                    printf("\nTalalat kivalasztasa (0 - vissza): ");
                    scanf("%d", &talalatvalaszt);
                    getchar();
                    if(talalatvalaszt > 0 && talalatvalaszt <= db)
                      talalatvalaszt = talalat[talalatvalaszt - 1];
                    else if(talalatvalaszt == 0) {
                      free(mitkeres);
                      free(talalat);
                      break;
                    }
                    else {
                      printf("Ervenytelen ertek, nyomjon ENTER-t a fomenube lepeshez\n");
                      free(mitkeres);
                      free(talalat);
                      getchar();
                      break;
                    }
                }

                else if(db == 1)
                  talalatvalaszt = talalat[0];

                else {
                  free(mitkeres);
                  getchar();
                  break;
                }

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
                        break;

                    case 2:
                        if(naplomeret == 1) {
                          felszabadit(naplo);
                          naplomeret = 0;
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
                        felszabadit(mozgo -> kov);
                        mozgo -> kov = mozgo -> kov -> kov;

                        break;

                    default:
                        break;
                }

                free(talalat);
                free(mitkeres);
                break;

            case 3:
                system("cls");
                printf("Valasszon intervallumot!\n");
                printf("  1 - Nap\n  2 - Het\n  3 - Honap\n  4 - Vissza\n");
                printf("\n  Valasz: ");
                scanf("%d", &n);
                //while ((c = getchar()) != '\n' && c != EOF) { }

                switch(n) {
                  case 1:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.%d.", &ev, &ho, &nap);
                    if(napkiir(naplo, ev, ho, nap) == 0)
                      printf("Nincs talalat!\n");
                    scanf("%d", &ev);
                    break;

                  case 2:

                  case 3:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.", &ev, &ho);
                    if(hokiir(naplo, ev, ho) == 0)
                      printf("Nincs talalat!\n");
                    scanf("%d", &nap);
                    break;

                  case 4:
                    break;

                  default:
                    break;
                }
                break;

            case 4:
                printf("Fajl neve: ");
                fajlnev = beolvas();
                fp = fopen(fajlnev, "w");
                mozgo = naplo;
                while(mozgo != NULL) {
                  fajlba_kiir(naplo, fp);
                  mozgo = mozgo -> kov;
                }

                fclose(fp);
                free(fajlnev);
                break;

            case 5:
                felszabadit(naplo);
                naplo = NULL;
                printf("Fajl neve: ");
                fajlnev = beolvas();
                fp = fopen(fajlnev, "r");
                naplo = fajlbol_keszit(naplo, fp);
                getchar();
                fclose(fp);
                free(fajlnev);
                break;

              case 6:
                system("cls");
                teljeskiir(naplo);
                printf("\nNyomja meg az ENTER-t a visszalepeshez\n");
                getchar();
                break;
        }
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
