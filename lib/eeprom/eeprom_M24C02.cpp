#include "eeprom_M24C02.h"
#include "DebugLog.h"


eeprom::EEProm_M24C02::EEProm_M24C02(uint8_t in_E0, uint8_t in_E1, uint8_t in_E2, uint8_t wb_pin, TwoWire* Wire_value) : 
    E0{in_E0}, E1{in_E1}, E2{in_E2}, wb{wb_pin}
{
    i2c_dev = new Adafruit_I2CDevice(address, Wire_value); 
}

eeprom::EEProm_M24C02::~EEProm_M24C02()
{
    delete i2c_dev; 
}

bool eeprom::EEProm_M24C02::setup()
{
    pinMode(E0,OUTPUT); 
    pinMode(E1,OUTPUT); 
    pinMode(E2,OUTPUT); 
    //write blocking pin (turned off when not writing)
    pinMode(wb,OUTPUT); 
    //E0-E2 customize the i2c address but because there is no other i2c (for now)
    //these can be set low 
    digitalWrite(E0,LOW); 
    digitalWrite(E1,LOW); 
    digitalWrite(E2,LOW); 
    digitalWrite(wb,HIGH); 

    return i2c_dev->begin(); 

}

bool eeprom::EEProm_M24C02::read_byte(uint8_t addr, uint8_t& value)
{ 
    uint8_t buffer[1] = {addr}; 
    uint8_t out_buff[1] = {0x0}; 
    bool check = i2c_dev->write_then_read(buffer,1,out_buff,1); 
    if (!check) 
    {
        value = 0x0; 
        LOG_INFO("ERROR when reading byte"); 
        return check; 
    }
    value = out_buff[0]; 
    return check; 

}

bool eeprom::EEProm_M24C02::write_byte(uint8_t addr, const uint8_t value)
{
    digitalWrite(wb,LOW); 
    uint8_t buffer[2] = {addr, value}; 
    bool check = i2c_dev->write(buffer,2); 

    uint8_t timeout = 100;
    while (timeout--) {
    if (i2c_dev->detected())
    {

        digitalWrite(wb,HIGH); 
        return true;
    }
    delay(1);
    }

    digitalWrite(wb,HIGH); 
    return check; 
}

bool eeprom::EEProm_M24C02::detect_helper()
{
    return i2c_dev->detected(); 
}