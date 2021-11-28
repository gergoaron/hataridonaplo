#ifndef DATUMKEZELES_H_INCLUDED
#define DATUMKEZELES_H_INCLUDED
#include <stdio.h>
#include <stdbool.h>
#include "esemenykezeles.h"


bool szokoev(int ev);

int hanyadik_nap(int ev, int ho, int nap);

void hanyadik_het(int ev, int ho, int nap, int *hanyadikhet, int *milyennap);

int napkiir(Esemeny *lista, int ev, int ho, int nap);

int hokiir(Esemeny *lista, int ev, int ho);

int hetkiir(Esemeny *lista, int ev, int het);

#endif // DATUMKEZELES_H_INCLUDED
