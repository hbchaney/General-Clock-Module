#pragma once 

#include "Adafruit_I2CDevice.h"
#include "DebugLog.h"
//defs

namespace eeprom 
{ 
class EEPromM24C02
{
    public: 

    EEPromM24C02(const EEPromM24C02&) = delete; 
    EEPromM24C02(uint8_t in_E0, uint8_t in_E1, uint8_t in_E2, uint8_t wb_pin, TwoWire& Wire); 
    EEPromM24C02(uint8_t wb_pin, TwoWire& wire_in); 
    void init();  

    bool detect_helper(); 

    bool write_bytes(uint8_t addr, const uint8_t* values, uint8_t num_bytes); 
    bool read_bytes(uint8_t addr, uint8_t* output, uint8_t num_bytes); 

    private: 

    //vars 
    static constexpr uint8_t EEPROM_ADDRERSS {0x50}; 
    uint8_t address; 
    TwoWire& wire_ref;  

    uint8_t E0 = 0; 
    uint8_t E1 = 0; 
    uint8_t E2 = 0; 

    bool e_present; 

    uint8_t wb; 
}; 
}