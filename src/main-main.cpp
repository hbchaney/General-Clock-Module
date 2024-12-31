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

input::InputManager man{
    9,10,0,1,4
}; 


display::DisplayManager disp{Wire1};  
utilities::ClockTime clk_time{1, 1, 1, 1, 1, 2021}; 
rtc::RTCDS3231 rtc_dev{Wire, clk_time}; 
SequenceBuzzer buzzie{
    26, {100,200,200,100}, 150
}; 
clock_mode::ModeManager manager {
    disp, clk_time, rtc_dev, buzzie, man
}; 


void setup() 
{ 
    rtc_dev.init(); //should probably put this in the mode managers init
    base_utilities::UpdateBase::run_inits();  

}

void setup1()
{   
    base_utilities::UpdateBase::run_inits_1(); 
}

void loop() 
{   
    base_utilities::UpdateBase::run_updates(); 
}

void loop1()
{
    base_utilities::UpdateBase::run_updates_1(); 
}
