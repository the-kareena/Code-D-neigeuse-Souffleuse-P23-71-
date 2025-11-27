#include "movement.h"
#include <Arduino.h>
#include <LibRobus.h>


#include "suiveurDeLigne.h"



void avancer_MOVE(float valeur, float vitesse)

/*

Prend en argument une distance en mm et une vitesse.

*/
{
    // De base, avance de 1000 mm et avec une vitesse de 0.10.

    float vitesseGauche = vitesse;
    float vitesseDroit = vitesse;

    float distance_a_parcourir_finale = ((valeur * 3200) / (76.2 * M_PI));
    float distance_parcourue_finale_gauche = 0;
    float distance_parcourue_finale_droit = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_gauche <= distance_a_parcourir_finale)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Avance pendant 50 ms.

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs.
        float encodeurDroit = ENCODER_Read(1);

        if (encodeurGauche != encodeurDroit) // Différence de lecture.
        {
            vitesseDroit = (0.001 * (encodeurGauche - encodeurDroit)) + vitesseDroit;
        }

        distance_parcourue_finale_gauche += encodeurGauche;
        distance_parcourue_finale_droit += encodeurDroit;

        // Test pour comprendre comment la robot réagit au code.

        /* Serial.println(encodeurGauche);
        Serial.println(encodeurDroit);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(distance_parcourue_finale_gauche);
        Serial.println(distance_parcourue_finale_droit);
        Serial.println(distance_a_parcourir_finale);  */

        ENCODER_Reset(0);
        ENCODER_Reset(1);
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}

void tourneGauche(float angle)
{ // Fonction qui permet au robot de tourner à gauche sur lui même.

    ENCODER_ReadReset(GAUCHE);
    ENCODER_ReadReset(DROITE);

    float vitesse = VMAX_TOURNE;
    float vitesseGauche = -vitesse;
    float vitesseDroite = vitesse;
    float total2moteurs = 0;
    float moyenne2moteurs = 0;

    int32_t pulsesGauche = 0;
    int32_t pulsesDroite = 0;
    int32_t nbPulsesVoulues = angle * (1964 / 90); // 3200/(3po * 2,54 * pi) * (pi*0.25) --> 1964 == 90 deg
    

    while (moyenne2moteurs <= nbPulsesVoulues)
    {
        pulsesGauche = ENCODER_Read(GAUCHE);
        pulsesDroite = ENCODER_Read(DROITE);

        total2moteurs = -pulsesGauche + pulsesDroite;
        moyenne2moteurs = total2moteurs / 2;

        vitesse = VMIN_TOURNE + ((VMAX_TOURNE - VMIN_TOURNE) * 4.0f * (moyenne2moteurs / (float)nbPulsesVoulues) * (1.0f - (moyenne2moteurs / (float)nbPulsesVoulues)));

        vitesseGauche = -vitesse;
        vitesseDroite = vitesse;

        MOTOR_SetSpeed(GAUCHE, vitesseGauche);
        MOTOR_SetSpeed(DROITE, vitesseDroite);
    }

    MOTOR_SetSpeed(GAUCHE, 0);
    MOTOR_SetSpeed(DROITE, 0);

    ENCODER_ReadReset(GAUCHE);
    ENCODER_ReadReset(DROITE);
    moyenne2moteurs = 0;
    total2moteurs = 0;
}

void virage_DROIT(float rayon, float angle, float vitesse)

// Ici, VALEUR est la distance entre la roue gauche et le centre du centre qu'il parcourt.

{
    float rayon_petit_cercle = rayon;
    float cir_petit_cercle = ((rayon) * 2 * M_PI);
    float cir_grand_cercle = ((rayon_petit_cercle + 185) * 2 * M_PI); // Espace entre les roues de 185 mm.
    float ratio_cercle = (cir_petit_cercle / cir_grand_cercle);

    float distance_a_parcourir_grand_cercle = (((cir_grand_cercle * (angle / 360)) * 3200) / (76.2 * M_PI));

    float vitesseDroit = (ratio_cercle * vitesse);
    float vitesseGauche = vitesse;

    float distance_parcourue_finale_gauche = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_gauche <= distance_a_parcourir_grand_cercle) // && distance_parcourue_finale_droit < distance_a_parcourir_grand_cercle)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Tourne à gauche pendant 20 ms.

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs après 20 ms.
        float encodeurDroit = ENCODER_Read(1);

        if ((encodeurGauche * ratio_cercle) != encodeurDroit) // Différence de lecture.
        {
            vitesseDroit = (0.0008 * ((encodeurGauche * ratio_cercle) - encodeurDroit) + vitesseDroit);
        }
        distance_parcourue_finale_gauche += (encodeurGauche);

        /* Serial.println(ratio_cercle);
        Serial.println(encodeurGauche);
        Serial.println(encodeurDroit);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(distance_parcourue_finale_gauche);
        Serial.println(distance_a_parcourir_grand_cercle); */

        ENCODER_Reset(0);
        ENCODER_Reset(1);
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}


void virage_GAUCHE(float rayon, float angle, float vitesse)

// Ici, VALEUR est la distance entre la roue gauche et le centre du centre qu'il parcourt.

{
    float rayon_petit_cercle = rayon;
    float cir_petit_cercle = ((rayon) * 2 * M_PI);
    float cir_grand_cercle = ((rayon_petit_cercle + 185) * 2 * M_PI); // Espace entre les roues de 185 mm.
    float ratio_cercle = (cir_petit_cercle / cir_grand_cercle);

    float distance_a_parcourir_grand_cercle = (((cir_grand_cercle * (angle / 360)) * 3200) / (76.2 * M_PI));

    float vitesseGauche = (ratio_cercle * vitesse);
    float vitesseDroit = vitesse;

    float distance_parcourue_finale_droit = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_droit <= distance_a_parcourir_grand_cercle) // && distance_parcourue_finale_droit < distance_a_parcourir_grand_cercle)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Tourne à gauche pendant 20 ms.

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs après 20 ms.
        float encodeurDroit = ENCODER_Read(1);

        if ((encodeurDroit * ratio_cercle) != encodeurGauche) // Différence de lecture.
        {
            vitesseGauche = (0.0008 * ((encodeurDroit * ratio_cercle) - encodeurGauche) + vitesseGauche);
        }
        distance_parcourue_finale_droit += (encodeurDroit);

        /* Serial.println(ratio_cercle);
        Serial.println(encodeurGauche);
        Serial.println(encodeurDroit);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(distance_parcourue_finale_droit);
        Serial.println(distance_a_parcourir_grand_cercle); */

        ENCODER_Reset(0);
        ENCODER_Reset(1);
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}

void surplace_DROIT(float angle, float vitesse)
{
    float rayon_surplace = 92;
    float cir_cercle = (rayon_surplace * 2 * M_PI);

    float distance_a_parcourir_DROIT = (((cir_cercle * (angle / 360)) * 3200) / (76.2 * M_PI));

    float vitesseGauche = vitesse;
    float vitesseDroit = -vitesse;

    float distance_parcourue_finale_DROIT = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_DROIT < distance_a_parcourir_DROIT)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Surplace

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs après 20 ms.
        float encodeurDroit = ENCODER_Read(1);

        if (encodeurGauche != -encodeurDroit) // Différence de lecture.
        {
            vitesseDroit = ((-0.00002 * (encodeurGauche + encodeurDroit))) + vitesseDroit;
        }

        distance_parcourue_finale_DROIT -= encodeurDroit;

        ENCODER_Reset(0);
        ENCODER_Reset(1);

        /* Serial.println(encodeurDroit);
        Serial.println(encodeurGauche);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(cir_cercle);
        Serial.println(distance_parcourue_finale_DROIT); */
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}

void surplace_GAUCHE(float angle, float vitesse)
{
    float rayon_surplace = 90;
    float cir_cercle = (rayon_surplace * 2 * M_PI);

    float distance_a_parcourir_GAUCHE = (((cir_cercle * (angle / 360)) * 3200) / (76.2 * M_PI));

    float vitesseGauche = -vitesse;
    float vitesseDroit = vitesse;

    float distance_parcourue_finale_GAUCHE = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_GAUCHE < distance_a_parcourir_GAUCHE)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Surplace

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs après 20 ms.
        float encodeurDroit = ENCODER_Read(1);

        if (encodeurDroit != -encodeurGauche) // Différence de lecture.
        {
            vitesseGauche = ((-0.00002 * (encodeurGauche + encodeurDroit))) + vitesseGauche;
        }

        distance_parcourue_finale_GAUCHE -= encodeurGauche;

        ENCODER_Reset(0);
        ENCODER_Reset(1);

        /* Serial.println(encodeurDroit);
        Serial.println(encodeurGauche);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(cir_cercle);
        Serial.println(distance_parcourue_finale_GAUCHE); */
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}

void reculer_MOVE(float valeur, float vitesse)

/*

Prend en argument une distance en mm et une vitesse.

*/
{
    // De base, avance de 1000 mm et avec une vitesse de 0.10.

    float vitesseGauche = -vitesse;
    float vitesseDroit = -vitesse;

    float distance_a_parcourir_finale = -((valeur * 3200) / (76.2 * M_PI));
    float distance_parcourue_finale_gauche = 0;
    float distance_parcourue_finale_droit = 0;

    ENCODER_Reset(0);
    ENCODER_Reset(1);

    while (distance_parcourue_finale_gauche >= distance_a_parcourir_finale)
    {
        MOTOR_SetSpeed(0, vitesseGauche);
        MOTOR_SetSpeed(1, vitesseDroit);

        delay(50); // Avance pendant 50 ms.

        float encodeurGauche = ENCODER_Read(0); // Lis la valeur des deux encodeurs.
        float encodeurDroit = ENCODER_Read(1);

        if (encodeurGauche != encodeurDroit) // Différence de lecture.
        {
            vitesseDroit = (0.001 * (encodeurGauche - encodeurDroit)) + vitesseDroit;
        }

        distance_parcourue_finale_gauche += encodeurGauche;
        distance_parcourue_finale_droit += encodeurDroit;

        // Test pour comprendre comment la robot réagit au code.

        Serial.println(encodeurGauche);
        Serial.println(encodeurDroit);
        Serial.println(vitesseGauche);
        Serial.println(vitesseDroit);
        Serial.println(distance_parcourue_finale_gauche);
        Serial.println(distance_parcourue_finale_droit);
        Serial.println(distance_a_parcourir_finale);

        ENCODER_Reset(0);
        ENCODER_Reset(1);
    }

    MOTOR_SetSpeed(0, 0); // Arrêt.
    MOTOR_SetSpeed(1, 0);
}