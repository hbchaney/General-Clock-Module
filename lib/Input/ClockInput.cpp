#include "ClockInput.h"


using namespace input; 


InputManager::InputManager(uint8_t left_button_pin,
                 uint8_t right_button_pin, 
                 uint8_t re_pin_1, 
                 uint8_t re_pin_2, 
                 uint8_t re_sw_pin) : 
left_button{left_button_pin, 500}, 
right_button{right_button_pin, 500}, 
re{re_pin_1,re_pin_2,re_sw_pin}
{}

ClockInput InputManager::get_input() 
{
    if (auto output = left_button.get_output_cache(); output == press_type::Short)
    {
        return ClockInput::LEFT_PRESS; 
    }
    else if (auto output = right_button.get_output_cache(); output == press_type::Short)
    {
        return ClockInput::RIGHT_PRESS; 
    }
    else if (auto output = re.pull_cache(); output != Input::R_NOP)
    {
        switch (output) 
        {
            case Input::R_INC: 
                return ClockInput::INC; 
            case Input::R_DEC: 
                return ClockInput::DEC; 
            case Input::R_PUSH_SHORT: 
                return ClockInput::ROT_PRESS;
        }
    }
    return ClockInput::NONE; 
}
