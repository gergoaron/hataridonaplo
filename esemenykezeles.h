#ifndef ESEMENYKEZELES_H_INCLUDED
#define ESEMENYKEZELES_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>

typedef struct Esemeny{
    int ev, ho, nap, het, hetnapja;
    int ora, perc;
    char* helyszin;
    char* nev ;
    char* megj;
    struct Esemeny* kov;
}Esemeny;

void esemeny_init(Esemeny *p);

void esemeny_kiir(Esemeny *e);

void teljeskiir(Esemeny *lista);

void felszabadit(Esemeny *lista);

char *beolvas();

void esemeny_beolvas(Esemeny *mibe);

bool esemeny_igazol(Esemeny *e);

Esemeny *esemeny_hozzafuz(Esemeny* lista);


#endif // ESEMENYKEZELES_H_INCLUDED
