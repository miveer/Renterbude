// Datum 17.05.26

#include <SPI.h>
#include <mcp2515.h>
#include "SevSeg.h"

#define DIV_ADR_OFEN 1
#define DEV_ADR_LIEGE 2

SevSeg sevseg; 
struct can_frame canMsg;
MCP2515 mcp2515(10);

int t1 = 0;
int t2 = 0;

#define DIG_0  2
#define DIG_1  3
#define DIG_2  4 
#define DIG_3  5

#define A   6 
#define B   7 
#define C   A5
#define D   A4
#define E   A3 
#define F   8 
#define G   A2
#define DP  A1

void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();

  ini_seg();

  Serial.println("7-Segment Setup done.");
}

void loop() {
  read_can();
  sevseg.refreshDisplay(); 
}

void read_can(){

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    if(canMsg.can_id != DIV_ADR_OFEN) return;

    t1 =  canMsg.data[0]*100;
    t2 =  canMsg.data[1];
    sevseg.setNumber(t1+t2,2);
  }  

}

void ini_seg(){
  byte numDigits = 4;
  byte digitPins[] = {DIG_0, DIG_1, DIG_2,DIG_3};
  byte segmentPins[] = {A, B, C, D, E, F, G, DP};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; 
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}