#include "affichage.h"
#include "fichiers.h"
#include <unistd.h>
void affichage(oxy myOxy) { ///Fonction pour afficher nos valeurs contenue dans le fichier Data
    if (access(".verrouData", F_OK) == -1) { ///Si le fichier .verrouData n'existe pas
        ///On créer un fichier .verrouData
        FILE *verrou = verrou = fopen(".verrouData", "w");;
        ///On ouvre le fichier Data afin de modifier nos valeurs contenues dedans
        FILE *File = fopen("Data.txt", "w");
        fprintf(File, "%d\n%d", myOxy.spo2, myOxy.pouls);///On écris nos valeurs dans le fichier Data.txt
        fclose(File); ///On ferme notre fichier Data.txt
        remove(".verrouData");///on supprime notre fichier .verrouData
    }
}