#include <Arduino.h>
#define DEBUGLOG_DEFAULT_LOG_LEVEL_DEBUG
#include "WireHelper.h"
#include "DebugLog.h"
#include "TwoSegDisplay.h"
#include "Buzzer.h"
#include "SPI.h"
#include "Rotary_Encoder.h"
#include "ClockInput.h"
#include "DisplayManager.h"
#include "time_set.h"
#include "clock_time.h"
#include "time_show.h"
#include "mode_manager.h"

input::InputManager man{
    9,10,1,0,4
}; 


display::DisplayManager disp{Wire1};  
// utilities::ClockTime clk_time{1, 1, 1, 1, 1, 2021}; 
// rtc::RTCDS3231 rtc_dev{Wire, clk_time}; 
// SequenceBuzzer buzzie{
//     26, {100,200,200,100}, 150
// }; 
// clock_mode::ModeManager manager {
//     disp, clk_time, rtc_dev, buzzie, man
// }; 

display::DisplayValues disp_vals[5]; 
int current_val = 0; 

void setup() 
{ 
    // rtc_dev.init(); //should probably put this in the mode managers init
    base_utilities::UpdateBase::run_inits();  

    //check if setting ints works 
    disp_vals[0].top = 2222; 
    disp_vals[0].bottom = 3333; 
    disp_vals[0].top.set_colon(display::Lexicon::COLON_DOT); 
    disp_vals[0].bottom.set_colon(display::Lexicon::COLON_BOTTOM_DOT); 

    //check if setting chars works 
    disp_vals[1].top = "abcd"; 
    disp_vals[1].bottom = "efgh"; 
    disp_vals[1].top.set_colon(display::Lexicon::COLON_TOP_DOT); 
    disp_vals[1].bottom.set_colon(0x0); 

    //check if the set left and set right work
    disp_vals[2].top.set_left(3); 
    disp_vals[2].top.set_right(4,true); //test 0 pad 
    disp_vals[2].bottom.set(static_cast<uint8_t>(4), 1);
    disp_vals[2].bottom.set(display::Lexicon::SPACE, 0); 
    disp_vals[2].bottom.set('t',2); 
    disp_vals[2].bottom.set(display::Lexicon::SPACE, 3); 
    disp_vals[2].bottom.set_colon(0); 
    disp_vals[2].bottom.set_dots(0b1010);

    disp.set_values(disp_vals[current_val]); 

}

void setup1()
{   
    base_utilities::UpdateBase::run_inits_1(); 
}

void loop() 
{   
    if (man.get_input() != input::ClockInput::NONE)
    {
        LOG_INFO("input triggered"); 
        current_val = (current_val + 1) % std::size(disp_vals); 
        disp.set_values(disp_vals[current_val]); 
        if (current_val == 2) 
        {
            //test blinking 
            disp.set_blink(0b0101); 
        }
        else
        {
            disp.set_blink(0b0); 
        }
    }


    base_utilities::UpdateBase::run_updates(); 
}

void loop1()
{
    base_utilities::UpdateBase::run_updates_1(); 
}
