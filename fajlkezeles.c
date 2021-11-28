#include "fajlkezeles.h"

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
    if(tomb[0] == '\0' || (tomb[0] == '0' && tomb[1] == '\0') ) {
        free(tomb);
        return NULL;
    }
    return tomb;
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
