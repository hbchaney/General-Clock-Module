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
    rtc_ref.set_time(); //sets time with shared clock time ref
}

void TimeSet::enter_mode() 
{
    rtc_ref.refresh_time(); //make sure it has the most recent time before starting
    disp.set_display_mode(display::DisplayModes::CLOCK_MODE); 
    disp.update_with_time(time_ref); 
    disp.update_blink_setting(0b0001); 
    current_mode = current_set_mode::HOURS; 
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
            disp.update_blink_setting(0b0001); 
            break; 
        case current_set_mode::MINS: 
            disp.update_blink_setting(0b0010); 
            break; 
        case current_set_mode::YEAR: 
            disp.set_display_mode(display::DisplayModes::YEAR_DATE_MODE); 
            disp.update_blink_setting(0b0011); 
            break; 
        case current_set_mode::MONTHS: 
            disp.set_display_mode(display::DisplayModes::CLOCK_MODE); 
            disp.update_blink_setting(0b0100); 
            break; 
        case current_set_mode::DAYS: 
            disp.update_blink_setting(0b1000); 
            break; 
        default: 
            LOG_ERROR("current mode set to unknown"); 
    }
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
    disp.update_with_time(time_ref); 
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
    disp.update_with_time(time_ref); 
}
