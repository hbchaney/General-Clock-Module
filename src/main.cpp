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

input::InputManager man{
    9,10,1,0,4
}; 

display::DisplayValues disp_vals{
    12,47,9,25,0b0000,true
}; 
display::DisplayManager disp{Wire1};  
utilities::ClockTime clk_time{1, 1, 1, 1, 1, 2021}; 
rtc::RTCDS3231 rtc_dev{Wire, clk_time}; 

clock_mode::TimeSet time_set {disp, clk_time, rtc_dev}; 
clock_mode::TimeShow time_show {disp, clk_time, rtc_dev}; 

bool mil_en = false; 

void setup() 
{ 
    rtc_dev.init(); 
    base_utilities::UpdateBase::run_inits();
    time_show.enter_mode();  
}

void setup1()
{   
    base_utilities::UpdateBase::run_inits_1(); 
}

void loop() 
{   
    auto out = man.get_input(); 
    time_show.process_input(out); 
    time_show.tick(); 


    base_utilities::UpdateBase::run_updates(); 
}

void loop1()
{
    base_utilities::UpdateBase::run_updates_1(); 
}
