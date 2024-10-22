#include "lp5868.h"
#define DEBUGLOG_DEFAULT_LOG_LEVEL_DEBUG

#include "DebugLog.h"

using namespace display; 

LP5868::LP5868(TwoWire& wire_ref, uint8_t v_sync_pin_in) : 
    wire{wire_ref}, v_sync_pin{v_sync_pin_in}
{}

uint8_t LP5868::check_fault()
{ 
    wire.beginTransmission(ADDR); 
    wire.write(0x64); 
    wire.endTransmission(); 
    wire.requestFrom(ADDR, 1); 
    return wire.read(); 
}


bool LP5868::enable_chip(DISPLAY_MODE disp_mode = DISPLAY_MODE::INSTANT_UPDATE)
{
    //enable chip
    wire.begin(); 
    wire.beginTransmission(ADDR); 
    wire.write(0x0);
    wire.write(0x1); 
    auto status = wire.endTransmission(); 

    //set mode, max line, and freq
    wire.beginTransmission(ADDR); 
    wire.write(0x01); 
    wire.write(0b00101000 | ((static_cast<uint8_t>(disp_mode) & 0b11) << 1)); 
    wire.endTransmission(); 

    //phase shift and other settings
    wire.beginTransmission(ADDR); 
    wire.write(0x0); 
    wire.endTransmission(); 
    wire.requestFrom(ADDR, 1); 
    LOG_INFO("chip en set to ", static_cast<int>(wire.read())); 
    return true; 
}


bool LP5868::set_address(uint16_t address, uint8_t value)
{
    uint8_t start = ADDR | (address >> 8); 
    uint8_t second = address & 0xff; 

    wire.beginTransmission(start); 
    wire.write(second); 
    wire.write(value); 
    auto status = wire.endTransmission(); 
    if (status)
    {
        LOG_ERROR("error occured when writing to i2c : ", static_cast<int>(status)); 
        return false; 
    }
    return true; 
}

bool LP5868::set_address(uint16_t address, uint16_t value)
{
    uint8_t start = ADDR | (address >> 8); 
    uint8_t second = address & 0xff; 


    wire.beginTransmission(start); 
    wire.write(second); 
    wire.write(static_cast<uint8_t>(value >> 8)); 
    wire.write(static_cast<uint8_t>(value & 0xff)); 
    auto status = wire.endTransmission(); 
    if (status)
    {
        LOG_ERROR("error occured when writing to i2c : ", static_cast<int>(status)); 
        return false; 
    }
    return true; 
}

uint8_t LP5868::get_address(uint16_t address)
{
    uint8_t start = ADDR | (address >> 8); 
    uint8_t second = address & 0xff; 

    wire.beginTransmission(start); 
    wire.write(second); 
    wire.endTransmission(); 

    wire.requestFrom(start, 1); 
    return wire.read(); 
}

void LP5868::set_mode(DISPLAY_MODE mode)
{
    wire.beginTransmission(ADDR); 
    wire.write(0x01); 
    wire.write(0b00101000 | ((static_cast<uint8_t>(mode) & 0b11) << 1)); 
    wire.endTransmission(); 
}

void LP5868::toggle_vsync()
{
    digitalWrite(v_sync_pin, HIGH); 
    delay(1); //min pulse width is 200us
    digitalWrite(v_sync_pin, LOW); 
}