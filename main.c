#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include <string.h>
#include "esemenykezeles.h"
#include "datumkezeles.h"

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

int main()
{
    Esemeny* naplo = (Esemeny*) malloc(sizeof(Esemeny));
    esemeny_init(naplo);
    Esemeny* mozgo, *seged;
    free(naplo);
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
    system("cls");
    return 0;
}
