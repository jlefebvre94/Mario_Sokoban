/*
main.c
------
Par mateo21, pour Le Site du Zéro (www.siteduzero.com)
Rôle : menu du jeu. Permet de choisir entre l'éditeur et le jeu
lui-même.
*/
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "Constantes.h"
#include "editeur.h"
#include "jeu.h"
#include "fichiers.h"

int main(int argc, char *argv[]){
    //Description des variables
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;

    int continuer = 1;

    //initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);

    //Chargement de l'icone
    SDL_WM_SetIcon(IMG_Load("caisse.jpg"),NULL);

    //Definition de l'ecran
    ecran = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    //Titre de la fenetre
    SDL_WM_SetCaption("Mario Sokoban", NULL);

    // Chargement de l'image et position
    menu = IMG_Load("menu.jpg");
    positionMenu.x = 0;
    positionMenu.y = 0;

    //Boucle d'evenement
    while(continuer) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:     // fin de l'appli
                        continuer = 0;
                        break;
                    case SDLK_KP1:      // Jouer
                        //jouer(ecran);
                        break;
                    case SDLK_KP2:      //Lancer l'editeur
                        //editeur(ecran);
                        break;
                } //fin switch 2
            break;
        } //fin Switch 1

        //effacement de l'ecran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0,0,0));

    //application de l'image
    SDL_BlitSurface(menu,NULL,ecran,&positionMenu);

    //Rafraichissement Affichage
    SDL_Flip(ecran);
    } //Fin while

    //Liberation de la surface
    SDL_FreeSurface(menu);
    SDL_Quit();

    return EXIT_SUCCESS;
} // fin Main
