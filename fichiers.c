#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "Constantes.h"
#include "fichiers.h"


//Chargement de niveau
int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR]) {
    //declaration variables
    FILE* fichier = NULL;
    char ligneFichier[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};
    int i = 0, j = 0;

    //ouverture fichier
    fichier = fopen("niveaux.lvl", "r");
    if(fichier == NULL)
        return 0;

    //lecture fichier
    fgets(ligneFichier, NB_BLOCS_HAUTEUR * NB_BLOCS_LARGEUR + 1, fichier);

    //Remplissage de la carte
    for(i = 0; i < NB_BLOCS_LARGEUR; i++) {
        for(j = 0; i < NB_BLOCS_HAUTEUR; j++) {
            switch(ligneFichier[i * NB_BLOCS_LARGEUR + j]) {
                case '0':
                    niveau[j][i] = 0;
                    break;
                case '1':
                    niveau[j][i] = 1;
                    break;
                case '2':
                    niveau[j][i] = 2;
                    break;
                case '3':
                    niveau[j][i] = 3;
                    break;
                case '4':
                    niveau[j][i] = 4;
                    break;
            }
        }

    }
    //fermeture fichier
    fclose(fichier);
    return 1;
}

int sauvegarderNiveau(int niveau[][NB_BLOCS_HAUTEUR]) {
    //declaration Variable
    FILE* fichier = NULL;
    int i = 0, j = 0;

    //ouverture fichier
    fichier = fopen("niveau.lvl", "w");
    if(fichier == NULL)
        return 0;

    //ecriture dans fichier
    for(i = 0; i < NB_BLOCS_LARGEUR; i++) {
        for(j = 0; j < NB_BLOCS_HAUTEUR; j++) {
            fprint(fichier, "%d", niveau[j][i]);
        }
    }

    //fermeture fichier
    fclose(fichier);
    return 1;
}
