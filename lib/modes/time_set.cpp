#include "time_set.h"

using namespace clock_mode; 



TimeSet::TimeSet(display::DisplayManager& disp_in, 
                 utilities::ClockTime& time_in, 
                 rtc::RTCDS3231& rtc_in) : 
    base_utilities::Mode<input::ClockInput, ModeIndex>{ModeIndex::TimeSet},
    disp{disp_in}, 
    time_ref{time_in}, 
    rtc_ref{rtc_in}
{}

void TimeSet::process_input(input::ClockInput in)
{
    switch (in) 
    {
        case input::ClockInput::ROT_PRESS: 
            process_select(); 
            break; 
        case input::ClockInput::INC:
            process_inc(); 
            break; 
        case input::ClockInput::DEC: 
            process_dec(); 
            break; 
        case input::ClockInput::RIGHT_PRESS: 
            
        default: 
            break; 
    }
}

void TimeSet::tick()
{
    //dont do anything? 
    //maybe wait for a timeout eventually
}

void TimeSet::exit_mode() 
{
    disp.reset_blink(); //just in case
    rtc_ref.set_time(); //sets time with shared clock time ref
}

void TimeSet::enter_mode() 
{
    rtc_ref.refresh_time(); //make sure it has the most recent time before starting  
    disp.set_blink(0b0011, true);
    current_mode = current_set_mode::HOURS;
    update_disp();  
}

void TimeSet::process_select() 
{
    if (static_cast<int>(current_mode) + 1 == static_cast<int>(current_set_mode::END_OF_MODE))
    {
        //end of mode set the time 
        //setting time : 
        time_ref.print_time(); 
        rtc_ref.set_time(); 

        auto time_back = rtc_ref.get_time(); 
        time_back.print_time(); 
    }
    current_mode = static_cast<current_set_mode>((static_cast<int>(current_mode) + 1) % static_cast<int>(current_set_mode::END_OF_MODE)); 
    LOG_DEBUG("current mode : ", static_cast<int>( current_mode)); 
    switch (current_mode)
    {
        case current_set_mode::HOURS: 
            disp.set_blink(0b0011); 
            break; 
        case current_set_mode::MINS: 
            disp.set_blink(0b1100); 
            break; 
        case current_set_mode::YEAR:  
            disp.set_blink(0b1111); 
            break; 
        case current_set_mode::MONTHS: 
            disp.set_blink(0b110000); 
            break; 
        case current_set_mode::DAYS: 
            disp.set_blink(0b11000000); 
            break; 
        default: 
            LOG_ERROR("current mode set to unknown"); 
    }
    update_disp(); 
}

void TimeSet::process_inc() 
{
    switch (current_mode)
    {
        case current_set_mode::HOURS: 
            time_ref.add_time(1,0); 
            break; 
        case current_set_mode::MINS: 
            time_ref.add_time(0,1); 
            break; 
        case current_set_mode::YEAR: 
            LOG_INFO("Year : " , time_ref.get_year()); 
            time_ref.add_date(0,0,1); 
            break; 
        case current_set_mode::MONTHS: 
            time_ref.add_date(0,1,0);  
            break; 
        case current_set_mode::DAYS: 
            time_ref.add_date(1,0,0); 
            break; 
        default: 
            LOG_ERROR("current mode set to unknown"); 
    }
    update_disp();  
}

void TimeSet::process_dec() 
{
    switch (current_mode)
    {
        case current_set_mode::HOURS: 
            time_ref.sub_time(1,0); 
            break; 
        case current_set_mode::MINS: 
            time_ref.sub_time(0,1); 
            break; 
        case current_set_mode::YEAR: 
            time_ref.sub_date(0,0,1); 
            break; 
        case current_set_mode::MONTHS: 
            time_ref.sub_date(0,1,0);  
            break; 
        case current_set_mode::DAYS: 
            time_ref.sub_date(1,0,0); 
            break; 
        default: 
            LOG_ERROR("current mode set to unknown"); 
    }
    //special case for year 
    update_disp();  
}


void TimeSet::update_disp()
{
    if (current_mode != current_set_mode::YEAR)
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
    else 
    {
        auto& vals = disp.get_values(); 
        vals.top = time_ref.get_year(); 
        vals.top.set_colon(0x0);  
        vals.bottom.set_left(time_ref.get_months()); 
        vals.bottom.set_right(time_ref.get_days()); 
        vals.bottom.set_colon(display::Lexicon::COLON_BOTTOM);
    }
}