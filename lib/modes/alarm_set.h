#pragma once 
#include "mode.h"
#include "mode_utilities.h"
#include "alarm_manager.h"
#include "ClockInput.h"
#include "eeprom_M24C02.h"
#include "DisplayManager.h"

namespace clock_mode 
{

class AlarmSet : public base_utilities::Mode<input::ClockInput, ModeIndex>
{

    public: 
    AlarmSet(alarm::AlarmManager& al_man,  
        display::DisplayManager& disp_man, 
        utilities::ClockTime& time_in); 

    void process_input(input::ClockInput in) override;  
    void enter_mode() override; 
    void exit_mode() override; 
    void tick() override; 

    private: 

    void process_num_select(input::ClockInput in);  
    void process_on_off_select(input::ClockInput in); 
    void process_hour_select(input::ClockInput in); 
    void process_min_selct(input::ClockInput in); 

    void update_disp(); 

    enum class AlarmSubMode {
        NUM_SELECT = 0, 
        ON_OFF_SELECT = 1,
        HOUR_SELECT = 2, 
        MIN_SELECT = 3, 
        END = 4
    };

    alarm::AlarmManager& alarm_manager; 
    display::DisplayManager& disp_manager; 
    utilities::ClockTime& clk_time_ref; //time ref to see if mil time or not 
    int current_mode = static_cast<int>(AlarmSubMode::NUM_SELECT); 

    utilities::ClockTime working_time; //used to set alarm time 

    static constexpr uint8_t blnk_seq[] {
        0b0011, 
        0b1100, 
        0b110000, 
        0b11000000
    }; 
}; 


}