#pragma once 
#include "Arduino.h"
#include "button.h"
#include "unity.h"
#include "Input.h"
#include "Adafruit_ST7789.h"
#include "three_button_manager.h"
#include <algorithm>

constexpr int button_1 = 26; 
constexpr int button_2 = 25; 
constexpr int button_3 = 24; 
constexpr unsigned long long_press_length = 1000; 

constexpr uint8_t dc = 9; 
constexpr uint8_t rst = 10; 
constexpr uint8_t tc = 11; 

const int screen_width{320}; 
const int screen_height{172}; 

Adafruit_ST7789 disp{tc,dc,rst}; 
ThreeButtonManager three_input{button_1,button_2,button_3,long_press_length}; 

void test_check_all_button_inputs() 
{   
    //require all button types to be pressed 3 times
    bool c; 
    std::vector<short> small_press_cnt = {0,0,0}; 
    std::vector<short> long_press_cnt = {0,0,0}; 

    while ( *std::min_element(small_press_cnt.begin(),small_press_cnt.end()) < 3 || 
        *std::min_element(long_press_cnt.begin(),long_press_cnt.end()) < 3 )
    {
        int out = static_cast<int>(three_input.Update()); 
        if (out == static_cast<int>(Input::none)) continue;
        if (out >= 3 ) 
        {
            long_press_cnt[out % 3]++; 
        }
        else 
        {
            small_press_cnt[out]++; 
        }

        if (out % 3 == 0)
        {
            disp.print("A"); 
            if (out >= 3) disp.print("-"); 
        }
        else if (out % 3 == 1)
        {
            disp.print("B"); 
            if (out >= 3) disp.print("-"); 
        }
        else 
        {
            disp.print("C"); 
            if (out >= 3) disp.print("-"); 
        }
    }
}   

void test_multicore_input() 
{
    //not sure if I can actually test this 
}

void Input_tester_setup_and_run()
{
    delay(3000);
    disp.init(screen_height,screen_width); 
    disp.setRotation(3); 
    disp.setTextSize(4); 
    disp.setCursor(20,20); 
    three_input.Setup(); 
    disp.fillScreen(0x0000); 

    UNITY_BEGIN();

    RUN_TEST(test_check_all_button_inputs); 
    
    UNITY_END(); 
}
