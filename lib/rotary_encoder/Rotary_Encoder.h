#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "update.h"
#include "input.h"
#include <Arduino.h> 


class RotaryEncoder : public base_utilities::UpdateBase { 
    int in_pin1, in_pin2, button_pin; 
    int8_t l,r,lrmem = 3;
    int TRANS[16];
    int lrsum = 0;
    
    //buttons 
    bool button_toggle = 0;     
    Input output_buffer = Input::R_NOP; //this buffer will be used with loop check to see if anything has been pressed 
    unsigned long press_start = 0;  
    unsigned long long_press = 2000;
    unsigned long noise_floor = 20; //time that needs to be held to minimize noise 
    //checks to see if there was a increment +1 or -1 
    Input check_increment();
    Input check_button(); 

    public: 
    RotaryEncoder(int,int,int, base_utilities::UpdateCore up_core = base_utilities::UpdateCore::CORE_1); 
    ~RotaryEncoder(); 

    void init(void) override; 

    void update(void) override;  

    Input pull_cache (void); 
    Input check_cache (void); 


};

#endif


