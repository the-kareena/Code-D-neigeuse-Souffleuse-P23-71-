#include <Arduino.h>
#include "pelleSouffleuse.h"
#include "pin.h"

void pelle(bool etat) // Le robot met 8 sec pour baisser ou monter la pelle
{
    if (etat == ETEIN)
    {
        digitalWrite(pinENA, HIGH);
        digitalWrite(pinIN1, LOW); 
        digitalWrite(pinIN2, HIGH);
    }
    if (etat == ALLUMER)
    {
        digitalWrite(pinENA, HIGH);
        digitalWrite(pinIN1, HIGH);
        digitalWrite(pinIN2, LOW);
    }
}

void pelleArret(){
    digitalWrite(pinENA, LOW);
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
}


void souffleuse(bool etat)
{
    if (etat == ALLUMER)
    {
        // Monter doucement de 0° à 180°
        for (int angle = ESC_MIN; angle <= ESC_MAX; angle++)
        {
            SERVO_SetAngle(ESC_pin, angle);
            delay(10);
        }

        // Optionnel : rester à fond
        SERVO_SetAngle(ESC_pin, ESC_MAX);
    }
    else if (etat == ETEIN)
    {
        // Descendre doucement de 180° à 0°
        for (int angle = ESC_MAX; angle >= ESC_MIN; angle--)
        {
            SERVO_SetAngle(ESC_pin, angle);
            delay(10);
        }

        SERVO_SetAngle(ESC_pin, ESC_MIN);  // OFF
    }
}
