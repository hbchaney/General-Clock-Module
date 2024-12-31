#pragma once 
#include "clock_time.h"
#include "mode_utilities.h"
#include "mode.h"
#include "ClockInput.h"
#include "DisplayManager.h"
#include "rtc_ds3231.h"


namespace clock_mode
{

class TimeSet : public base_utilities::Mode<input::ClockInput, ModeIndex>
{

    enum class current_set_mode 
    {
        HOURS = 0, 
        MINS, 
        YEAR,
        MONTHS, 
        DAYS, 
        END_OF_MODE,
    }; 

    //first set time then year replaces top and set year 
    //then set month and day  


    public: 
    TimeSet(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in); 
    

    void process_input(input::ClockInput in) override; 
    void tick() override; 
    void exit_mode() override; 
    void enter_mode() override; 

    private: 
    void process_select(); 
    void process_inc();
    void process_dec();  

    void update_disp(); 

    display::DisplayManager& disp; 
    utilities::ClockTime& time_ref; 
    rtc::RTCDS3231& rtc_ref; 
    current_set_mode current_mode; 

}; 

}