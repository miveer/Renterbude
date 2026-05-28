#pragma once
#include <Arduino.h> 
#include "variablen.h"
#include <SPI.h>
#include "mcp2515.h"
#include <NeoPixelBrightnessBus.h>


// versuch um farben zu tauschen.
// typedef NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> RTM1;
// typedef NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt1800KbpsMethod> RTM2;
// typedef NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt2800KbpsMethod> RTM3;
// typedef NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt3800KbpsMethod> RTM4;
typedef NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp32Rmt0800KbpsMethod> RTM1;
typedef NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp32Rmt1800KbpsMethod> RTM2;
typedef NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp32Rmt2800KbpsMethod> RTM3;
typedef NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp32Rmt3800KbpsMethod> RTM4;


// IIC - Encoder Values ////////////////////
#define SDA 10
#define SCL  9

// Serial Tx für Temperatur ///////////////
#define RxT1 5
#define TxT1 4

#define RxT2 7
#define TxT2 6

// CAN - Temperaturen Anzeigen ////////////
#define C_CLK   14
#define C_MOSI  13
#define C_MISO  12  
#define C_CS    11

// Micro //////////////////////////////////
#define MICRO 1

// Stepper Motoren Ventilator 1 ///////////
#define STEP1 39
#define STEP2 38
#define STEP3 37
#define STEP4 36

// Ventilator PWM Ausgang /////////////////
#define VENTILATOR_1 41
#define VENTILATOR_2 40

// LED Streifen Warmweiß //////////////////
#define LED_VORNE   48
#define LED_MITTE   45
#define LED_HINTEN  35

// dyn LED Streifen ///////////////////////
#define LED_DYN_VORNE_1 15
#define LED_DYN_VORNE_2 16
#define LED_DYN_MITTE_3 17
#define LED_DYN_MITTE_4 18

#define KHZ_18 18000 

RTM1 stripe_1(con.led_count,LED_DYN_VORNE_1);
RTM2 stripe_2(con.led_count,LED_DYN_VORNE_2);
RTM3 stripe_3(con.led_count,LED_DYN_MITTE_3);
RTM4 stripe_4(con.led_count,LED_DYN_MITTE_4);

// CAN ////////////////////////////////////
#define MHZ8 8000000
SPIClass cspi(HSPI);
MCP2515 mcp2515(C_CS,MHZ8,&cspi);

// Servo
Servo myservo_1;
Servo myservo_2;
// Servo myservo_3;
// Servo myservo_4;

///////// Servo //////////////////////////
void ini_servo(){
  myservo_1.attach(STEP1);  
  myservo_2.attach(STEP2);
  // myservo_3.attach(STEP3);  
  // myservo_4.attach(STEP4);  
}

// CAN Config /////////////////////////////
void ini_can(){
  cspi.begin(C_CLK,C_MISO,C_MOSI,C_CS);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
}

// Stripes Config /////////////////////////
void ini_dyn_stripes(){
    
  stripe_1.Begin();
  stripe_2.Begin();
  stripe_3.Begin();
  stripe_4.Begin();

  for(int i=0 ; i<con.led_count ; i++){
    stripe_1.SetPixelColor(i, RgbColor(  0,  0,  0));
    stripe_2.SetPixelColor(i, RgbColor(  0,  0,  0));
    stripe_3.SetPixelColor(i, RgbColor(  0,  0,  0));
    stripe_4.SetPixelColor(i, RgbColor(  0,  0,  0));
  }
  
  stripe_1.SetBrightness(50);
  stripe_2.SetBrightness(50);
  stripe_3.SetBrightness(50);
  stripe_4.SetBrightness(50);

  stripe_1.Show();
  stripe_2.Show();
  stripe_3.Show();
  stripe_4.Show();    
}

// IO Config /////////////////////////////////////////
void ini_io(){

  pinMode(MICRO,INPUT);

  pinMode(STEP1 ,OUTPUT);
  pinMode(STEP2 ,OUTPUT);
  pinMode(STEP3 ,OUTPUT);
  pinMode(STEP4 ,OUTPUT);

  pinMode(VENTILATOR_1 ,OUTPUT);
  pinMode(VENTILATOR_2 ,OUTPUT);
  
  pinMode(LED_VORNE ,OUTPUT);
  pinMode(LED_MITTE ,OUTPUT);
  pinMode(LED_HINTEN ,OUTPUT);

  analogWriteFrequency(KHZ_18);


}

