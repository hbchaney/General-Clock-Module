#pragma once 
#include <Arduino.h>
#include <Wire.h>

namespace display 
{

enum class DISPLAY_MODE : uint8_t
{
    INSTANT_UPDATE = 0, 
    VSYNC_8BIT = 1, 
    VSYNC_16BIT =2, 
}; 


//class for interfacing with lp5868
//if running in vsync mode it is recommended to run on a seperate core
class LP5868 
{
    public: 
    LP5868(TwoWire& wire_ref, uint8_t v_sync_pin_in); 


    bool enable_chip(DISPLAY_MODE disp_mode); 
    uint8_t check_fault(); 
    bool set_address(uint16_t address, uint8_t value); 
    bool set_address(uint16_t address, uint16_t value); 
    uint8_t get_address(uint16_t address); 
    void set_mode(DISPLAY_MODE mode); 
    void toggle_vsync(); 


    private: 
    TwoWire& wire; 
    uint8_t v_sync_pin; 

    //consts 
    static constexpr uint16_t DC_START = 0x100; 
    static constexpr uint8_t ADDR = 0x40; 

    static constexpr uint16_t PWM_OFFSET = 0x200; 
}; 

}