#include "time_show.h"

using namespace clock_mode; 


TimeShow::TimeShow(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in, 
            SequenceBuzzer& buzz, 
            const alarm::AlarmManager& al_man) : 
    base_utilities::Mode<input::ClockInput, ModeIndex>{ModeIndex::TimeShow},
    disp{disp_in},
    time_ref{time_in},
    rtc_ref{rtc_in}, 
    buzzie{buzz}, 
    al_ref{al_man}
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
    else if (in == input::ClockInput::ROT_PRESS)
    {
        if (al_trig)
        {
            al_trig = false; 
            buzzie.stop_buzzing(); 
        }
    }
}

void TimeShow::tick()
{
    if (last_update + update_freq < millis()) 
    {
        last_update = millis(); 
        rtc_ref.refresh_time(); 
        update_disp();  
        if (last_min != time_ref.get_mins())
        {
            last_min = time_ref.get_mins(); 
            //check for alarm 
            if (al_ref.check_alarm(time_ref))
            {
                buzzie.start_buzzing(); 
                al_trig = true; 
                al_start = millis(); 
            }
        }
    }

    if (al_trig && al_start + AL_TIMEOUT < millis())
    {
        al_trig = false; 
        buzzie.stop_buzzing(); 
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
    //stop buzzing if buzzing 
    al_trig = false; 
    buzzie.stop_buzzing(); 
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