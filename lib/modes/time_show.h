#pragma once 

#include "clock_time.h"
#include "mode_utilities.h"
#include "mode.h"
#include "ClockInput.h"
#include "DisplayManager.h"
#include "rtc_ds3231.h"

namespace clock_mode
{

class TimeShow : public base_utilities::Mode<input::ClockInput, ModeIndex>
{
    public: 
    TimeShow(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in);

    void process_input(input::ClockInput in) override; 
    void tick() override; 
    void exit_mode() override; 
    void enter_mode() override; 

    private: 

    void update_disp(); 

    display::DisplayManager& disp; 
    utilities::ClockTime& time_ref; 
    rtc::RTCDS3231 rtc_ref; 

    uint32_t last_update = 0; 
    uint32_t update_freq = 1000; //check for updates every 100 millis

}; 

} //eof clock_mode