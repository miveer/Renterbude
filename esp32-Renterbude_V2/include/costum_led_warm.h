#pragma once
#include <Arduino.h>
#include "pinconfig.h"
#include "variablen.h"

void update_led_warm(System s){
    analogWrite(LED_VORNE,s.hell_vorne );
    analogWrite(LED_MITTE,s.hell_mitte);
    analogWrite(LED_HINTEN,s.hell_hinten);
}

