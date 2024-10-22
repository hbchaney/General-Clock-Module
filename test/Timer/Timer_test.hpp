#pragma once
#include "unity.h"
#include <Arduino.h>
#include "Input.h"
#include "Timer.h"


eeprom::EEProm_M24C02 eeprom_dev(13,14,15,12,&Wire1); //regular eeprom position

Timer t1{0,eeprom_dev};

void test_basic_functions()
{
    //testing basic functions on the timer 
    // t1.manual_save(20,10); 
    auto st = t1.get_state(); 
    TEST_ASSERT_EQUAL(st,state::RESET); 
    TEST_ASSERT_EQUAL(t1.get_minutes(),20); 
    TEST_ASSERT_EQUAL(t1.get_seconds(),10); 
}

void test_timing_functions()
{

}

void Timer_tester_setup_and_run() 
{
    delay(3000); 

    eeprom_dev.setup(); 
    t1.init(); 

    UNITY_BEGIN(); 

    RUN_TEST(test_basic_functions); 

    UNITY_END(); 
    
}