#include "mesure.h"
#include "fichiers.h"
oxy mesureTest(char* filename) {
    oxy myOxy; /// déclaration de l'oxy
    absorp myAbsorp; /// déclaration des absorp
    absorp myAbsorp2;
    float pouls;
    float SPO2;
    int level = 1; /// variable permettant d'utiliser le switch de la machine d'état
    int periode = 0; /// on compte les périodes parcourues
    float stock_cpt; /// stockage du compteur
    float cnt_ech = 1; /// incrément toutes les 2 ms
    int state = 0; /// variable permettant de vérifier si on est à la fin du fichier
    float BPM_moy = 0; /// on cherche la moyenne de bpm sur 3 periodes
    int nombre_periode = 3;
    float tableau_bpm[nombre_periode];
    int i;
    FILE *fichier = initFichier(filename); /// on ouvre le fichier
    myAbsorp = lireFichier(fichier, &state);
    float min_ACR = myAbsorp.acr; /// initialisation des valeurs de absorp
    float max_ACR = myAbsorp.acr;
    float min_ACIR = myAbsorp.acir;
    float max_ACIR = myAbsorp.acir;
    float stockage_peak_to_peak[2]; /// stockage des valeurs crete à crete
    float RsIR;
    while (state != EOF) { /// condition d'arrêt de lecture du fichier
        myAbsorp2 = lireFichier(fichier, &state);  /// prend la trame courante
        if (state != EOF) {
            myAbsorp = myAbsorp2;
            /// initialisation d'une machine d'état
            if (level == 1) {
                if (myAbsorp.acr > 500) {
                    level = 2;
                }
            }
            if (level == 2) {
                if (myAbsorp.acr < -500) {
                    level = 3;
                    cnt_ech = 0;
                }
            }
            if (level == 3) {
                if (myAbsorp.acr > 500) {
                    level = 4;
                }
            }
            if (level == 4) {
                if (myAbsorp.acr < -500) {
                    level = 3;
                    stock_cpt = cnt_ech;
                    cnt_ech = 0; /// reset du compteur à 0
                    for (i = 0; i < nombre_periode; i++) { /// buffer du tableau
                        tableau_bpm[i] = tableau_bpm[i + 1];
                    }
                    tableau_bpm[2] = stock_cpt; /// stockage du compteur dans un tableau
                    periode++;
                }
            }
            cnt_ech++; /// on compte le nombre de point pour une période
            /// le compteur echantillon va augmenter de 1 toute les 2 ms pour réaliser le calcul du bpm = 30000/calc_bpm
            usleep(2); /// suspend l'exécution du programme appelant durant 2 millisecondes afin d'obtenir 2ms par point
            if (periode == nombre_periode) { /// moyenne sur 3 périodes
                min_ACR = 0;
                max_ACR = 0;
                min_ACIR = 0;
                max_ACIR = 0;
                periode = 0;

                ///on fait la moyenne du nombre de point sur 3 périodes
                BPM_moy = ((tableau_bpm[0] + tableau_bpm[1] + tableau_bpm[2]) / nombre_periode);
            }
            if (myAbsorp.acir < min_ACIR) { /// mise à jour du min si une valeur le dépasse vers le bas
                min_ACIR = myAbsorp.acir;
                stockage_peak_to_peak[1] = max_ACIR - min_ACIR;
            } else if (myAbsorp.acir > max_ACIR) { /// mise à jour du max si une valeur le dépasse
                max_ACIR = myAbsorp.acir;
                stockage_peak_to_peak[1] = max_ACIR - min_ACIR;
            }
            if (myAbsorp.acr < min_ACR) { /// mise à jour du min si une valeur le dépasse vers le bas
                min_ACR = myAbsorp.acr;
                stockage_peak_to_peak[0] = max_ACR - min_ACR;
            } else if (myAbsorp.acr > max_ACR) { /// mise à jour du max si une valeur le dépasse
                max_ACR = myAbsorp.acr;
                stockage_peak_to_peak[0] = max_ACR - min_ACR;
            }
            RsIR = (stockage_peak_to_peak[0] / myAbsorp.dcr) / (stockage_peak_to_peak[1] / myAbsorp.dcir); /// calcul du RsIR

            pouls = 60 / (0.002 * BPM_moy); /// 60 pour 1 BPS pour trouver la valeur sur 2 ms
            myOxy.pouls = pouls;
            if (RsIR <= 1) { /// on observe la fonction affine de gauche de la caractéristique
                SPO2 = ((-25 * RsIR) + 110); /// y = -25x + 110
            } else { /// fonction affine de droite
                SPO2 = ((-(100 / 3) * RsIR) + (350 / 3)); /// y = -33x + 116
            }
            myOxy.spo2 = SPO2;
        }
    }
    return myOxy;
}
