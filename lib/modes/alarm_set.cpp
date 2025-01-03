#include "alarm_set.h"

using namespace clock_mode; 

AlarmSet::AlarmSet(alarm::AlarmManager& al_man,  
        display::DisplayManager& disp_man, 
        utilities::ClockTime& time_in) : 
base_utilities::Mode<input::ClockInput, ModeIndex>{clock_mode::ModeIndex::AlarmSet}, 
alarm_manager{al_man}, 
disp_manager{disp_man}, 
clk_time_ref{time_in}
{}


void AlarmSet::process_input(input::ClockInput in)
{
    if (in == input::ClockInput::ROT_PRESS)
    {
        current_mode++; 
        current_mode = current_mode % static_cast<int>(AlarmSubMode::END); 
        disp_manager.set_blink(blnk_seq[current_mode]); 
    }
    else if (in == input::ClockInput::RIGHT_PRESS)
    {
        clk_time_ref.enable_military(!clk_time_ref.is_military()); 
        working_time.enable_military(clk_time_ref.is_military()); 
    }
    else {
        switch (static_cast<AlarmSubMode>(current_mode))
        {
            case AlarmSubMode::NUM_SELECT: 
                process_num_select(in); 
                break; 
            case AlarmSubMode::ON_OFF_SELECT: 
                process_on_off_select(in); 
                break; 
            case AlarmSubMode::HOUR_SELECT: 
                process_hour_select(in); 
                break; 
            case AlarmSubMode::MIN_SELECT: 
                process_min_selct(in); 
                break; 
        }
    }
    update_disp(); 
}

void AlarmSet::enter_mode() 
{
    current_mode = static_cast<int>(AlarmSubMode::NUM_SELECT); 
    //reset alarm index
    alarm_manager.set_alarm_index(0); 
    working_time = alarm_manager.get_current_alarm().to_clock_time(); 
    working_time.enable_military(clk_time_ref.is_military()); 
    disp_manager.set_blink(blnk_seq[current_mode]); 

    update_disp(); 
}

void AlarmSet::exit_mode() 
{
    //save the current alarm setting 
    alarm_manager.get_current_alarm().set_from_clktime(working_time);
    alarm_manager.save_alarm();  
}

void AlarmSet::tick() {} //NOOP for now 

void AlarmSet::update_disp() 
{
    //set top stuff
    alarm_manager.get_alarm_index(); 
    auto& [top, bottom] = disp_manager.get_values(); 
    top.set('a', 0); 
    top.set(static_cast<uint8_t>(alarm_manager.get_alarm_index()), 1); 
    top.set_dots(0); 

    if (alarm_manager.get_current_alarm().on)
    {
        top.set('o',2); 
        top.set('n',3); 
    }
    else 
    {
        top.set('o', 2); 
        top.set('f', 3); 
    }

    top.set_colon(display::Lexicon::COLON_BOTTOM); 

    bottom.set_left(working_time.get_hours()); 
    bottom.set_right(working_time.get_mins(), true); 
    bottom.set_colon(display::Lexicon::COLON_NO_DOT); 
    if (working_time.is_am())
    {
        bottom.set_dots(0b0); 
    } 
    else 
    {
        bottom.set_dots(0b1000); 
    }
}

void AlarmSet::process_num_select(input::ClockInput in)
{
    if (in == input::ClockInput::INC)
    {
        alarm_manager.get_current_alarm().set_from_clktime(working_time); 
        alarm_manager.get_current_alarm().print_time(); 
        alarm_manager.save_alarm(); 
        alarm_manager.next_alarm(); 
        working_time = alarm_manager.get_current_alarm().to_clock_time(); 
        working_time.enable_military(clk_time_ref.is_military()); 
    }
    else if (in == input::ClockInput::DEC)
    {
        alarm_manager.get_current_alarm().set_from_clktime(working_time);
        alarm_manager.save_alarm(); 
        alarm_manager.previous_alarm();
        working_time = alarm_manager.get_current_alarm().to_clock_time(); 
        working_time.enable_military(clk_time_ref.is_military()); 
    }
    update_disp(); 
}

void AlarmSet::process_on_off_select(input::ClockInput in) 
{
    if (in == input::ClockInput::DEC || in == input::ClockInput::INC)
    {
        alarm_manager.get_current_alarm().on = !alarm_manager.get_current_alarm().on; 
    }
}

void AlarmSet::process_hour_select(input::ClockInput in) 
{
    if (in == input::ClockInput::INC) 
    {
        working_time.add_time(1,0); 
    }
    else if (in == input::ClockInput::DEC) 
    {
        working_time.sub_time(1,0); 
    }
}

void AlarmSet::process_min_selct(input::ClockInput in)
{
    if (in == input::ClockInput::INC) 
    {
        working_time.add_time(0,1); 
    }
    else if (in == input::ClockInput::DEC) 
    {
        working_time.sub_time(0,1); 
    }
}