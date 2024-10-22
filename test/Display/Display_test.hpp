#pragma once
#include <Arduino.h>
#include "Adafruit_ST7789.h"
#include "button.h"
#include "Timer.h"
#include "display_utilities.h"
#include "unity.h"
#include "bitmaps.h"

//consts 

const uint8_t dc = 9; 
const uint8_t rst = 10; 
const uint8_t tc = 11; 

const int screen_width{320}; 
const int screen_height{172}; 


Adafruit_ST7789 disp{tc,dc,rst}; 

void test_display_wakeup() 
{ 
    //clc disp
    disp.fillScreen(0x5b2d); 
    disp.fillRoundRect( 20,20,280,100,5,0x5cad); 
}

void test_draw_utils () 
{ 
    disp.fillScreen(0x0000); 
    utilities::Coordinate<int> location = {30,30}; 
    utilities::Coordinate<int> other_location = {60, 30}; 
    display_utilities::draw_play_sign(&disp,location);
    display_utilities::draw_pause_sign(&disp,other_location); 
    delay(2000);  
}

void test_timer_count_down() 
{ 
    disp.fillScreen(0); 
    //text size 
    disp.setTextSize(3); 

    GFXcanvas16 time_canvas{40,20}; 
    time_canvas.setTextSize(3); 

    //timer loop 
    auto current_time = millis(); 
    while (millis() < current_time + 8000)
    {
        //clear screen 
        time_canvas.fillScreen(0x0000);
        time_canvas.setCursor(0,0); 
        time_canvas.print((millis() - current_time)/1000); 
        disp.drawRGBBitmap(30,30,time_canvas.getBuffer(),time_canvas.width(),time_canvas.height()); 
    }
    delay(1000); 
}

void test_timer_placement() 
{ 
    //clear screen
    disp.fillScreen(0x0000); 

    GFXcanvas16 timer_1{200,40}; 
    GFXcanvas16 timer_2{200,40};
    GFXcanvas16 timer_3{200,40};

    timer_1.setTextSize(3); 
    timer_2.setTextSize(3); 
    timer_3.setTextSize(3); 

    timer_1.fillScreen(0x2945); //red
    timer_2.fillScreen(0x2945); //orange
    timer_3.fillScreen(0x2945); //porple

    timer_1.setCursor(5,10); 
    timer_2.setCursor(5,10); 
    timer_3.setCursor(5,10); 

    timer_1.print("10:40"); 
    timer_2.print("9:45"); 
    timer_3.print("1:00");

    display_utilities::draw_play_sign(&timer_1,{140,10}); 
    display_utilities::draw_pause_sign(&timer_2,{140,10}); 
    display_utilities::draw_play_sign(&timer_3,{140,10}); 

    //draw display bitmaps 
    disp.drawRGBBitmap(30,10,timer_1.getBuffer(),timer_1.width(),timer_1.height());
    disp.drawRGBBitmap(30,disp.height()/2 - timer_2.height()/2,timer_2.getBuffer(),timer_2.width(),timer_2.height());
    disp.drawRGBBitmap(30,disp.height() - 50,timer_3.getBuffer(),timer_3.width(),timer_3.height());

    auto current_time = millis() ; 
    while (millis() < current_time + 8000)
    {
        timer_1.fillScreen(0x2945);
        timer_1.setCursor(5,10); 
        timer_1.print("10:4");
        timer_1.print((millis() - current_time)/1000); 
        display_utilities::draw_pause_sign(&timer_1,{140,10}); 
        disp.drawRGBBitmap(30,10,timer_1.getBuffer(),timer_1.width(),timer_1.height());
    }

    disp.drawXBitmap(280,150,duck_00.bitmap,duck_00.width,duck_00.height,0xec02); 

    //test using base class 
    Adafruit_GFX* disp_pointer = &disp; 
    disp_pointer->fillCircle(
        40,
        40,
        30,
        0x8305
    ); 

    disp_pointer->drawRGBBitmap(30,10,timer_1.getBuffer(),timer_1.width(),timer_1.height());
}

void Display_tester_setup_and_run()
{
    delay(3000);
    disp.init(screen_height,screen_width); 
    disp.setRotation(3); 

    UNITY_BEGIN();

    // RUN_TEST(test_display_wakeup); 
    // RUN_TEST(test_draw_utils); 
    // RUN_TEST(test_timer_count_down); 
    RUN_TEST(test_timer_placement); 



    UNITY_END(); 
}
