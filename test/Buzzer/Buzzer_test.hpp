#pragma once 
#include <Arduino.h>
#include "unity.h"
#include "Buzzer.h"

constexpr uint8_t PIN = 29;
std::vector<ulong> seq {100,100,100,300}; 

SequenceBuzzer buzzie(29,seq,5); 


void test_buzzer_init() 
{
    buzzie.init();  
}

void test_vol_levels()
{
    //set 250
    // buzzie.set_buzzer_vol(250); 
    // buzzie.test_buzz(); 
    // buzzie.set_buzzer_vol(200); 
    // buzzie.test_buzz(); 
    // buzzie.set_buzzer_vol(150); 
    // buzzie.test_buzz(); 
    buzzie.set_buzzer_vol(5); 
    buzzie.test_buzz(); 
    buzzie.test_buzz(); 
}

void test_buzzer_start_stop()
{
    pinMode(21,OUTPUT); 
    digitalWrite(21,HIGH); 
    buzzie.start_buzzing();
    auto start = millis();
    while ((start + 5000) > millis())
    {
        buzzie.Update(); 
        if (buzzie.get_vol_output()) 
        {
            digitalWrite(21,LOW); 
        }
        else 
        {
            digitalWrite(21,HIGH); 
        }
    }
    buzzie.stop_buzzing();
    buzzie.start_buzzing(); 
    while ((start + 5000) > millis())
    {
        buzzie.Update(); 
        if (buzzie.get_vol_output()) 
        {
            digitalWrite(21,LOW); 
        }
        else 
        {
            digitalWrite(21,HIGH); 
        }
    }
    buzzie.stop_buzzing(); 
}   

void buzzer_setup_and_run()
{
    delay(3000); // 
    buzzie.init(); 
    Serial.begin(); 
    UNITY_BEGIN(); 

    // RUN_TEST(test_vol_levels); 
        
    // RUN_TEST(test_buzzer_init); 
    RUN_TEST(test_buzzer_start_stop); 

    UNITY_END(); 
}