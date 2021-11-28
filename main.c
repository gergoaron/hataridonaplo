#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esemenykezeles.h"
#include "datumkezeles.h"
#include "fajlkezeles.h"
#include "menukezeles.h"

#include "debugmalloc.h"

int main()
{
    Esemeny* naplo = (Esemeny*) malloc(sizeof(Esemeny));
    esemeny_init(naplo);
    free(naplo);
    naplo = NULL;

    Esemeny* mozgo, *seged;
    FILE *fp;
    int ev, ho, het, nap;

    char *mitkeres, *fajlnev;

    int *talalat;
    int i, n, db = 0, talalatvalaszt;
    int v = fomenu();

    while(v != 0) {
        switch(v) {
            case 1:
                system("cls");
                naplo = esemeny_hozzafuz(naplo);
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
                break;

            case 3:
                system("cls");
                printf("Valasszon intervallumot!\n");
                printf("  1 - Nap\n  2 - Het\n  3 - Honap\n  0 - Vissza\n");
                printf("\n  Valasz: ");
                scanf("%d", &n);
                system("cls");

                switch(n) {
                  case 1:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.%d.", &ev, &ho, &nap);
                    getchar();
                    if(napkiir(naplo, ev, ho, nap) == 0)
                      printf("Nincs talalat!\n");
                    printf("\nNyomjon ENTER-t a visszalepeshez\n");
                    getchar();
                    break;

                  case 2:
                    printf("Adja meg a datumot(ev.het.): ");
                    scanf("%d.%d.",&ev, &het);
                    getchar();
                    if(hetkiir(naplo, ev, het) == 0)
                      printf("Nincs talalat!\n");
                    printf("\nNyomjon ENTER-t a visszalepeshez\n");
                    getchar();
                    break;
                  case 3:
                    printf("Adja meg a datumot: ");
                    scanf("%d.%d.", &ev, &ho);
                    getchar();
                    if(hokiir(naplo, ev, ho) == 0)
                      printf("Nincs talalat!\n");
                    printf("\nNyomjon ENTER-t a visszalepeshez\n");
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
                if(fp == NULL) {
                    perror("Fajl megnyitasa sikertelen!");
                    getchar();
                    break;
                }
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
                break;

            case 5:
                system("cls");
                felszabadit(naplo);
                naplo = NULL;
                printf("Fajl beolvasasa\n");
                printf("Fajl neve: ");
                fajlnev = beolvas();
                fp = fopen(fajlnev, "r");
                if(fp == NULL) {
                    perror("Fajl megnyitasa sikertelen!");
                    getchar();
                    break;
                }
                naplo = fajlbol_keszit(naplo, fp);
                fclose(fp);
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
    system("cls");
    return 0;
}
