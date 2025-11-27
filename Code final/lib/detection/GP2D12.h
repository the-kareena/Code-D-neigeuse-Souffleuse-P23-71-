//////////////////////////////////////////////////////////////////////////////////////////////////////
// Etienne Vibert    Karina Ait Ouazzou  22 octobre 2025
// Librairie pour lire les capteurs de distance GP2D12
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

//fonctions : 

// Il print la distance des capteurs GP2D12 sur le moniteur serie
// On l'appelle dans le loop() pour voir les distances en temps reel
// Exemple d'utilisation:
// void loop() {
//   PrintDistance();
//   delay(500); // Attendre 500 ms entre chaque lecture
// }
void PrintDistance();


// Fonction qui retourne la distance mesurée par le capteur de gauche en centimètres
// on l'utilise la fonction quand on besoin de savoir la distance a droite
// Exemple d'utilisation:
// int distance = DistanceGauche();
// ou
// if (DistanceGauche() < 80) {
//   // on est vers la fin du mur
// }
int DistanceGauche();