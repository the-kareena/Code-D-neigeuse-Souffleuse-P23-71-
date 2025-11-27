/*
Projet: Code final du robot
Equipe: P23
Auteurs: Karina Ait Ouazzou, Etienne Vibert
Description: Programme permettant au robot d'accomplir les taches du deneigeur
Date: 2025-11-06
*/

#include <Arduino.h>
#include <LibRobus.h>

#include <Wire.h>
#include <GP2D12.h> // la librairie du ditecteur de distance

#include <movement.h> // Librairie pour les mouvements du robot

#include "detectionBouton.h"
#include "suiveurDeLigne.h"
#include "pin.h"
#include "pelleSouffleuse.h"

static float seuilContrasteCentre = 100.00;

void tournerLigneDroite()
{
  float angle = 0.0;
  while (!(detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true))
  {
     //surplace_DROIT((angle+=10.0f),0.2);
     MOTOR_SetSpeed(0, 0.2);
    MOTOR_SetSpeed(1, -0.2);
    }
    //MOTOR_SetSpeed(0, 0.1);
    //MOTOR_SetSpeed(0, -0.1);
}

void tournerLigneGauche()
{
  float angle = 0.0;
  while (!(detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true))
  {
     //surplace_DROIT((angle+=10.0f),0.2);
     MOTOR_SetSpeed(0, -0.2);
    MOTOR_SetSpeed(1, 0.2);
    }
    //MOTOR_SetSpeed(0, 0.1);
    //MOTOR_SetSpeed(0, -0.1);
}

void setup()
{ // Initialisation
  BoardInit();

  // initaliserPin();
  digitalWrite(pinBuzzer,LOW);
  pelle(ETEIN);
  SERVO_Enable(0);

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  SERVO_Enable(ESC_pin);
  SERVO_SetAngle(ESC_pin, ESC_MIN);

  Serial.begin(115200);

  // attachInterrupt(digitalPinToInterrupt(1), bouton, FALLING);
}

void loop()
{
  // while (ROBUS_IsBumper(2) == 0){
   
  // }

  while (true)
  {
    suivreLigneRecule();
    if (DistanceGauche() < 30)
    {
      while (!ligneHorizontale())
      {
        suivreLigneRecule();
      }
      digitalWrite(pinBuzzer,HIGH);

      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      Serial.println("Ligne horizontale detectee!!!");
      delay(1000);
      pelle(ALLUMER);
      delay(8000);
      souffleuse(ALLUMER);
      avancer_MOVE(150, -0.3);
      //surplace_GAUCHE(140, 0.3);
       surplace_GAUCHE(45, 0.3);
       tournerLigneGauche();
      //delay(2000);
      while (!ligneHorizontale())
      {
        suivreLigneRecule();
      }
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      // avancer_MOVE(400,-0.3);
      pelle(ETEIN);
      delay(8000);
      
      digitalWrite(pinBuzzer,LOW);

      reculer_MOVE(100, -0.3);
      while (!ligneHorizontale())
      {
        suivreLigne();
      }
      souffleuse(ETEIN);
      avancer_MOVE(100, -0.3);
      surplace_DROIT(45, 0.3);
      tournerLigneDroite();
    }
}
}

////////////////////////////// TEST DU DETECTEUR DE DISTANCE ///////////////////////////////
// void loop()
// {
//     if (DistanceGauche()< 30)
//     {
//         Serial.println("Maison detectee");
//     }
//     else {
//         Serial.println("A la recherche de la maison!!!");
//     }

//     PrintDistance();
//     delay(1000);
// }

////////////////////////////// TEST DE LA SOUFFLEUSE ///////////////////////////////
// void loop()
// {
// while (true)
//   {
//     souffleuse(ALLUMER);
//       delay(3000);

//       souffleuse(ETEIN);

//       delay(2000);

//   }

//     delay(500);
// }

////////////////////////////// TEST DE LA PELLE ///////////////////////////////////
// void loop()
// {
//   /* while(true){
//     souffleuse(ALLUMER);
//     delay(5000);
//     souffleuse(ETEIN);
//     delay(5000);
//   } */

//   while (true)
//   {
//     if (Serial.available() > 0)
//     {
//         char cmd = Serial.read();

//         switch (cmd)
//         {
//         case 'u':   // monter
//         case 'U':
//             Serial.println(F("Monter la pelle..."));
//             pelle(ALLUMER);
//             delay(8000); // mouvement complet
//             pelleArret();
//             Serial.println(F("Pelle en haut."));
//             break;

//         case 'd':   // descendre
//         case 'D':
//             Serial.println(F("Descendre la pelle..."));
//             pelle(ETEIN);
//             delay(8000); // mouvement complet
//             pelleArret();
//             Serial.println(F("Pelle en bas."));
//             break;

//         case 's':   // stop
//         case 'S':
//             Serial.println(F("Stop pelle."));
//             pelleArret();
//             break;

//         case 'h':   // help
//         case 'H':
//             Serial.println(F("Commandes :"));
//             Serial.println(F("  u = monter la pelle"));
//             Serial.println(F("  d = descendre la pelle"));
//             Serial.println(F("  s = stop"));
//             Serial.println(F("  h = aide"));
//             break;

//         case '\r':
//         case '\n':
//             // ignorer les retours chariot / nouvelle ligne
//             break;

//         default:
//             Serial.print(F("Commande inconnue : "));
//             Serial.println(cmd);
//             Serial.println(F("Tape 'h' pour l'aide."));
//             break;
//         }
//     }
//   }
// }

// void loop()
// {

//   while (commencement == 0)
//   {
//   }

//   while (true)
//   {
//     suivreLigneRecule();  // en etat normale il suit simplement la ligne
//     if (DistanceGauche() < 50)
//     {
//       while (!ligneHorizontale())
//       {
//         suivreLigneRecule();
//       }
//       MOTOR_SetSpeed(0,0);
//       MOTOR_SetSpeed(1,0);
//       pelle(ALLUMER);
// ///////////////////////////////////////////////////////SERVO_SetAngle(0,180); // 2.4ms

//      //souffleuse(ALLUMER);
//       delay(3000);
//       tourneDroit(90);
//       while (!ligneHorizontale())
//       {
//         suivreLigneRecule();
//       }

//       MOTOR_SetSpeed(0,-0.1);
//       MOTOR_SetSpeed(1,-0.1);
//        delay(500);

//       //souffleuse(ETEIN);
//       while(!ligneHorizontale()){
//         suivreLigne();
//       }
// /////////////////////////////////////////////////SERVO_SetAngle(0,0); // 0.54 ms
//       pelle(ETEIN);
//       tourneGauche(90);
//      delay(50);  //attendre que la pelle ferme au complet

//      delay(2000);

//    }

//     delay(500);
//   }