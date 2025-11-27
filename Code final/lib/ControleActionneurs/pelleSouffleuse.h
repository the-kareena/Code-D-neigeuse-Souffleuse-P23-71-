#include <Arduino.h>
#include <LibRobus.h>
#include "pin.h"

#define ALLUMER 1
#define ETEIN 0



const int ESC_MIN = 45;   // ≈ 1000 µs (ESC OFF)
const int ESC_MAX = 103;  // ≈ 2000 µs (ESC FULL)

const uint8_t ESC_pin = 0; // la pin a laquelle le ESC est branche 

void pelle(bool etat);

void pelleArret();

void souffleuse(bool etat);

