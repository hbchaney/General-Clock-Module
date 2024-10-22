#include "Rotary_Encoder.h"
#include "DebugLog.h"

RotaryEncoder::RotaryEncoder(int pin1, int pin2, int sw, base_utilities::UpdateCore up_core) : UpdateBase{up_core},  in_pin1{pin1}, in_pin2{pin2}, button_pin{sw}, TRANS{0,-1,1,14,1,0,14,-1,-1,14,0,1,14,1,-1,0} 
{
    /*
    first and second are the turn pins, last is the button pin 
    */
} 

RotaryEncoder::~RotaryEncoder() {} 

void RotaryEncoder::init() { 

    pinMode(in_pin1,INPUT); 
    pinMode(in_pin2,INPUT); 
    pinMode(button_pin,INPUT_PULLUP); 

    l = digitalRead(in_pin1);
    r = digitalRead(in_pin2);  

}

Input RotaryEncoder::check_increment() { 
    l = digitalRead(in_pin1);
    r = digitalRead(in_pin2);
    lrmem = ((lrmem & 0b00000011) << 2) + 2*l + r;
    lrsum = lrsum + TRANS[lrmem];
    /* encoder not in the neutral state */
    if(lrsum % 4 != 0) return(Input::R_NOP);
    /* encoder in the neutral state */
    if (lrsum == 4) {
        lrsum=0;
        return(Input::R_INC);
        }
    if (lrsum == -4) {
        lrsum=0;
        return(Input::R_DEC);
        }
    /* lrsum > 0 if the impossible transition */
    lrsum=0;
    return Input::R_NOP;
}

Input RotaryEncoder::check_button() { 
    //two modes long and short press 

    //start timer for button 
    auto pin = digitalRead(button_pin); 
    if (pin == LOW && button_toggle == false) { //button pushed down and toggle not triggered yet (starts timer)
        press_start = millis(); 
        button_toggle = true; 
        return Input::R_NOP; 
    }
    else if (pin == HIGH && button_toggle == true)
    {
        //button released
        button_toggle = false; 
        
        //check that the press is not just noise 
        auto t_held = millis() - press_start; 
        if (t_held > noise_floor)
        {
            return (t_held > long_press) ? Input::R_PUSH_LONG : Input::R_PUSH_SHORT; 
        }
    }
    return Input::R_NOP; 

}


void RotaryEncoder::update() { 
    auto out = check_button(); 
    auto r_out = check_increment(); 
    if (out != Input::R_NOP) { 
        output_buffer = out;
    }
    else if (r_out != Input::R_NOP)
    {
        output_buffer = r_out; 
    }
}


Input RotaryEncoder::pull_cache() { 
    auto o = output_buffer; 
    output_buffer = Input::R_NOP; 
    return o; 
}

Input RotaryEncoder::check_cache() { 
    return output_buffer; 
}