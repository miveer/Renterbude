#pragma once
#include <Arduino.h>
#include "variablen.h"

// extern SemaphoreHandle_t myMutex;

void update_enc_switches(System *s,Config *c){

    static int counter_v = 0;
    static int counter_m = 0;
    static int last_state_dyn_vorne = 0;
    static int last_state_dyn_mitte = 0;

    // Update IIC Encoder Values
    Wire.requestFrom(IIC_ADR_ENC, IIC_REQ_BYTES);    
    s->enc1 = Wire.read();
    s->enc2 = Wire.read();
    s->enc3 = Wire.read();
    s->enc4 = Wire.read();
    s->enc5 = Wire.read();
    s->sw_enc_1 = Wire.read();
    s->sw_enc_2 = Wire.read();
    s->sw_enc_3 = Wire.read();
    s->sw_enc_4 = Wire.read();
    s->sw_enc_5 = Wire.read();


    // Auswertung Encoder Values und Switch states

    // Helligkeit dyn Stripes
    s->hell_dyn_vorne = s->enc1;
    s->hell_dyn_mitte = s->enc2;

    // Enc Switch 1
    if(s->sw_enc_1 && last_state_dyn_vorne){
        counter_v++;
    }
    // Short push
    if(counter_v > 0 && counter_v < 3 && !last_state_dyn_vorne){
        s->programm_vorne++;
        if(s->programm_vorne > MAX_PROGRAMMS) s->programm_vorne = 0;
        counter_v = 0;
        Serial.print("Short push S1 prog_vor : ");Serial.println(s->programm_vorne);
    }
    // Long push
    if(counter_v >= 3){
        s->state_dyn_stripe_vorne = s->state_dyn_stripe_vorne^1;
        counter_v = 0;
        Serial.print("Long Push S1 s->state_dyn_stripe_vorne : ");Serial.println(s->state_dyn_stripe_vorne);
    }

    // Enc Switch 2
    if(s->sw_enc_2 && last_state_dyn_mitte){
        counter_m++;
    }
    // Short push
    if(counter_m > 0 && counter_m < 3 && !last_state_dyn_mitte){
        s->programm_mitte++;
        if(s->programm_mitte > MAX_PROGRAMMS) s->programm_mitte = 0;
        counter_m = 0;
        Serial.print("Short push S3 prog_mitte : ");Serial.println(s->programm_mitte);
    }
    // Long push
    if(counter_m >= 3){
        s->state_dyn_stripe_mitte = s->state_dyn_stripe_mitte^1;
        counter_m = 0;
        Serial.print("Long Push S2 s->state_dyn_stripe_mitte : ");Serial.println(s->state_dyn_stripe_mitte);
    }


    // Enc Switch 3
    if(s->sw_enc_3){
        s->state_stripe_vorne = s->state_stripe_vorne^1;
    }

    // Enc Switch 4
    if(s->sw_enc_4){
        s->state_stripe_mitte = s->state_stripe_mitte^1;
    }

    // Enc Switch 5
    if(s->sw_enc_5){
        s->state_stripe_hinten = s->state_stripe_hinten^1;
    }

    // Wenn Warmweiß aus ist wert vom Poti -> speed dyn stripe 1&2
    if(s->state_stripe_vorne){
        s->hell_vorne = s->enc3;
    }else{
        s->hell_vorne = 0;
        c->delay_time_dyn_stripe_1 = map(s->enc3,0,255,0,50);
    }

    // Wenn Warmweiß aus ist wert vom Poti -> speed dyn stripe 3&4
    if(s->state_stripe_mitte){
        s->hell_mitte = s->enc4;
    }else{
        s->hell_mitte = 0;
    }

    // Wenn Warmweiß aus ist wert vom Poti -> intense Micro
    if(s->state_stripe_hinten ){
        s->hell_hinten = s->enc5;
    }else{
        s->hell_hinten = 0;
        c->micro_intense = map(s->enc5,0,255,0,100);
    }

    // speichert alten Switch Status
    last_state_dyn_vorne = s->sw_enc_1;
    last_state_dyn_mitte = s->sw_enc_2;

}
