#pragma once 

#include "Adafruit_I2CDevice.h"
#include "DebugLog.h"
//defs

namespace eeprom 
{ 
    class EEProm_M24C02
    {
        public: 

        EEProm_M24C02(uint8_t in_E0, uint8_t in_E1, uint8_t in_E2, uint8_t wb_pin, TwoWire* Wire); 
        bool setup(); 

        ~EEProm_M24C02(); 

        bool detect_helper(); 

        bool write_byte(uint8_t addr, const uint8_t value); 
        bool read_byte(uint8_t addr, uint8_t& value); 

        //reads random type
        template<typename T> 
        T read(uint8_t addr)
        {  
            int sz = sizeof(T)/sizeof(uint8_t); //size in no bytes
            T end_v; 
            uint8_t* v_ptr = reinterpret_cast<uint8_t*>(&end_v); 
            //error status 
            bool error; 
            for (int i = 0; i < sz; i++)
            {
                uint8_t tmp; 
                error = !(read_byte(addr,tmp));
                *v_ptr = tmp;  
                //if some error happens returns 0
                if (error) 
                { 
                    end_v = 0; 
                    return end_v; 
                }
                addr++; 
                v_ptr++; 
            }
            return end_v; 
        }

        template<typename T> 
        bool write(uint8_t addr, const T value)
        {
            int sz = sizeof(T)/sizeof(uint8_t); 
            const uint8_t* v_ptr = reinterpret_cast<const uint8_t*>(&value); 
            for (int i = 0;  i < sz; i++)
            {
                if (!write_byte(addr,*v_ptr))
                {
                    LOG_ERROR("COULD NOT RECORD INFO"); 
                    return false; 
                } 
                addr++;
                v_ptr++;
            }
            return true; 
        }

        private: 

        //vars 
        const uint8_t address {0x50}; 
        Adafruit_I2CDevice* i2c_dev; 

        uint8_t E0; 
        uint8_t E1; 
        uint8_t E2; 
        uint8_t wb; 
    }; 
}