#pragma once 

#include "clock_time.h"
#include "mode_utilities.h"
#include "mode.h"
#include "ClockInput.h"
#include "DisplayManager.h"
#include "rtc_ds3231.h"
#include "Buzzer.h"

namespace clock_mode
{

class TimeShow : public base_utilities::Mode<input::ClockInput, ModeIndex>
{
    public: 
    TimeShow() = delete; 
    TimeShow(display::DisplayManager& disp_in, 
            utilities::ClockTime& time_in, 
            rtc::RTCDS3231& rtc_in, 
            SequenceBuzzer& buzz, 
            const alarm::AlarmManager& al_man);

    void process_input(input::ClockInput in) override; 
    void tick() override; 
    void exit_mode() override; 
    void enter_mode() override; 

    private: 

    void update_disp(); 

    display::DisplayManager& disp; 
    utilities::ClockTime& time_ref; 
    rtc::RTCDS3231& rtc_ref; 
    SequenceBuzzer& buzzie; 
    const alarm::AlarmManager& al_ref; 

    uint32_t last_update = 0; 
    uint32_t update_freq = 1000; //check for updates every 100 millis
    uint8_t last_min = 0; 

    ulong al_start = 0; 
    static constexpr ulong AL_TIMEOUT =  15000;
    bool al_trig;  

}; 

} //eof clock_mode