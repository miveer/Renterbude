#pragma once
#include <Arduino.h>
#include "variablen.h"

void print_all_values(System s){

    Serial.println("----------NEW Values------------");
    Serial.println("---Encoder---");
    Serial.print("ENC 1 : ");Serial.println(s.enc1);
    Serial.print("ENC 2 : ");Serial.println(s.enc2);
    Serial.print("ENC 3 : ");Serial.println(s.enc3);
    Serial.print("ENC 4 : ");Serial.println(s.enc4);
    Serial.print("ENC 5 : ");Serial.println(s.enc5);

    Serial.println("---Switches---");
    Serial.print("SW 1 : ");Serial.println(s.sw_enc_1);
    Serial.print("SW 2 : ");Serial.println(s.sw_enc_2);
    Serial.print("SW 3 : ");Serial.println(s.sw_enc_3);
    Serial.print("SW 4 : ");Serial.println(s.sw_enc_4);
    Serial.print("SW 5 : ");Serial.println(s.sw_enc_5);
    
    Serial.println("---Temp&Humi---");
    Serial.print("String 1: ");Serial.println(s.s_temp1);
    Serial.print("humi 1: ");Serial.print(s.hum1);Serial.println(" RH");
    Serial.print("temp o: ");Serial.print(s.temp_ofen);Serial.println(" C");
    Serial.println();
    Serial.print("String 2: ");Serial.println(s.s_temp2);
    Serial.print("humi 2: ");Serial.print(s.hum2);Serial.println(" RH");
    Serial.print("temp l: ");Serial.print(s.temp_liege);Serial.println(" C");

    Serial.println("--------------------------------");
}