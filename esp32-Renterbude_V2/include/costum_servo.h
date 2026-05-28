#pragma once
#include <Arduino.h>
#include <Servo.h>
#include "pinconfig.h"
#include "variablen.h"

extern SemaphoreHandle_t myMutex;

void update_servo(System s, int ventilator){
    if(ventilator == 1){
        myservo_1.write(s.step12_pos);
        myservo_2.write(180-s.step12_pos);
    }
    // if(ventilator == 2){
    //     myservo_3.write(s.step34_pos);
    //     myservo_4.write(180-s.step34_pos);
    // }

}

void update_ventilator(System s){
    analogWrite(VENTILATOR_1,sys.ventilator_speed);
}

void PIDRegler(float T1,float T2,System *s){

    // Empfindlichkeit des PID-Reglers
    float Tn = 1.0;
    float Tv = 1.0;
    float Kp = 1.0;
    // Variablen für P,I,D
    float P,I,D;
    // Zeitkostante 
    float delta_t = 1.05f;


    s->En = T1 - T2;       // T1 = Temperatur_1 = Sollwert hier 
                        // T2 = Temperatur_2 = Istwert
                        // En = Regelabweichung
    // if(xSemaphoreTake(myMutex, portMAX_DELAY) == pdTRUE){                        
        // Berechnugn P I D
        P = s->En - s->en1;
        D = (delta_t / Tn) * s->En;
        I = Tv * (s->En - 2 * s->en1 + s->en2);

        // Berechnung neuer Stellgröße
        s->delta_y = Kp * ( P + D + I);
        s->Yn = s->yn1 + s->delta_y;

        // Speichert alte Werte
        s->en2 = s->en1;
        s->en1 = s->En;
        s->yn1 = s->Yn;

        // Begrenzt Yn auf 100
        if (s->Yn >= 100.0) s->Yn=100.0;
        if (s->Yn <    0.0) s->Yn=  0.0;

    //     xSemaphoreGive(myMutex);
    // }
    Serial.print("value = ");Serial.println(s->Yn);
    
    if(s->Yn < 10.0f){
      s->ventilator_speed = 0.0f;
    }else{
      s->ventilator_speed = map(s->Yn,0,100,0,255);
    }
}

void resetPID(System *s){
//   if(xSemaphoreTake(myMutex, portMAX_DELAY) == pdTRUE){
    s->en2=0.0f;
    s->en1=0.0f;
    s->yn1=0.0f;
    s->En=0.0f;
    s->Yn=0.0f;
    s->delta_y=0.0f; 
    s->ventilator_speed = 0;
//     xSemaphoreGive(myMutex);
//   }
}