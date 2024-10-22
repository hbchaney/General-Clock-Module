#pragma once 
#include "button.h"
#include "Rotary_Encoder.h"
#include "update.h"

namespace input
{
    

enum class ClockInput
{
    INC, 
    DEC, 
    ROT_PRESS, 
    LEFT_PRESS, 
    RIGHT_PRESS, 
    NONE
}; 

class InputManager
{
    public: 
    InputManager(uint8_t left_button_pin,
                 uint8_t right_button_pin, 
                 uint8_t re_pin_1, 
                 uint8_t re_pin_2, 
                 uint8_t re_sw_pin); 


    ClockInput get_input(); 


    private: 
    Button left_button; 
    Button right_button; 
    RotaryEncoder re; 

}; 

}