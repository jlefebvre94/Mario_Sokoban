#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "Constantes.h"
#include "editeur.h"
#include "fichiers.h"

void editeur(SDL_Surface* ecran) {
    //declaration des variables
    SDL_Surface *mur = NULL, *caisse = NULL, *objectif = NULL, *mario = NULL;
    SDL_Rect position;
    SDL_Event event;

    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = MUR, i = 0, j = 0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    //Chargement des objets et du niveau
    mur = IMG_Load("mur.jpg");
    caisse = IMG_Load("caisse.jpg");
    objectif = IMG_Load("objectif.png");
    mario = IMG_Load("mario_bas.gif");

    //Chargement de la carte
    if(!chargeNiveau(carte))
        exit(EXIT_FAILURE);

    switch(event.type) {
        case SDL_QUIT :
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                //on place l'objet a l'endroit du clic
                carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                clicGaucheEnCours = 1;  // on retient que le boutton gauche est enfonce
            }
            else if(event.button.button == SDL_BUTTON_RIGHT) {
                //on efface l'objet
                carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = VIDE;
                clicDroitEnCours = 1; // on retient que le boutton droit est enfonc
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT) {
                clicGaucheEnCours = 0;
            }
            else if(event.button.button == SDL_BUTTON_RIGHT) {
                clicDroitEnCours = 0;
            }
            break;
        case SDL_MOUSEMOTION:
            if(clicGaucheEnCours) {
                // deplacement de la souris avec boutton gauche enfonce
                carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
            }
            else if(clicDroitEnCours) {
                //idem boutton droit
                carte[event.motion.x / TAILLE_BLOC][event.motion.y /TAILLE_BLOC] = VIDE;
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                SDLK_ESCAPE:
                    continuer = 0;
                    break;
                SDLK_S:
                    sauvegarderNiveau(carte);
                    break;
                SDLK_C:
                    chargerNiveau(carte);
                    break;
                SDLK_KP1:
                    objetActuel = MUR;
                    break;
                SDLK_KP2:
                    objetActuel = CAISSE;
                    break;
                SDLK_KP3:
                    objetActuel = OBJECTIF;
                    break;
                SDLK_KP4:
                    objetActuel = MARIO;
                    break;
            }
            break;
    }

    //Effacement de l'ecran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,255,255,255));

    //Placement des objets a l'ecran
    for(i = 0; i < NB_BLOCS_LARGEUR; i++) {
        for(j = 0; j < NB_BLOCS_HAUTEUR; j++)
            {
            position.x = i * TAILLE_BLOC;
            position.y = j * TAILLE_BLOC;

            switch(carte[i][j]) {
                case MUR:
                    SDL_BlitSurface(mur, NULL, ecran, &position);
                    break;
                case CAISSE:
                    SDL_BlitSurface(caisse, NULL, ecran, &position);
                    break;
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, ecran, &position);
                    break;
                case MARIO:
                    SDL_BlitSurface(mario, NULL, ecran, &position);
                    break;
            }
        }

    }

    //mise a jour de l'ecran
    SDL_Flip(ecran);

    SDL_FreeSurface(mur);
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(mario);
}
