#include "iir.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
absorp iirTest(char* filename) {
    absorp myAbsorp;

    char c; ///variable qui récupére le caractère du fichier
    int compteur_mot = 0; ///compteur pour le nombre de mot dans le fichier
    int o; ///compteur pour vider la chaîne mot_temp
    int i = 0; ///On intialisalise une variable pour stocker nos chiffres dans un tableau
    char mot_temp[10]= {0}; ///Tableau pour stocker nos chiffres
    float table_tempX[4] = {0,0,0,0}; ///Tableau pour stocker les X[n]
    float table_pres_X[4] = {0,0,0,0}; ///Tableau pour stocker les X[n-1]
    float table_pres_Y[4] = {0,0,0,0}; ///Tableau pour stocker les Y[n-1]

    ///On ouvre notre fichier en mode lecture
    FILE *fp = fopen(filename,"r");

    ///Si on arrive à ouvrir notre fichier
    if(fp != NULL) {
        ///Tant que notre fichier n'est pas vide
        while ((c = getc(fp)) != EOF) {
            ///Si le charactère trouvé est une , ou un \f, alors on peut attribuer
            /// le nombre trouvé à une des variables de myAbsorp
            if (c == ',' || c==12 || c== 0x0A) {
                ///premier élément du tableau du fichier -> acr
                if (compteur_mot % 4 == 0) {
                    ///Pour notre calcul de Y[n], on sauvegarde la valeur de mot_temp pour X[n]
                    ///Ici poru myAbsorp.acr
                    table_tempX[0] = atoi(mot_temp); ///Atof permet de convertir notre chaîne de charactère en un float

                    ///compteur pour trouver le nombre de lignes / n
                    compteur_mot++;
                }
                ///second élément du tableau du fichier -> dcr
                else if (compteur_mot % 4 == 1) {
                    ///Pour notre calcul de Y[n], on sauvegarde la valeur de mot_temp pour X[n]
                    ///Ici pour myAbsorp.dcr
                    table_tempX[1] = atoi(mot_temp);
                    compteur_mot++;
                }
                ///troisième élément du tableau du fichier -> acir
                else if (compteur_mot % 4 == 2) {
                    ///Pour notre calcul de Y[n], on sauvegarde la valeur de mot_temp pour X[n]
                    ///Ici pour myAbsorp.acir
                    table_tempX[2] = atoi(mot_temp);
                    compteur_mot++;
                }
                ///quatrième élément du tableau du fichier -> dcir
                else if (compteur_mot % 4 == 3) {
                    ///Pour notre calcul de Y[n], on sauvegarde la valeur de mot_temp pour X[n]
                    ///Ici pour myAbsorp.dcir
                    table_tempX[3] = atoi(mot_temp);
                    compteur_mot++;
                    ///On met notre calcul dans une variable int afin de ne garder que la partie entière
                    float Partie_Entiere_AY_ACR = 0.992 * (float)table_pres_Y[0];
                    float Partie_Entiere_AY_ACIR = 0.992 * (float)table_pres_Y[2];
                    ///On peut alors faire notre calcul de Y[n]
                    myAbsorp.acr = (float)table_tempX[0] - (float)table_pres_X[0] + Partie_Entiere_AY_ACR;
                    myAbsorp.dcr = (float)table_tempX[1];
                    myAbsorp.acir = (float)table_tempX[2] - (float)table_pres_X[2] + Partie_Entiere_AY_ACIR;
                    myAbsorp.dcir = (float)table_tempX[3];
                    ///On remplace les valeurs de table_presX avec celle de table_presX_temp
                    /// pour l'utiliser dans le prochain calcul
                    table_pres_X[0] = table_tempX[0];
                    table_pres_X[1] = table_tempX[1];
                    table_pres_X[2] = table_tempX[2];
                    table_pres_X[3] = table_tempX[3];
                    ///On remplace les valeurs de table_presY avec celle de myAbsorp
                    /// pour l'utiliser dans le prochain calcul
                    table_pres_Y[0] = myAbsorp.acr;
                    table_pres_Y[1] = myAbsorp.dcr;
                    table_pres_Y[2] = myAbsorp.acir;
                    table_pres_Y[3] = myAbsorp.dcir;
                }
                /// on vide la chaîne de caractère une fois le nombre stocké
                for(o = 0;o<i;o++){
                    mot_temp[o]='\0';
                }
                i=0;
            } else if(c != 0x0D && c!= 8) {
                ///Tant qu'on ne recontre pas de ',' ou '\f'On ajoute le chiffre dans notre tableau temporaire
                mot_temp[i]=c;
                i++;
            }
        }
        ///On retroune les valeurs trouvées avec notre structure absorp
        return myAbsorp;
    }
    ///Si on n'arrive pas à ouvrir notre fichier, printf (Monkey !)
    else {
        printf("Reject humanity ... Return to MONK2Y !!!\n");
    }
}