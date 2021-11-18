#include "fir.h"

absorp firTest(char* filename) {
    absorp myAbsorp;
    char c; /// caractère permettant de parcourir le fichier
    int nb_line = 0; /// comptage des lignes du fichier
    char tempstring[5]; /// chaîne de caractères temporaire pour stocker les float du fichier
    int i1 = 0; /// incréments pour les boucles for
    int j1 = 0;
    int ccounter = 0; /// compteur permettant de déterminer la taille de chaque float
    int o;
    //// on crée 2 tableaux (pour acr et acir) de 51 cases pour effectuer un buffer (décalage), les premières valeurs sont initialisées à zéro car nous n'avons pas d'information à t<0
    float x[51] = {0}; /// tableaux qui seront buffer pour stocker les 50 dernières valeurs de x
    float x2[51] = {0};
    int n; /// compteurs
    int j;
    int i;
    /// ouverture du fichier donné en argument
    FILE *fichier = fopen(filename, "r");
    /// comptage du nombre de lignes
    while ((c = getc(fichier)) != EOF) {
        if (c == '\b' || c == 0x0D)
            ++nb_line;
    }
    float matrice[nb_line][4];
    /// replacement du parseur au tout début du fichier afin de le reparcourir
    fseek(fichier, 0, SEEK_SET);
    while ((c = getc(fichier)) != EOF) {
        if (c == ',') {  /// on regarde si le caractère est un chiffre, une virgule ou un saut de ligne pour créer une matrice des valeurs
            matrice[i1][j1] = atoi(tempstring); /// passage de la chaîne en entier
            for (o = 0; o < ccounter; o++) { /// on vide la chaîne de caractères
                tempstring[o] = '\0';
            }
            j1++;
            ccounter = 0; /// reset compteur
        }
        if (c == '\f') {
            if (j1 != 0) {
                matrice[i1][j1] = atoi(tempstring); /// passage de la chaîne en entier
                for (o = 0; o < ccounter; o++) { /// on vide la chaîne de caractères
                    tempstring[o] = '\0';
                }
                i1++;
                j1 = 0;
                ccounter = 0; /// reset compteur

            }
        } else {
            if (c != '\b' && c != 8 && c != 0x0D) {
                tempstring[ccounter] = c; /// ajout du chiffre dans la chaîne de caractères
                ccounter++;
            }
        }
    }
    float h[51] = {1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,3.8503956e-004,1.6465231e-004,1.4774946e-019};/// valeurs théoriques de h
    for (n = 0; n < nb_line; n++) { /// parcours de la matrice
        myAbsorp.acr = matrice[n][0]; /// on initialise les valeurs de signaux en fonction de la matrice
        myAbsorp.dcr = matrice[n][1];
        myAbsorp.acir = matrice[n][2];
        myAbsorp.dcir = matrice[n][3];
        /// calcul de l'acr
        float temp = x[50];
        float temp2 = 0;
        x[50] = myAbsorp.acr;
        for (j = 49; j >= 0; j--) { /// on décale toutes les valeurs du tableau de 1 à l'aide de swaps
            if (j % 2 != 0) { /// on prend une valeur sur deux pour effectuer des swaps
                temp2 = x[j];
                x[j] = temp;
            } else {
                temp = x[j];
                x[j] = temp2;
            }
        }
        float y = 0;
        for (i = 0; i <= 50; i++) { /// calcul de la somme écrite dans l'énoncé
            y += h[i] * x[50 - i];
        }
        myAbsorp.acr = y; /// valeur de l'acr pour n = compteur
        /// calcul  de l'acir
        float temp3 = x2[50];
        float temp4 = 0;
        x2[50] = myAbsorp.acir;
        for (j = 49; j >= 0; j--) { /// on décale toutes les valeurs du tableau de 1 à l'aide de swaps
            if (j % 2 != 0) { /// on prend une valeur sur deux pour effectuer des swaps
                temp4 = x2[j];
                x2[j] = temp3;
            } else {
                temp3 = x2[j];
                x2[j] = temp4;
            }
        }
        float y2 = 0;
        for (i = 0; i <= 50; i++) { /// calcul de la somme écrite dans l'énoncé
            y2 += h[i] * x2[50 - i];
        }
        myAbsorp.acir = y2; /// valeur de l'acir pour n = compteur
    }
    return myAbsorp;
}
