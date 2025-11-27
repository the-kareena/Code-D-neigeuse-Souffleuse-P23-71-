//Declaration des variables
// Définition des variables globales
#define VITESSE_AVANCE 0.3
#define VITESSE_TOURNE 0.3
#define GAUCHE LEFT
#define DROITE RIGHT

#define VMIN_TOURNE 0.1
#define VMAX_TOURNE 0.2

 
// Définition des variables pour le suiveur de ligne
#define PIN_U1 A5
#define PIN_U2 A6
#define PIN_U3 A7
#define CAPTEUR_GAUCHE PIN_U1
#define CAPTEUR_CENTRE PIN_U2
#define CAPTEUR_DROITE PIN_U3
 
 
 
 
void tourner(int direction);
 
void tournerRecule(int direction);
 
 
void tourneDroit(float angle);
 
 
bool detectionLigne(int capteur, float seuil);
 
void suivreLigne();
 
void suivreLigneRecule();
 
bool ligneHorizontale ();
 
void debugLigne();