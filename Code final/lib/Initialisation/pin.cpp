#include "pin.h"
#include "LibRobus.h"


void initaliserPin(){
    pinMode(pinPelle, OUTPUT); //pin souffleuse
    pinMode(pinSouffleuse, OUTPUT); //pin pelle
    pinMode(pinBuzzer,OUTPUT);
}