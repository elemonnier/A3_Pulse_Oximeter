#include "lecture.h"
#include <string.h>

///fonction pour lire les valeurs d'entrée de la trame
absorp lecture(FILE* file_pf, int* file_state) {
    absorp myAbsorp;
    float Table_temp[4] = {0}; ///Tableau temporaire pour stocké les valeurs de de la trame récupérer par le scanf

    char i;
    char c;

    ///On récupère les valeurs de la trame du fichier avec le scanf qu'on stock dans Table_temp

    while (*file_state != EOF) {
        *file_state = fscanf(file_pf, "%f,%f,%f,%f%c%c", &Table_temp[0], &Table_temp[1], &Table_temp[2], &Table_temp[3],
                             &i, &c);
        if (*file_state == 6) {
            ///On update les valeurs de acr, dcr, acir et dcir de myAbsorp
            myAbsorp.acr = Table_temp[0] - 2048;
            myAbsorp.dcr = Table_temp[1];
            myAbsorp.acir = Table_temp[2] - 2048;
            myAbsorp.dcir = Table_temp[3];
            return myAbsorp;
        }
    }
}