#pragma once 

#include "TwoSegDisplay.h" 
#include "update.h"
#include "clock_time.h"
#include "alarm_manager.h"


namespace display 
{

//struct for generic time stuff
struct DisplayValues
{
    display::Lexicon top; 
    display::Lexicon bottom;  
}; 

//general class for setting the fields and animations for them 
//class currently supports blinking in each field
class DisplayManager : base_utilities::UpdateBase
{
public: 
DisplayManager(TwoWire& wire_in, uint8_t v_sync_pin_in = 255); 


void init() override; 
void update() override; 

DisplayValues& get_values( bool toggled_changed = true); 
void set_values(const DisplayValues& val); 
void set_blink(uint8_t blnk_set, bool top_col = false, bool bottom_col = false); 
void reset_blink(); 

private: 

void update_blinks(); 

DisplayValues current_vals; 
TwoSegDisplay disp; 
bool vals_changed = false;  
static constexpr unsigned long blink_time = 250; 
unsigned long blink_start = 0; 
bool blink_on = false; 

uint8_t blink_settings = 0;
bool top_colon_blink = 0;
bool bottom_colon_blink = 0; 

}; 

}