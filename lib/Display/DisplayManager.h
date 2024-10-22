#pragma once 

#include "TwoSegDisplay.h" 
#include "update.h"
#include "clock_time.h"


namespace display 
{

enum class DisplayModes 
{
    CLOCK_MODE, //generic mode for displaying the clock
    YEAR_DATE_MODE, //mode for setting the year 
    ALARM_MODE,
}; 

//struct for generic time stuff
struct DisplayValues
{
    //generic clock variables
    uint8_t hour;  
    uint8_t min;  
    uint8_t month;  
    uint8_t day; 
    uint8_t blink_setting; // for all blinking 0b1111
    bool is_am; 

    //year date settings
    uint16_t year; 

    //alarm variables 
    uint8_t alarm_number; 

    bool alarm_on; 
}; 

//general class for setting the fields and animations for them 
//class currently supports blinking in each field
class DisplayManager : base_utilities::UpdateBase
{
public: 
DisplayManager(TwoWire& wire_in, uint8_t v_sync_pin_in = 255); 
 

void init() override; 
void update() override; 

void set_display_mode(DisplayModes mode); 

void set_clock_display(DisplayValues& vals); 

//time showing utilities
void update_with_time(utilities::ClockTime& clk_time); 
void update_blink_setting(uint8_t blink_setting); 


private: 

void clock_display();  
void alarm_set_display(); 
void year_date_display(); 


DisplayValues current_vals; 
DisplayModes current_mode; 
TwoSegDisplay disp; 
bool vals_changed = false; 
bool blink_status = false; 
static constexpr unsigned long blink_time = 250; 
unsigned long blink_start = 0; 
uint8_t brightness = 150; //add something later to make this adjustable

}; 

}