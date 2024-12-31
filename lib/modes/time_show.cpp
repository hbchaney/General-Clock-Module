#include "time_show.h"

using namespace clock_mode; 


TimeShow::TimeShow(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in, 
            SequenceBuzzer& buzz) : 
    base_utilities::Mode<input::ClockInput, ModeIndex>{ModeIndex::TimeShow},
    disp{disp_in},
    time_ref{time_in},
    rtc_ref{rtc_in}, 
    buzzie{buzz}
{}

void TimeShow::process_input(input::ClockInput in)
{
    //toggle between military time
    if (in == input::ClockInput::RIGHT_PRESS) 
    {
        time_ref.enable_military(!time_ref.is_military()); //toggle the current military settings
        buzzie.pulse(50, 200);
        update_disp(); 
        LOG_INFO("mil toggle"); 
    }
}

void TimeShow::tick()
{
    if (last_update + update_freq < millis()) 
    {
        last_update = millis(); 
        rtc_ref.refresh_time(); 
        update_disp();  
    }
}

void TimeShow::enter_mode() 
{
    last_update = millis(); 
    rtc_ref.refresh_time(); 
    disp.reset_blink(); 
    update_disp();  
}

void TimeShow::exit_mode() 
{
    //also does nothing for now
}

void TimeShow::update_disp()
{
    auto& vals = disp.get_values(); 
    vals.top.set_left(time_ref.get_hours()); 
    vals.top.set_right(time_ref.get_mins(), true); 
    vals.top.set_colon(display::Lexicon::COLON_NO_DOT); 
    vals.bottom.set_left(time_ref.get_months()); 
    vals.bottom.set_right(time_ref.get_days()); 
    vals.bottom.set_colon(display::Lexicon::COLON_BOTTOM); 
    if (!time_ref.is_am()) 
    {
        vals.top.set_dots(0b1000); 
    }
    else 
    {
        vals.top.set_dots(0b0); 
    }
}