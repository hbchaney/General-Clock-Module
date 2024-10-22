#include "TwoSegDisplay.h"

using namespace display; 

TwoSegDisplay::TwoSegDisplay(TwoWire& wire_ref, uint8_t v_sync_pin_in) : disp_driver{wire_ref, v_sync_pin_in}
{}

bool TwoSegDisplay::enable_chip(DISPLAY_MODE disp_mode)
{
    if (disp_mode == DISPLAY_MODE::VSYNC_8BIT || disp_mode == DISPLAY_MODE::VSYNC_16BIT)
        {
            vsync_enabled = true; 
        }
        else 
        {
            vsync_enabled = false;
        }
        return disp_driver.enable_chip(disp_mode); 
}

uint8_t TwoSegDisplay::check_fault()
{
    return disp_driver.check_fault(); 
}

void TwoSegDisplay::toggle_vsync()
{
    if (vsync_enabled)
    {
        disp_driver.toggle_vsync();  
    }
}

void TwoSegDisplay::set_digit(SEGMENT_MAP number, bool top, uint8_t segs, uint8_t value)
{
    for(uint8_t i = 0; i < 8; i++) 
    {
        uint8_t inten = segs & (0x1 << i) ? value : 0;
        auto add = top ? top_seg_map[i] : bot_seg_map[i];  
        disp_driver.set_address(static_cast<uint16_t>(number) + add, inten); 
    }
}

void TwoSegDisplay::display_number(uint8_t hour, 
    uint8_t min, 
    uint8_t month, 
    uint8_t day, 
    uint8_t brightness, 
    bool colon_dot, 
    uint8_t digit_on, 
    bool pm_dot,
    uint8_t zero_heading)
{
    uint8_t hour_tens = hour / 10; 
    uint8_t hour_sing = hour % 10; 

    uint8_t min_tens = min / 10; 
    uint8_t min_sing = min % 10; 

    uint8_t month_tens = month / 10; 
    uint8_t months_sing = month % 10; 

    uint8_t day_tens = day / 10; 
    uint8_t day_sing = day % 10; 

    //whether to show the leading zero or not
    uint8_t zero_value_0 = zero_heading & 0b0001 || hour_tens ? brightness : 0; 
    uint8_t zero_value_1 = zero_heading & 0b0010 || min_tens ? brightness : 0; 
    uint8_t zero_value_2 = zero_heading & 0b0100 || month_tens ? brightness : 0; 
    uint8_t zero_value_3 = zero_heading & 0b1000 || day_tens ? brightness : 0; 

    //first digit
    if (digit_on & 0b1) 
    {
        set_digit(SEGMENT_MAP::DIGIT_ONE, true, num_map[hour_tens], zero_value_0);
        set_digit(SEGMENT_MAP::DIGIT_TWO, true, num_map[hour_sing], brightness);
    }
    else 
    {
        set_digit(SEGMENT_MAP::DIGIT_ONE, true, 0, brightness);
        set_digit(SEGMENT_MAP::DIGIT_TWO, true, 0, brightness);
    }

    if (digit_on & 0b10)
    {
        set_digit(SEGMENT_MAP::DIGIT_THREE, true, num_map[min_tens], zero_value_1);
        set_digit(SEGMENT_MAP::DIGIT_FOUR, true, num_map[min_sing] | pm_dot << 7, brightness);
    }
    else 
    {
        set_digit(SEGMENT_MAP::DIGIT_THREE, true, 0, brightness);
        set_digit(SEGMENT_MAP::DIGIT_FOUR, true, 0, brightness);
    }

    set_digit(SEGMENT_MAP::COLON, true, COLON_NO_DOT, brightness);

    if (digit_on & 0b100)
    {
        set_digit(SEGMENT_MAP::DIGIT_ONE, false, num_map[month_tens], zero_value_2);
        set_digit(SEGMENT_MAP::DIGIT_TWO, false, num_map[months_sing], brightness);
    }
    else 
    {
        set_digit(SEGMENT_MAP::DIGIT_ONE, false, 0, brightness);
        set_digit(SEGMENT_MAP::DIGIT_TWO, false, 0, brightness);
    }

    if (digit_on & 0b1000)
    {
        set_digit(SEGMENT_MAP::DIGIT_THREE, false, num_map[day_tens], zero_value_3);
        set_digit(SEGMENT_MAP::DIGIT_FOUR, false, num_map[day_sing], brightness);
    }
    else 
    {
        set_digit(SEGMENT_MAP::DIGIT_THREE, false, 0, brightness);
        set_digit(SEGMENT_MAP::DIGIT_FOUR, false, 0, brightness);
    }

    set_digit(SEGMENT_MAP::COLON, false, 0b10, brightness);
}


void TwoSegDisplay::display_char()
{

}