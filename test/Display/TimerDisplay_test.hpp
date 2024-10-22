#pragma once 
#include "Arduino.h"
#include "button.h"
#include "unity.h"
#include "Input.h"
#include "Adafruit_ST7789.h"
#include "TimerDisplay.h"
#include "eeprom_M24C02.h"

const uint8_t dc = 9; 
const uint8_t rst = 10; 
const uint8_t tc = 11; 

const int screen_width{320}; 
const int screen_height{172}; 

Adafruit_ST7789 disp{tc,dc,rst}; 
eeprom::EEProm_M24C02 eeprom_dev(13,14,15,12,&Wire1); //regular eeprom position

Timer t1{0,eeprom_dev}; 
TimerDisplay td{&disp,0,&t1}; 

void test_TimerDisplay_init() 
{
    //init the timerdisp to test for runtime error 
    t1.init(); 

    Input in = Input::none; 
    
    //run for a few seconds 
    auto start_time = millis(); 
    while (start_time + 5000 < millis()); 
    {
        td.Update(); 
    }

}

void DisplayTimer_tester_setup_and_run() 
{
    delay(3000); 
    disp.init(screen_height,screen_width); 
    disp.setRotation(3); 
    disp.fillScreen(0x0000); 

    eeprom_dev.setup(); 
    td.init(); 

    UNITY_BEGIN(); 

    RUN_TEST(test_TimerDisplay_init); 

    UNITY_END(); 
    
}