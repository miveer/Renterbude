#pragma once
#include <Arduino.h>
#include "Wire.h"
#include <Servo.h>

#define IIC_ADR_ENC 8
#define IIC_REQ_BYTES 10

#define MAX_PROGRAMMS 5

#define CAN_ADR_OFEN 1
#define CAN_ADR_LIEGE 2
#define MAX_ANZEIGE_VALUE 99

#define CORE_0 0
#define CORE_1 1
#define PRIO_1 1
#define PRIO_2 2
#define STACKSIZE 8192

// CAN Massage für Display
struct can_frame canMsg;

// Task handler
TaskHandle_t Update_Task_Hanlder;
TaskHandle_t TempCan_Task_Hanlder;
TaskHandle_t DynLed_Task_Hanlder;


struct System
{
    int enc1 = 0;                      // Enc 1 Value (0-255) 
    int enc2 = 0;                      // Enc 2 Value (0-255)
    int enc3 = 0;                      // Enc 3 Value (0-255)
    int enc4 = 0;                      // Enc 4 Value (0-255)
    int enc5 = 0;                      // Enc 5 Value (0-255)

    uint8_t step12_pos = 0;                // Servo Motor position 0-180°
    uint8_t step34_pos = 0;                // Servo Motor position 0-180°

    int hell_dyn_vorne = 0;            // Helligkeit
    int hell_dyn_mitte = 0;            // Helligkeit

    int hell_vorne  = 0;               // Helligkeit Value
    int hell_mitte  = 0;               // Helligkeit Value
    int hell_hinten = 0;               // Helligkeit Value

    bool sw_enc_1 = 0;                 // State Switch Encoder 1
    bool sw_enc_2 = 0;                 // State Switch Encoder 2
    bool sw_enc_3 = 0;                 // State Switch Encoder 3
    bool sw_enc_4 = 0;                 // State Switch Encoder 4
    bool sw_enc_5 = 0;                 // State Switch Encoder 5

    int programm_vorne = 0;            // Merkt sich Programm vorne Stripe 1&2
    int programm_mitte = 0;            // Merkt sich Programm vorne Stripe 3&4
    bool state_dyn_stripe_vorne = 0;   // An / Aus dyn Stripe 1&2
    bool state_dyn_stripe_mitte = 0;   // An / Aus dyn Stripe 3&4
    bool state_stripe_vorne  = 0;      // An / Aus LED Stripe vorne
    bool state_stripe_mitte  = 0;      // An / Aus LED Stripe mitte
    bool state_stripe_hinten = 0;      // An / Aus LED Stripe hinten

    String s_temp1 = "";               // Empfängt String mit Temp 1
    String s_temp2 = "";               // Empfängt String mit Temp 2
    float hum1 = 0;                    // Luftfeuchtigkeit 1 als float
    float temp_ofen = 0;               // Temperatur 1       als float
    float hum2 = 0;                    // Luftfeuchtigkeit 2 als float
    float temp_liege = 0;              // Temperatur 2       als float

    // Diese Werte müssen gespeichert werden
    float En=0.0,en1=0.0,en2=0.0;
    float Yn=0.0,yn1=0.0;
    float delta_y = 0.0f;

    int ventilator_speed = 0;

}sys;

struct Config
{
    // Anzahl der LED pro Streifen
    int led_count = 33;


    // PWM Eigenschaften
    const int freq = 5000;
    const int resolution = 8;

    // Micro settings
    int m_offset = 1995;
    int micro_intense =100;
    
    // Streifen settings
    int delay_time_dyn_stripe_1 = 50;
    int delay_time_dyn_stripe_3 = 50;

}con;







