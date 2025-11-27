#include "suiveurDeLigne.h"
#include <Arduino.h>
#include "LibRobus.h"
 
float seuilContrasteGauche = 100.00;
float seuilContrasteCentre = 100.00;
float seuilContrasteDroite = 100.00;

void tourner(int direction)
{
    if (direction == GAUCHE)
    {
        MOTOR_SetSpeed(DROITE, VITESSE_TOURNE);
        MOTOR_SetSpeed(GAUCHE, 0.05);
    }
    else
    {
        MOTOR_SetSpeed(GAUCHE, VITESSE_TOURNE);
        MOTOR_SetSpeed(DROITE, 0.05);
    }
}
 
void tournerRecule(int direction)
{
    if (direction == GAUCHE)
    {
        MOTOR_SetSpeed(DROITE, -VITESSE_TOURNE);
        MOTOR_SetSpeed(GAUCHE, -0.05);
    }
    else
    {
        MOTOR_SetSpeed(GAUCHE, -VITESSE_TOURNE);
        MOTOR_SetSpeed(DROITE, -0.05);
    }
}
 
void tourneDroit(float angle)
{ // Fonction qui permet au robot de tourner à droite sur lui même.
    float vitesse = VMAX_TOURNE;
    float vitesseGauche = vitesse;
    float vitesseDroite = -vitesse;
    float total2moteurs = 0;
    float moyenne2moteurs = 0;
 
    int32_t pulsesGauche = 0;
    int32_t pulsesDroite = 0;
    int32_t nbPulsesVoulues = angle * (1964 / 90); // 90 deg == 1964
 
    while (moyenne2moteurs <= nbPulsesVoulues)
    {
        pulsesGauche = ENCODER_Read(GAUCHE);
        pulsesDroite = ENCODER_Read(DROITE);
 
        total2moteurs = pulsesGauche - pulsesDroite;
        moyenne2moteurs = total2moteurs / 2;
 
        vitesse = VMIN_TOURNE + ((VMAX_TOURNE - VMIN_TOURNE) * 4.0f * (moyenne2moteurs / (float)nbPulsesVoulues) * (1.0f - (moyenne2moteurs / (float)nbPulsesVoulues)));
 
        vitesseGauche = vitesse;
        vitesseDroite = -vitesse;
 
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
 
bool detectionLigne(int capteur, float seuil)
{
    float etat = analogRead(capteur);
    if (etat < seuil)
    {
        return true;
    }
    else
    {
        return false;
    }
}
 
void suivreLigne()
{
    if (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true)
    {
        MOTOR_SetSpeed(GAUCHE, VITESSE_AVANCE);
        MOTOR_SetSpeed(DROITE, VITESSE_AVANCE);
        delayMicroseconds(1);
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_GAUCHE, seuilContrasteGauche) == true))
    {
        while (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false)
        {
            // Serial.println("TOURNER GAUCHE");
            tourner(GAUCHE);
            delayMicroseconds(1);
        }
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_DROITE, seuilContrasteDroite) == true))
    {
        while (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false)
        {
            // Serial.println("TOURNER DROITE");
            tourner(DROITE);
            delayMicroseconds(1);
        }
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_GAUCHE, seuilContrasteGauche) == true) && (detectionLigne(CAPTEUR_DROITE, seuilContrasteDroite) == true))
    {
        MOTOR_SetSpeed(GAUCHE, VITESSE_AVANCE);
        MOTOR_SetSpeed(DROITE, VITESSE_AVANCE);
        delayMicroseconds(1);
    }
}
 
void suivreLigneRecule(){
     if (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true)
    {
        MOTOR_SetSpeed(GAUCHE, -VITESSE_AVANCE);
        MOTOR_SetSpeed(DROITE, -VITESSE_AVANCE);
        delayMicroseconds(1);
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_GAUCHE, seuilContrasteGauche) == true))
    {
        while (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false)
        {
            // Serial.println("TOURNER GAUCHE");
            tournerRecule(GAUCHE);
            delayMicroseconds(1);
        }
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_DROITE, seuilContrasteDroite) == true))
    {
        while (detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false)
        {
            // Serial.println("TOURNER DROITE");
            tournerRecule(DROITE);
            delayMicroseconds(1);
        }
    }
    else if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == false) && (detectionLigne(CAPTEUR_GAUCHE, seuilContrasteGauche) == true) && (detectionLigne(CAPTEUR_DROITE, seuilContrasteDroite) == true))
    {
        MOTOR_SetSpeed(GAUCHE, -VITESSE_AVANCE);
        MOTOR_SetSpeed(DROITE, -VITESSE_AVANCE);
        delayMicroseconds(1);
    }
 
}
 
 
bool ligneHorizontale()
{
    if ((detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true) && (detectionLigne(CAPTEUR_GAUCHE, seuilContrasteGauche) == true) && (detectionLigne(CAPTEUR_DROITE, seuilContrasteDroite) == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}
 
 
void debugLigne()
{
    // Debug
    Serial.print("Capteur gauche: ");
    Serial.print(analogRead(CAPTEUR_GAUCHE));
    Serial.print(" | Capteur centre: ");
    Serial.print(analogRead(CAPTEUR_CENTRE));
    Serial.print(" | Capteur droite: ");
    Serial.println(analogRead(CAPTEUR_DROITE));
    delay(200);
}