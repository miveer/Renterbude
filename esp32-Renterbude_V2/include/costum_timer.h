#pragma once
#include <Arduino.h>

#define TO_MS 1000

void IRAM_ATTR onTimer0() {
  // Diese Funktion wird alle MILLI_SEC_DEBUGG aufgerufen

}

void IRAM_ATTR onTimer1() {
  // Diese Funktion wird alle MILLI_SEC_DEBUGG aufgerufen

}

void IRAM_ATTR onTimer2() {
  // Diese Funktion wird alle MILLI_SEC_DECODER aufgerufen

}

void init_timer(hw_timer_t *t,int num, int time_ms, void(*func)()){
  // Timer sauber stoppen
  // Setze den Timer: Timer 0, auf eine Frequenz von 10Hz (100ms Intervall)
  t = timerBegin(num, 80, true);              // Timer0, Prescaler von 80 (80 MHz / 80 = 1 MHz)
  timerAttachInterrupt(t, func, true);        // Interrupt beim Auslösen
  timerAlarmWrite(t, time_ms * TO_MS, true);  // 100ms (100000 Mikrosekunden)
  timerAlarmEnable(t);                        // Timer aktivieren
}