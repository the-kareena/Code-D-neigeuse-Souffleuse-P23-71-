/////////////////////////////////////////////////////////////////////////////////////////////////////
// Etienne Vibert   Karina Ait Ouazzou  22 octobre 2025
// Librairie pour lire les capteurs de distance GP2D12
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GP2D12.h"
#include <Arduino.h>
#include <LibRobus.h>

void PrintDistance()
{
  
  int capteur1 = (6787 / ROBUS_ReadIR(0)) - 3;

  Serial.print("Distance capteur 1: ");
  Serial.print(capteur1);
  Serial.println(" cm");
  Serial.print(" Valeur brute: ");
  Serial.println(ROBUS_ReadIR(0));

}

int DistanceGauche()
{
  int tension = ROBUS_ReadIR(0);
  if(tension < 88 ){
    return 80;
  }
  int capteurArriere = (6787 / tension - 3);
  return capteurArriere;
}
