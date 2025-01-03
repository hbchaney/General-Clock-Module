#include <Arduino.h>
#define DEBUGLOG_DEFAULT_LOG_LEVEL_DEBUG
#include "WireHelper.h"
#include "DebugLog.h"
#include "TwoSegDisplay.h"
#include "Buzzer.h"
#include "SPI.h"
#include "Rotary_Encoder.h"
#include "ClockInput.h"
#include "DisplayManager.h"
#include "time_set.h"
#include "clock_time.h"
#include "time_show.h"
#include "mode_manager.h"
#include "WireHelper.h"

input::InputManager man{
    9,10,1,0,4
}; 

eeprom::EEPromM24C02 prom{15, Wire}; 
i2c_tools::WireHelper help{Wire}; 

void setup() 
{ 
    base_utilities::UpdateBase::run_inits();  
    prom.init(); 

}

void setup1()
{   
    base_utilities::UpdateBase::run_inits_1(); 
}

void loop() 
{    
    auto in = man.get_input(); 
    if (in == input::ClockInput::LEFT_PRESS)
    {
        //save a random number 
        uint8_t ran = rand(); 
        prom.write_bytes(10, &ran, 1);
        LOG_INFO("saving : ", static_cast<int>(ran), " to byte 10");  
    }
    else if (in == input::ClockInput::RIGHT_PRESS)
    {
        uint8_t out;
        prom.read_bytes(10, &out, 1); 
        LOG_INFO("byte saved : ", static_cast<int>(out), " at byte pos 10"); 
    }
    else if (in == input::ClockInput::INC)
    {
        uint8_t ran[2]; 
        ran[0] = rand(); 
        ran[1] = rand(); 
        auto stat = prom.write_bytes(20, ran, 2); 
        LOG_INFO("bytes saved : ", static_cast<int>(ran[0]), " | ",static_cast<int>(ran[1]), ", status : ", stat); 
    }
    else if (in == input::ClockInput::DEC) 
    {
        uint8_t out[2]; 
        auto stat = prom.read_bytes(20, out, 2); 
        LOG_INFO("bytes read : ", static_cast<int>(out[0]), " | ",static_cast<int>(out[1]), ", status : ", stat);
    }
    else if (in == input::ClockInput::ROT_PRESS)
    {
        if (!prom.detect_helper())
        {
            help.print_all_present(); 
        } 
    }
    base_utilities::UpdateBase::run_updates(); 
}

void loop1()
{
    base_utilities::UpdateBase::run_updates_1(); 
}
