#pragma once
#include <Arduino.h>
#include "variablen.h"

// extern SemaphoreHandle_t myMutex;

void update_temp_values(System *s){

    // Update Temperatur 1
    if(Serial1.available()){
      s->s_temp1 = Serial1.readStringUntil('\n'); 
      s->hum1 = s->s_temp1.substring(2,7).toFloat();
      s->temp_ofen = s->s_temp1.substring(10,15).toFloat();
    }
    
    // Update Temperatur 2
    if(Serial2.available()){
      s->s_temp2 = Serial2.readStringUntil('\n'); 
      s->hum2 = s->s_temp2.substring(2,7).toFloat();
      s->temp_liege = s->s_temp2.substring(10,15).toFloat();
    }

}