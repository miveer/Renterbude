/*

  IN : 5 x Encoder
  OUT: Values over IIC

*/
//#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <Wire.h>

// Encoder 1
#define SW_1  5
#define DT_1  7
#define CLK_1 6

// Encoder 2
#define SW_2  8
#define DT_2  10
#define CLK_2  9

// Encoder 3
#define SW_3  A7
#define DT_3  A1
#define CLK_3 A0

// Encoder 4
#define SW_4  A2
#define DT_4   0
#define CLK_4 A3

// Encoder 5
#define SW_5  1
#define DT_5  3
#define CLK_5 2

Encoder Enc_1(DT_1, CLK_1);
Encoder Enc_2(DT_2, CLK_2);
Encoder Enc_3(DT_3, CLK_3);
Encoder Enc_4(DT_4, CLK_4);
Encoder Enc_5(DT_5, CLK_5);

long position_1  = 0;
long position_2  = 0;
long position_3  = 0;
long position_4  = 0;
long position_5  = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin(8);                // join I2C bus with address #8
  Wire.onRequest(requestEvent); // register event

  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);
  pinMode(SW_3, INPUT);
  pinMode(SW_4, INPUT);
  pinMode(SW_5, INPUT);

  Serial.println("Setup Encoder Test done.");
}

void loop() {

  long newPos_1 = Enc_1.read();
  if (newPos_1 != position_1) {
    position_1 = newPos_1;
  }

  long newPos_2 = Enc_2.read();
  if (newPos_2 != position_2) {
    position_2 = newPos_2;
  }

  long newPos_3 = Enc_3.read();
  if (newPos_3 != position_3) {
    position_3 = newPos_3;
  }

  long newPos_4 = Enc_4.read();
  if (newPos_4 != position_4) {
    position_4 = newPos_4;
  }

  long newPos_5 = Enc_5.read();
  if (newPos_5 != position_5) {
    position_5 = newPos_5;
  }

}

void requestEvent() {
  byte p1 = 0;
  byte p2 = 0;
  byte p3 = 0;
  byte p4 = 0;
  byte p5 = 0;
  byte sw1 = 0;
  byte sw2 = 0;
  byte sw3 = 0;
  byte sw4 = 0;
  byte sw5 = 0;
  int temp = 0;

  p1 = position_1 & 0xFF;
  p2 = position_2 & 0xFF;
  p3 = position_3 & 0xFF;
  p4 = position_4 & 0xFF;
  p5 = position_5 & 0xFF;

  sw1 = !digitalRead(SW_1);
  sw2 = !digitalRead(SW_2);
  temp = analogRead(SW_3);
  if(temp > 400){
    sw3 = 0;
  }else{
    sw3 = 1;
  }
  sw4 = !digitalRead(SW_4);
  sw5 = !digitalRead(SW_5);
  
  /* 10 chars */
//  Wire.write("Hello");
  Wire.write(p1);
  Wire.write(p2);
  Wire.write(p3);
  Wire.write(p4);
  Wire.write(p5);
  Wire.write(sw1);
  Wire.write(sw2);
  Wire.write(sw3);
  Wire.write(sw4);
  Wire.write(sw5);
  
}



