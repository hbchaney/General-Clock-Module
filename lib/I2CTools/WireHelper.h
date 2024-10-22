#pragma once 
#include <Arduino.h>
#include <Wire.h>

namespace i2c_tools
{

class WireHelper
{
    public: 
    WireHelper(TwoWire& wire); 

    //scans for the first availiable address
    uint8_t scan_for_first_address(); 


    bool scan_address(uint8_t address); 

    private: 

    TwoWire& wire_ref; 
}; 


} //eof i2ctools