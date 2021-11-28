#ifndef FAJLKEZELES_H_INCLUDED
#define FAJLKEZELES_H_INCLUDED
#include <stdio.h>
#include "esemenykezeles.h"

char *szovegfajlbeolvas(FILE *fp);

void fajlbol_beolvas(Esemeny *mibe, FILE *fp);

Esemeny *fajlbol_hozzafuz(Esemeny *lista, FILE *fp);

Esemeny *fajlbol_keszit(Esemeny *lista, FILE *fp);

void fajlba_kiir(Esemeny *e, FILE *fp);


#endif // FAJLKEZELES_H_INCLUDED
