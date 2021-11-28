#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include <string.h>

typedef struct Esemeny{
    int ev, ho, nap, het, hetnapja;
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

bool szokoev(int ev) {
  if( (ev % 4 == 0 && ev % 100 != 0) || ev % 400 == 0)
    return true;
  return false;
}

int hanyadik_nap(int ev, int ho, int nap) {
  int hanyadik = nap, februar = 28;
  if(szokoev(ev))
    februar = 29;
  switch (ho) {
    case 1:
      break;
    case 2:
      hanyadik += 31;
      break;
    case 3:
      hanyadik += 31 + februar;
      break;
    case 4:
      hanyadik += 31 + februar + 31;
      break;
    case 5:
      hanyadik += 31 + februar + 31 + 30;
      break;
    case 6:
      hanyadik += 31 + februar + 31 + 30 + 31;
      break;
    case 7:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30;
      break;
    case 8:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30 + 31;
      break;
    case 9:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30 + 31 + 31;
      break;
    case 10:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30 + 31 + 31 + 30;
      break;
    case 11:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
      break;
    case 12:
      hanyadik += 31 + februar + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
      break;
    default:
      hanyadik = 0;
      break;
  }
  return hanyadik;
}

void hanyadik_het(int ev, int ho, int nap, int *hanyadikhet, int *milyennap) {
  int napdb = hanyadik_nap(ev, ho, nap);
  int het = 1;
  if(napdb == 0)
    return;
  int hetnapja = 0, mozgoev = 2018;
  while(mozgoev < ev) {
    if(szokoev(mozgoev))
      hetnapja += 2;
    else
      hetnapja ++;
    hetnapja = hetnapja % 7;
    mozgoev ++;
  }

  while(mozgoev > ev) {
    mozgoev --;
    if(szokoev(mozgoev))
      hetnapja -= 2;
    else
      hetnapja --;
    hetnapja = hetnapja % 7;
  }

  for(int i = 1; i < napdb; i ++) {
    if(hetnapja == 6) {
      hetnapja = 0;
      het ++;
    }
    else hetnapja ++;
  }

  *hanyadikhet = het;
  *milyennap = hetnapja;
}

void esemeny_beolvas(Esemeny *mibe) {
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

    hanyadik_het(mibe -> ev, mibe -> ho, mibe -> nap, &mibe -> het, &mibe -> hetnapja);

}

bool esemeny_igazol(Esemeny *e) {
  if(e -> ev < 0)
    return false;
  if(e -> ho < 1 || e -> ho > 12)
    return false;
  if(e -> nap < 1 || e -> nap > 31)
    return false;
  if(e -> nap > 30 && !(e -> ho == 4 || e -> ho == 6 || e -> ho == 9 || e -> ho == 11))
    return false;
  if(e -> ho == 2) {
    if(szokoev(e -> ev)) {
      if(e -> nap > 29)
        return false;
      }
    if(e -> nap > 28)
      return false;
  }

  if(e -> ora < 0 || e -> ora > 23)
    return false;

  if(e -> perc < 0 || e -> perc > 59)
    return false;

  return true;
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

    hanyadik_het(mibe -> ev, mibe -> ho, mibe -> nap, &mibe -> het, &mibe -> hetnapja);

}

Esemeny *fajlbol_hozzafuz(Esemeny *lista, FILE *fp) {
    Esemeny* uj = (Esemeny*) malloc (sizeof(Esemeny));
    esemeny_init(uj);
    fajlbol_beolvas(uj, fp);
    if(!esemeny_igazol(uj)) {
      esemeny_destroy(uj);
      return lista;
    }

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
    if(!esemeny_igazol(uj)) {
      printf("Hibas adatok!\n");
      esemeny_destroy(uj);
      getchar();
      return lista;
    }
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

int hetkiir(Esemeny *lista, int ev, int het) {
    int db = 0;
    Esemeny *mozgo = lista;
    while(mozgo != NULL) {
      if(mozgo -> het == het && mozgo -> ev == ev) {
        esemeny_kiir(mozgo);
        db ++;
      }
      mozgo = mozgo -> kov;
    }
    return db;
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
    Esemeny* mozgo, *seged;
    free(naplo);
    esemeny_init(naplo);
    naplo = NULL;
    FILE *fp;
    int ev, ho, het, nap;

    char *mitkeres, *fajlnev;

    int *talalat;
    int i, c, n, db = 0, talalatvalaszt;
    int v = fomenu();
    while(v != 0) {
        switch(v) {
            case 1:
                system("cls");
                naplo = hozzafuz(naplo);
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
                        if(naplo -> kov == NULL) {
                          felszabadit(naplo);
                          naplo = NULL;
                          break;
                        }
                        mozgo = naplo;
                        seged = NULL;
                        for(int i = 0; i < talalatvalaszt; i ++) {
                          seged = mozgo;
                          mozgo = mozgo -> kov;
                        }

                        if(seged == NULL) {
                          seged = mozgo -> kov;
                          esemeny_destroy(mozgo);
                          naplo = seged;
                        }
                        else {
                          seged -> kov = mozgo -> kov;
                          esemeny_destroy(mozgo);
                        }

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
                printf("  1 - Nap\n  2 - Het\n  3 - Honap\n  0 - Vissza\n");
                printf("\n  Valasz: ");
                scanf("%d", &n);

                switch(n) {
                  case 1:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.%d.", &ev, &ho, &nap);
                    getchar();
                    if(napkiir(naplo, ev, ho, nap) == 0)
                      printf("Nincs talalat!\n");
                    printf("Nyomjon ENTER-t a visszalepeshez\n");
                    getchar();
                    break;

                  case 2:
                    printf("Adja meg a datumot(ev.het.): ");
                    scanf("%d.%d.",&ev, &het);
                    getchar();
                    if(hetkiir(naplo, ev, het) == 0)
                      printf("Nincs talalat!\n");
                    printf("Nyomjon ENTER-t a visszalepeshez\n");
                    getchar();
                    break;
                  case 3:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.", &ev, &ho);
                    getchar();
                    if(hokiir(naplo, ev, ho) == 0)
                      printf("Nincs talalat!\n");
                    printf("Nyomjon ENTER-t a visszalepeshez\n");
                    getchar();
                    break;

                  case 0:
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
                db = 0;
                while(mozgo != NULL) {
                  db ++;
                  mozgo = mozgo -> kov;
                }

                fprintf(fp, "%d\n", db);
                mozgo = naplo;

                while(mozgo != NULL) {
                  fajlba_kiir(naplo, fp);
                  mozgo = mozgo -> kov;
                }

                fclose(fp);
                free(fajlnev);
                break;

            case 5:
                system("cls");
                felszabadit(naplo);
                naplo = NULL;
                printf("Fajl beolvasasa\n");
                printf("Fajl neve: ");
                fajlnev = beolvas();
                fp = fopen(fajlnev, "r");
                naplo = fajlbol_keszit(naplo, fp);
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
    felszabadit(naplo);
    return 0;
}
