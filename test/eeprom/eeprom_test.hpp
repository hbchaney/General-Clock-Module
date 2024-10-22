#pragma once
#include <Arduino.h>
#include "eeprom_M24C02.h"
#include "unity.h"

eeprom::EEProm_M24C02 eeprom_dev(13,14,15,12,&Wire1); //regular eeprom position

void test_write_byte() 
{
    uint8_t value {8}; 
    TEST_ASSERT_TRUE(eeprom_dev.write_byte(15,value)); //writing 8 to eeprom at position 15
}

void test_read_byte()
{
    //call this test after the write 
    uint8_t value = 0; 
    TEST_ASSERT_TRUE(eeprom_dev.read_byte(15,value)); 
    TEST_ASSERT_EQUAL(8,value); 
}

//testing eeprom with other data types 

void test_write() 
{
    int v = 15286; 
    TEST_ASSERT_TRUE(eeprom_dev.write(20,v)); //should write four bytes 
    //read the four bytes seperately 
    uint8_t b0; 
    uint8_t b1; 
    uint8_t b2; 
    uint8_t b3; 
    TEST_ASSERT_TRUE(eeprom_dev.read_byte(20,b0)); 
    TEST_ASSERT_TRUE(eeprom_dev.read_byte(21,b1)); 
    TEST_ASSERT_TRUE(eeprom_dev.read_byte(22,b2)); 
    TEST_ASSERT_TRUE(eeprom_dev.read_byte(23,b3));
    TEST_ASSERT_EQUAL(v >> 24 & 0xff, b3); 
    TEST_ASSERT_EQUAL(v >> 16 & 0xff, b2); 
    TEST_ASSERT_EQUAL(v >> 8 & 0xff, b1); 
    TEST_ASSERT_EQUAL(v & 0xff,b0 );  
}

void test_read()
{
    int v = eeprom_dev.read<int>(20); //read from 20
    TEST_ASSERT_EQUAL(15286,v); 
}

void eeprom_setup_and_run()
{
    delay(3000); // 
    eeprom_dev.setup(); 

    UNITY_BEGIN(); 

    RUN_TEST(test_write_byte); 
    RUN_TEST(test_read_byte); 
    RUN_TEST(test_write); 
    RUN_TEST(test_read); 

    UNITY_END(); 
}