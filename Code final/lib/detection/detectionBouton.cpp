#include "detectionBouton.h"

void bouton()
{
    if (etat == 0)
    {
        commencement = 1;
        etat = 1;
    }
    if (etat == 1)
    {
        arret = 1;
    }
}