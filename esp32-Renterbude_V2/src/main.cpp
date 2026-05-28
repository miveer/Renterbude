#include <Arduino.h>
#include <driver/adc.h>
#include <esp_timer.h>
#include <SPI.h>
#include <Wire.h>
#include "mcp2515.h"

// Costum Libs
#include "variablen.h"
#include "pinconfig.h"
#include "costum_servo.h"
#include "costum_led_warm.h"
#include "costum_dyn_stripes.h"
#include "costum_timer.h"
#include "costum_can.h"
#include "costum_debug.h"
#include "costum_temp.h"
#include "costum_switch.h"



void update_switch_task(void *parameter){
  Serial.println("Erstelle update-Task.");
  
  ini_io();
  Wire.begin(SDA, SCL);

  while(1){
    update_enc_switches(&sys,&con);
    update_led_warm(sys);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void update_tempCan_task(void *parameter){
  Serial.println("Erstelle update_tempCan_task.");

  Serial1.begin(9600, SERIAL_8N1, RxT1, TxT1);
  Serial2.begin(9600, SERIAL_8N1, RxT2, TxT2);
  
  ini_can();
  ini_servo();
  
  while(1){

    update_temp_values(&sys);

    send_can(CAN_ADR_OFEN,sys.temp_ofen);
    send_can(CAN_ADR_LIEGE,sys.temp_liege);

    if(sys.temp_ofen > 30.0f){
      PIDRegler(25.0f,sys.temp_liege,&sys);
      sys.step12_pos = 30;
    }else{
      resetPID(&sys);
      sys.step12_pos = 0;
    }
    update_servo(sys,1);
    update_ventilator(sys);

    vTaskDelay(pdMS_TO_TICKS(1050));
  }
}

void update_dynled_task(void *paramenter){
  Serial.println("Erstelle update_dynled_task.");
  ini_dyn_stripes();

  RgbColor c_w = RgbColor(255,255,255);
  RgbColor c_gelb = RgbColor(255,255,0);
  RgbColor c_red = RgbColor(0,255,0);
  RgbColor c_blue = RgbColor(0,0,255);

  while(1){

    if(sys.state_dyn_stripe_vorne){

      switch(sys.programm_vorne){
        case 0:
          usercolor(&stripe_1,sys,con,c_w);
          usercolor(&stripe_2,sys,con,c_w);
        break;
        
        case 1:
          vunormal(&stripe_1,sys,con,0);
          vunormal(&stripe_2,sys,con,1);
        break;
    
        case 2:
          movingRainbow(&stripe_1,sys,con,0);
          movingRainbow(&stripe_2,sys,con,1);
        break;
        
        case 3:
          usercolor(&stripe_1,sys,con,c_gelb);
          usercolor(&stripe_2,sys,con,c_gelb);
        break;
        
        case 4:
          usercolor(&stripe_1,sys,con,c_blue);
          usercolor(&stripe_2,sys,con,c_blue);
        break;

        case 5:
          usercolor(&stripe_1,sys,con,c_red);
          usercolor(&stripe_2,sys,con,c_red);
        break;

        default:
          break;
      }
    }else{
      aus(&stripe_1,con);
      aus(&stripe_2,con);
    }

    if(sys.state_dyn_stripe_mitte){
      
      switch(sys.programm_mitte){
        case 0:
          usercolor(&stripe_3,sys,con,c_w);
          usercolor(&stripe_4,sys,con,c_w);
        break;
        
        case 1:
          vunormal(&stripe_3,sys,con,2);
          vunormal(&stripe_4,sys,con,3);
        break;
    
        case 2:
          movingRainbow(&stripe_3,sys,con,2);
          movingRainbow(&stripe_4,sys,con,3);
        break;
        
        case 3:
          usercolor(&stripe_3,sys,con,c_gelb);
          usercolor(&stripe_4,sys,con,c_gelb);
        break;
        
        case 4:
          usercolor(&stripe_3,sys,con,c_blue);
          usercolor(&stripe_4,sys,con,c_blue);
        break;

        case 5:
          usercolor(&stripe_3,sys,con,c_red);
          usercolor(&stripe_4,sys,con,c_red);
        break;

        default:
          break;
      }
    }else{
      aus(&stripe_3,con);
      aus(&stripe_4,con);
    }
    
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  
}



void setup(){

  Serial.begin(115200);
  Serial.println("Erstelle Tasks.");

  xTaskCreatePinnedToCore(update_switch_task , "update_switches_task", STACKSIZE, NULL, PRIO_1, &Update_Task_Hanlder , CORE_0);
  xTaskCreatePinnedToCore(update_tempCan_task, "update_tempCan_task" , STACKSIZE, NULL, PRIO_1, &TempCan_Task_Hanlder, CORE_0);
  xTaskCreatePinnedToCore(update_dynled_task , "update_dynled_task"  , STACKSIZE, NULL, PRIO_2, &DynLed_Task_Hanlder , CORE_1);

  delay(1000);
  Serial.println("Setup done.");
}



void loop(){

  // print_all_values(sys);
  // delay(666);

}

