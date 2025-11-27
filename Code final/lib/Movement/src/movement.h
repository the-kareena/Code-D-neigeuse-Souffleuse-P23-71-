#ifndef MOVEMENT_H
#define MOVEMENT_H

#define VMIN_TOURNE 0.3
#define VMAX_TOURNE 0.3
#define KP 0.01
#define GAUCHE LEFT
#define DROITE RIGHT


void avancer_MOVE(float valeur = 1000, float vitesse = 0.30); // Fonction avancer avec paramètres de base.

void tourneGauche(float angle);

void virage_DROIT(float valeur = 1000, float angle = 45, float vitesse = 0.50);

void virage_GAUCHE(float valeur = 1000, float angle = 45, float vitesse = 0.50);

void surplace_DROIT(float angle = 90, float vitesse = 0.30);

void surplace_GAUCHE(float angle = 90, float vitesse = 0.30);

void avancer_ind(float vitesse = 0.20);

void reculer_MOVE(float valeur = 1000, float vitesse = 0.30);

#endif