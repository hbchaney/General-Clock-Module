#pragma once 
#include "mode_manager.h"

using namespace clock_mode; 

ModeManager::ModeManager(display::DisplayManager& disp_in,
                utilities::ClockTime& time_in, 
                rtc::RTCDS3231& rtc_in, 
                SequenceBuzzer& buzzie_in, 
                input::InputManager& input_manager_in, 
                eeprom::EEPromM24C02& eeprom_in) : 
disp{disp_in},
time{time_in},
rtc{rtc_in},
buzzie{buzzie_in},
input_manager{input_manager_in}, 
eeprom_dev{eeprom_in},
alarm_manager{eeprom_dev, STORAGE_START_ADDRESS},
time_show{disp,time,rtc, buzzie_in},
time_set{disp,time,rtc}, 
alarm_set{alarm_manager, disp_in, time},
mode_index_loop{ModeIndex::TimeShow, ModeIndex::TimeSet, ModeIndex::AlarmSet}
{
    //create the mode
    mode_map[ModeIndex::TimeShow] = &time_show; 
    mode_map[ModeIndex::TimeSet] = &time_set; 
    mode_map[ModeIndex::AlarmSet] = &alarm_set; 
    current_mode = &time_show; 

}

void ModeManager::update() 
{
    auto out = input_manager.get_input(); 
    if (out == input::ClockInput::LEFT_PRESS)
    {
        rotate_mode(); 
        buzzie.pulse(50, 200); 
    }
    else if (out != input::ClockInput::NONE) 
    {
        current_mode->process_input(out); 
        LOG_INFO("input entered : ", static_cast<int>(out)); 
    }
    else 
    {
        current_mode->tick(); 
    }
}

void ModeManager::init() 
{
    rtc.init(); 
    current_mode = &time_show; 
    current_mode->enter_mode(); 
}

void ModeManager::rotate_mode()
{
    loop_count++; 
    loop_count %= mode_index_loop.size(); 
    ModeIndex ind = mode_index_loop[loop_count]; 
    LOG_DEBUG("changing to mode : ", static_cast<int>(ind)); 
    //exit last mode
    current_mode->exit_mode(); 
    current_mode = mode_map[ind]; 
    current_mode->enter_mode(); 
}

void ModeManager::switch_mode(ModeIndex ind)
{
    current_mode->exit_mode(); 
    current_mode = mode_map.at(ind); 
    current_mode->enter_mode(); 
}
