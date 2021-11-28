#include "menukezeles.h"

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
