#pragma once
#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>
#include "variablen.h"
#include "pinconfig.h"


// Regenbogeneffect
RgbColor Wheel(byte WheelPos){

    if(WheelPos < 85){
        return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if(WheelPos < 170){
        return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else{
        WheelPos -= 170;
        return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

// Funktion ließt Micro/AUX Signal ein
int check_micro() {
  int tem = analogRead(MICRO);
  //Serial.print("micro = ");Serial.println(tem);
  return tem;
  // int temp[10];
  // int tem = 0;
  
  // for (int i = 0 ; i < 10 ; i++) {
  //  temp[i] = analogRead(MICRO);
  // }
  // for (int i = 0 ; i < 10 ; i++) {
  //   tem += temp[i];
  // }
  // tem = tem / 10;
  // Serial.print("micro = ");Serial.println(tem);
  // return tem;
}

////////////////////////////////////////////////////////////////////////
//                              AUS                                   //
////////////////////////////////////////////////////////////////////////

template <class T> void aus(T * stripe,Config c) {
  static unsigned long ntime = millis();
 
  if (millis() - ntime >= 100){
    for ( int i = 0 ; i < c.led_count; i++) {
      (*stripe).SetPixelColor(i, RgbColor(0, 0, 0));
    }
    (*stripe).Show();
    ntime = millis();
  }

}

template <class T> void usercolor(T * stripe, System s, Config c, RgbColor color) {
  static unsigned long ntime = millis();
 
  if (millis() - ntime >= 100){
    for (uint16_t i = 0; i < c.led_count ; i++) {
        (*stripe).SetPixelColor(i, color);
    }
    
    (*stripe).SetBrightness(s.hell_dyn_vorne);
    (*stripe).Show();
    
    ntime = millis();
 }

}

unsigned long ntimes[4] = {0};
bool durchlauf[4] = {0};

template <class T> void strobo(T * stripe,System s, Config c, int nr){

  if (millis() - ntimes[nr] >= c.delay_time_dyn_stripe_1)
  {

    RgbColor color = (durchlauf[nr] = !durchlauf[nr]) ? RgbColor(255,255,255) : RgbColor(0, 0, 0);
    for (uint16_t i = 0; i < c.led_count; i++) {
      (*stripe).SetPixelColor(i, color);
    }

    nr >=2 ? (*stripe).SetBrightness(sys.hell_dyn_mitte): (*stripe).SetBrightness(sys.hell_dyn_vorne);
    (*stripe).Show();
    ntimes = millis();
  }
}

#define SAMPLES 20
#define PEAK_FALL 15
unsigned long ntimevu[4] = {0};
int volJe[4][SAMPLES] = {0};
int lvlStripe[4] = {0};
int dotcount[4] = {0};
int volCount[4] = {0};
int peak[4] = {0};

uint16_t minLvlAvg[4] = {512,512,512,512}; 
uint16_t maxLvlAvg[4] = {1,1,1,1};


template <class T> void vunormal(T * stripe, System s,Config c, int nr){

  if (millis() - ntimevu[nr] >= 20 ) {

    RgbColor color;
    float scale = 0.0;
    float n = 0;
    uint16_t minLvl = 0; 
    uint16_t maxLvl = 0;
    int height = 0;
    int value = 0;

    int mic = check_micro();

    scale = c.micro_intense / 100.0;
    n = abs(mic - c.m_offset);
    n = n * scale ;
    value = (int)n;

    lvlStripe[nr] = ((lvlStripe[nr] * 7) + value) >> 3;

    int nenner = (long)(maxLvlAvg[nr] - minLvlAvg[nr]);
    int zaehler = c.led_count * (lvlStripe[nr] - minLvlAvg[nr]);

    // Serial.print("micro = ");Serial.print(mic);
    // Serial.print(", nenner = ");Serial.print(nenner);
    // Serial.print(" , zaehler= ");Serial.print(zaehler);
    // Serial.print(" , maxavg = ");Serial.print(maxLvlAvg[nr]);
    // Serial.print(" , minavg = ");Serial.print(minLvlAvg[nr]);

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = zaehler / nenner;
    //Serial.print(" , height = ");Serial.println(height);
    if (height < 0L)       
      height = 0;     // Clip output
    else if (height > c.led_count) 
      height = c.led_count;
    
      if (height > peak[nr]) 
      peak[nr] = height; // Keep 'peak' dot at top

    // Color pixels based on rainbow gradient
    for (int i = 0; i < c.led_count ; i++) {
      if (i >= height) {
        (*stripe).SetPixelColor(i, RgbColor(0, 0, 0));
      }
      else {
        color = Wheel(map(i, 0, c.led_count - 1, 30, 150));
        (*stripe).SetPixelColor(i, color);
      }
    }

    // Draw peak dot
    if (peak[nr] > 0 && peak[nr] <= c.led_count - 1) {
      RgbColor color_peak  = Wheel(map(peak[nr], 0, c.led_count - 1, 30, 150));
      (*stripe).SetPixelColor(peak[nr], color_peak);
    }

    // Wählt richtige Helligkeit
    nr >=2 ? (*stripe).SetBrightness(sys.hell_dyn_mitte): (*stripe).SetBrightness(sys.hell_dyn_vorne);
    
    (*stripe).Show(); // Update strip

    // Every few frames, make the peak pixel drop by 1:
    if (++dotcount[nr] >= PEAK_FALL) { //fall rate
      if (peak[nr] > 0) peak[nr]--;
      dotcount[nr] = 0;
    }

    volJe[nr][volCount[nr]] = n;                      // Save sample for dynamic leveling
    if (++volCount[nr] >= SAMPLES) volCount[nr] = 0; // Advance/rollover sample counter

    // Get volume range of prior frames
    minLvl = maxLvl = volJe[0][nr];
    for (int i = 0; i < SAMPLES; i++) {
      if (volJe[nr][i] < minLvl)      minLvl = volJe[nr][i];
      else if (volJe[nr][i] > maxLvl) maxLvl = volJe[nr][i];
    }

    if ((maxLvl - minLvl) < c.led_count) 
        maxLvl = minLvl + c.led_count;
    
    minLvlAvg[nr] = (minLvlAvg[nr] * 63 + minLvl) >> 6;
    maxLvlAvg[nr] = (maxLvlAvg[nr] * 63 + maxLvl) >> 6;
    
    // falls lange ruhig ist division durch Null verhindern
    if((long)(maxLvlAvg[nr] - minLvlAvg[nr]) == 0){
      maxLvlAvg[nr] += 1;
    }

    ntimevu[nr] = millis();
  }
}

unsigned long ntimemr[4] = {0};
int step[4] = {0};

template <class T> void movingRainbow(T * stripe, System s,Config c,int nr)
{

  if (millis() - ntimemr[nr] >= c.delay_time_dyn_stripe_1 ) {
    for ( int i = 0; i < c.led_count ; i++) {
      RgbColor color = Wheel(map(i, 0, c.led_count - 1, 30, 150));
      (*stripe).SetPixelColor(((i + step[nr]) % c.led_count), color);
    }

    (*stripe).SetBrightness(s.hell_dyn_vorne);
    (*stripe).Show();

    // updatet Position nach aufruf
    step[nr]++;
    if (step[nr] > c.led_count)
      step[nr] = 0;
    
    ntimemr[nr] = millis();
  }
}