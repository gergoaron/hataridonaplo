#include "esemenykezeles.h"

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

void felszabadit(Esemeny *lista) {
    Esemeny *felsz = lista;
    while(felsz != NULL) {
        Esemeny *next = felsz -> kov;
        esemeny_destroy(felsz);
        felsz = next;
    }
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

int *keres(Esemeny *eleje, char *mit) {
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

Esemeny *esemeny_hozzafuz(Esemeny* lista) {
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
