#pragma once
#include "mode.h"
#include "mode_utilities.h"
#include "ClockInput.h"
#include "time_show.h"
#include "time_set.h"
#include "Buzzer.h"
#include <map>

namespace clock_mode
{


class ModeManager : base_utilities::UpdateBase
{
    public: 
    ModeManager(display::DisplayManager& disp_in,
                utilities::ClockTime& time_in, 
                rtc::RTCDS3231& rtc_in, 
                SequenceBuzzer& buzzie); 


    void process_input(input::ClockInput in); 

    

    private:
    void switch_mode(); 

    const std::map<ModeIndex, base_utilities::Mode<input::ClockInput, ModeIndex>*> mode_map; 
}; 

}