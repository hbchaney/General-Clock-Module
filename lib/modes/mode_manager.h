#pragma once
#include "mode.h"
#include "mode_utilities.h"
#include "ClockInput.h"
#include "time_show.h"
#include "time_set.h"
#include "Buzzer.h"
#include "alarm_set.h"
#include <map>

namespace clock_mode
{


class ModeManager : base_utilities::UpdateBase
{
    public:  
    ModeManager(display::DisplayManager& disp_in,
                utilities::ClockTime& time_in, 
                rtc::RTCDS3231& rtc_in, 
                SequenceBuzzer& buzzie_in, 
                input::InputManager& input_manager_in, 
                eeprom::EEPromM24C02& eeprom_in); 

    void update() override; 
    void init() override; 
    

    private:
    void rotate_mode(); 


    void switch_mode(ModeIndex ind); //mode index call back 


    display::DisplayManager& disp; 
    utilities::ClockTime& time;
    rtc::RTCDS3231& rtc; 
    SequenceBuzzer& buzzie; 
    input::InputManager& input_manager; 
    eeprom::EEPromM24C02& eeprom_dev; 

    alarm::AlarmManager alarm_manager; 

    TimeShow time_show; 
    TimeSet time_set;  
    AlarmSet alarm_set; 

    int loop_count = 0;  
    base_utilities::Mode<input::ClockInput, ModeIndex>* current_mode; 
    std::map<ModeIndex, base_utilities::Mode<input::ClockInput, ModeIndex>*> mode_map; 
    std::vector<ModeIndex> mode_index_loop; 

    static constexpr uint8_t STORAGE_START_ADDRESS = 10; 
}; 

}