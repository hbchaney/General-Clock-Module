#pragma once 
#include "update.h"

enum press_type 
{ 
    Long, 
    Short,
    None
};

class Button : base_utilities::UpdateBase
{ 
    public: 

    Button(const Button&) = delete; 
    Button() = delete; 
    Button(uint8_t,unsigned long); 

    void init(); 
    void update();
    press_type get_output_cache(); 

    private: 

    void update_filter(); 
    int filtered_press = 0; 
    int buffered_press = 0; 
    bool buffering = true; 
    unsigned long buffer_length = 50; 
    unsigned long buffer_start = 0; 
    
    uint8_t pin_no; 
    unsigned long press_start; 
    unsigned long press_length; 

    bool last_state = false; //true when pressed down
    bool long_trigger = false; 
    unsigned long delay_length = 150; //delay between possible press inputs
    unsigned long last_press = 0; 

    press_type output_cache = None;

};
