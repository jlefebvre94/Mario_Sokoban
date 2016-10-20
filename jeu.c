
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "Constantes.h"
#include "jeu.h"
#include "fichiers.h"

void jouer(SDL_Surface* ecran) {
    /* Declaration des VARIABLES */
    //Variables SDL
    SDL_Surface *mario[4] = {NULL};  //4 direction donc 4 surfaces
    SDL_Surface *mur = NULL, *caisse = NULL, *caisseOk = NULL, *objectif = NULL, *marioActuel = NULL;  // les differentes  surfaces possible

    SDL_Rect position, positionJoueur;

    SDL_Event event; //evenement

    //autres variables
    int continuer = 1, objectifRestant = 0, i = 0, j = 0;
    int carte[NB_BLOCS_HAUTEUR] [NB_BLOCS_LARGEUR] = {0}; //tableau representant la carte

    /* Initialisation */
    //Chargement des sprites
    mur = IMG_Load("mur.jpg");
    caisse = IMG_Load("caisse.jpg");
    caisseOk = IMG_Load("caisse_Ok.jpg");
    objectif = IMG_Load("objectif.png");
    mario[BAS] = IMG_Load("mario_bas.gif");
    mario[HAUT] = IMG_Load("mario_haut.gif");
    mario[GAUCHE] = IMG_Load("mario_gauche.gif");
    mario[DROITE] = IMG_Load("mario_droite.gif");

    marioActuel = mario[DROITE]; // initialiser mario vers la droite

    //Chargement Carte
    if(chargerNiveau(carte)) {
        exit(EXIT_FAILURE);        // arret du jeu si le niveau n'est pas charge
    }

    //recherche de la position de Mario au depart
    for(i=0; i < NB_BLOCS_LARGEUR;i++) {
        for(j=0; j < NB_BLOCS_HAUTEUR;j++) {
            if(carte[i][j] == MARIO) {  //si mario est a cette position
                positionJoueur.x = i;
                positionJoueur.y = j;
                carte[i][j] = VIDE;
            }
        }

    }

    //Activation de la repetition de la touche
    SDL_EnableKeyRepeat(100,100);

    switch(event.type) {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    continuer = 0;
                    break;
                case SDLK_UP:
                    marioActuel = mario[HAUT];
                    deplacerJoueur(carte, &positionJoueur, HAUT);
                    break;
                case SDLK_DOWN:
                    marioActuel = mario[BAS];
                    depalcerJoueur(carte, &positionJoueur, BAS);
                    break;
                case SDLK_RIGHT:
                    marioActuel = mario[DROITE];
                    deplacerJoueur(carte, &positionJoueur, DROITE);
                    break;
                case SDLK_LEFT:
                    marioActuel = mario[GAUCHE];
                    deplacerJoueur(carte, &positionJoueur, GAUCHE);
                    break;
            }
            break;
    }

    //Effacement de l'ecran
    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format, 255, 255,255));


    //Placement des objets a l'ecran
    objectifRestant = 0;

    for(i=0;i < NB_BLOCS_LARGEUR; i++) {
        for(j=0;j < NB_BLOCS_HAUTEUR;i++) {
            position.x = i * TAILLE_BLOC;
            position.y = j * TAILLE_BLOC;

            switch(carte[i][j]){
                case MUR:
                    SDL_BlitSurface(mur,NULL,ecran,&position);
                    break;
                case CAISSE:
                    SDL_BlitSurface(caisse, NULL,ecran,&position);
                    break;
                case CAISSE_OK:
                    SDL_BlitSurface(caisseOk,NULL, ecran, &position);
                    break;
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, ecran, &position);
                    objectifRestant = 1;
                    break;



            }
        }

    }

    //Si il n'y a plus d'objectifs restant, c'est que c'est resolu
    if(!objectifRestant)
        continuer = 0;

    //Placement d Joueur
    position.x = positionJoueur.x * TAILLE_BLOC;
    position.y = positionJoueur.y * TAILLE_BLOC;
    SDL_BlitSurface(marioActuel, NULL, ecran, &position);

    //on Affiche
    SDL_Flip(ecran);

    //desactivation de la repetion des touches
    SDL_EnableKeyRepeat(0, 0);

    //Libaration des surfaces
    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOk);
    for(i=0; i<4;i++) {
        SDL_FreeSurface(mario[i]);
    }

}

void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction) {
    //Test de la direction
    switch(direction) {
        case HAUT:
            if(pos->y - 1 < 0) //si le joueur est tout en haut, il ne peut pas monter (passer dans un position y negatif)
                break;
            if(carte[pos->x][pos->y -1] == MUR) //le joueur ne peut pas monter si il y a un mur au dessus
                break;
            //cas ou on pousse une caisse mais qu'il y a un mur ou une autre caisse derriere ou la limite de la carte
            if((carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y -1] == CAISSE_OK) && (pos->y - 2 < 0 || carte[pos->x][pos->y - 2] == MUR || carte[pos->x][pos->y - 2] == CAISSE || carte[pos->x][pos->y - 2] == CAISSE_OK))
                break;

            //Si les conditions definis precedement sont passée on peut déplacer la caisse
            deplacerCaisse(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2]);
            //on deplace le joueur
            pos->y--;
        case BAS:
            if(pos->y + 1 > (NB_BLOCS_HAUTEUR-1)) //si le joueur est tout en haut, il ne peut pas monter (passer dans un position y negatif)
                break;
            if(carte[pos->x][pos->y  + 1] == MUR) //le joueur ne peut pas monter si il y a un mur au dessus
                break;
            //cas ou on pousse une caisse mais qu'il y a un mur ou une autre caisse derriere ou la limite de la carte
            if((carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK) &&
               (pos->y + 2 > (NB_BLOCS_HAUTEUR - 1) ||
               carte[pos->x][pos->y + 2] == MUR || carte[pos->x][pos->y + 2] == CAISSE || carte[pos->x][pos->y + 2] == CAISSE_OK))
                break;

            //Si les conditions definis precedement sont passée on peut déplacer la caisse
            deplacerCaisse(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2]);
            //on deplace le joueur
            pos->y++;
        case GAUCHE:
            if(pos->x - 1 < 0) //si le joueur est tout en haut, il ne peut pas monter (passer dans un position y negatif)
                break;
            if(carte[pos->x - 1][pos->y] == MUR) //le joueur ne peut pas monter si il y a un mur au dessus
                break;
            //cas ou on pousse une caisse mais qu'il y a un mur ou une autre caisse derriere ou la limite de la carte
            if((carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK) && (pos->x - 2 < 0 || carte[pos->x - 2 ][pos->y] == MUR || carte[pos->x - 2][pos->y] == CAISSE || carte[pos->x - 2][pos->y] == CAISSE_OK))
                break;

            //Si les conditions definis precedement sont passée on peut déplacer la caisse
            deplacerCaisse(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y]);
            //on deplace le joueur
            pos->x--;
        case DROITE:
            if(pos->x + 1 > (NB_BLOCS_LARGEUR - 1)) //si le joueur est tout en haut, il ne peut pas monter (passer dans un position y negatif)
                break;
            if(carte[pos->x + 1][pos->y] == MUR) //le joueur ne peut pas monter si il y a un mur au dessus
                break;
            //cas ou on pousse une caisse mais qu'il y a un mur ou une autre caisse derriere ou la limite de la carte
            if((carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK) && (pos->x + 2 > (NB_BLOCS_LARGEUR - 1) || carte[pos->x + 2][pos->y] == MUR || carte[pos->x + 2][pos->y] == CAISSE || carte[pos->x + 2][pos->y] == CAISSE_OK))
                break;

            //Si les conditions definis precedement sont passée on peut déplacer la caisse
            deplacerCaisse(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y]);
            //on deplace le joueur
            pos->x++;
    }
}

void deplacerCaisse(int *premiereCase, int *secondeCase) {
    //test si c'est une caisse
    if(*premiereCase == CAISSE || *premiereCase == CAISSE_OK) {
        //on modifie la casse d'arrivée
        if(*secondeCase == OBJECTIF) {
            *secondeCase = CAISSE_OK;
        }
        else {
            *secondeCase = CAISSE;
        }
        //On modifie la case de départ
        if(*premiereCase == CAISSE_OK) {
            *premiereCase = OBJECTIF;
        }
        else {
            *premiereCase = VIDE;
        }
    }
}
