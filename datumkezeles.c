#include "datumkezeles.h"

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
