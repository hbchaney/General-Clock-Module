#include "time_show.h"

using namespace clock_mode; 


TimeShow::TimeShow(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in) : 
    base_utilities::Mode<input::ClockInput, ModeIndex>{ModeIndex::TimeShow},
    disp{disp_in},
    time_ref{time_in},
    rtc_ref{rtc_in}
{}

void TimeShow::process_input(input::ClockInput in)
{
    //does nothing for now
}

void TimeShow::tick()
{
    if (last_update + update_freq < millis()) 
    {
        last_update = millis(); 
        rtc_ref.refresh_time(); 
        disp.update_with_time(time_ref); 
    }
}

void TimeShow::enter_mode() 
{
    last_update = millis(); 
    rtc_ref.refresh_time(); 
    disp.update_with_time(time_ref); 
    disp.update_blink_setting(0b0000); 
    disp.set_display_mode(display::DisplayModes::CLOCK_MODE); 
}

void TimeShow::exit_mode() 
{
    //also does nothing for now
}