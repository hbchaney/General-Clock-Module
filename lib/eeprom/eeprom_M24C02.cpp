#include "eeprom_M24C02.h"
#include "DebugLog.h"


eeprom::EEPromM24C02::EEPromM24C02(uint8_t in_E0, uint8_t in_E1, uint8_t in_E2, uint8_t wb_pin, TwoWire& Wire_value) : 
    E0{in_E0}, E1{in_E1}, E2{in_E2}, wb{wb_pin}, address{EEPROM_ADDRERSS}, wire_ref{Wire_value}
{
    e_present = true; 
}

eeprom::EEPromM24C02::EEPromM24C02(uint8_t wb_pin, TwoWire& wire_in) : 
wb{wb_pin}, 
address{EEPROM_ADDRERSS}, 
wire_ref{wire_in}
{
    e_present = false; 
}

void eeprom::EEPromM24C02::init()
{
    if (e_present)
    {
        pinMode(E0,OUTPUT); 
        pinMode(E1,OUTPUT); 
        pinMode(E2,OUTPUT); 
        digitalWrite(E0,LOW); 
        digitalWrite(E1,LOW); 
        digitalWrite(E2,LOW); 
    }
    
    //write blocking pin (turned off when not writing)
    pinMode(wb,OUTPUT); 
    digitalWrite(wb,HIGH); 

    wire_ref.begin(); 
}

bool eeprom::EEPromM24C02::write_bytes(uint8_t addr, const uint8_t* values, uint8_t num_bytes)
{
    bool res = true; 
    digitalWrite(wb, LOW); 
    wire_ref.beginTransmission(address); 
    wire_ref.write(addr); 
    auto write_stat = wire_ref.write(values, num_bytes); 
    wire_ref.endTransmission(); 
    if (write_stat != num_bytes)
    {
        LOG_ERROR("could not write : ", num_bytes, " bytes"); 
        res =  false; 
    }
    digitalWrite(wb, HIGH); 
    return res; 
}

bool eeprom::EEPromM24C02::read_bytes(uint8_t addr, uint8_t* output, uint8_t num_bytes)
{ 
    if (num_bytes > 16)
    {
        LOG_ERROR("tried to read too many bytes"); 
        return false; 
    }

    bool res = true; 
    wire_ref.beginTransmission(address); 
    wire_ref.write(addr); 
    auto response = wire_ref.endTransmission(); 
    if (response != 0) 
    {
        LOG_ERROR("get address error : ", response); 
        res = false; 
    }

    auto req_size = wire_ref.requestFrom(address, num_bytes); 
    if (req_size != num_bytes)
    {
        LOG_ERROR("error requesting from : ", address, ", rec sz : ", req_size);
        res = false;  
    }
    if (auto num_read = wire_ref.readBytes(output, num_bytes); num_read != num_bytes)
    {
        LOG_ERROR("error reading eeprom bytes from : ", address, ", read : ", num_read);
        res = false;  
    } 
    return res; 
}

bool eeprom::EEPromM24C02::detect_helper()
{
    wire_ref.begin(); 
    wire_ref.beginTransmission(address); 
    auto error = wire_ref.endTransmission(); 
    if (error != 0)
    {
        LOG_ERROR("detect error : ", error); 
        return false; 
    }
    else 
    {
        LOG_INFO("eeprom detected"); 
    }
    return true; 
}