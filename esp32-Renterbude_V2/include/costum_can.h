#pragma once
#include <Arduino.h>
#include "mcp2515.h"
#include "variablen.h"
#include "pinconfig.h"

extern SPIClass cspi;
extern MCP2515 mcp2515;

void send_can(int adr, float value){

  int ganz = (int)value;                 // Teil vor dem Komma
  int komma = (value - ganz) * 100;     // 2 Nachkommastellen

  if(ganz >= MAX_ANZEIGE_VALUE){
    ganz = MAX_ANZEIGE_VALUE;
  }
  if(komma >= MAX_ANZEIGE_VALUE){
    komma = MAX_ANZEIGE_VALUE;
  }

  canMsg.can_id = adr;
  canMsg.can_dlc = 8;
  canMsg.data[0] = ganz;
  canMsg.data[1] = komma;
  canMsg.data[2] = 0x32;
  canMsg.data[3] = 0xFA;
  canMsg.data[4] = 0x26;
  canMsg.data[5] = 0x8E;
  canMsg.data[6] = 0xBE;
  canMsg.data[7] = 0x86;

  mcp2515.sendMessage(&canMsg);
}