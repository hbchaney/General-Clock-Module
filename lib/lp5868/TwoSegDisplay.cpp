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


void TwoSegDisplay::display_lex(const Lexicon& top, const Lexicon& bot)
{
    //top first 
    set_digit(SEGMENT_MAP::DIGIT_ONE, true, top.get_mapped_output(0), top.get_brightness(0)); 
    set_digit(SEGMENT_MAP::DIGIT_TWO, true, top.get_mapped_output(1), top.get_brightness(1)); 
    set_digit(SEGMENT_MAP::DIGIT_THREE, true, top.get_mapped_output(2), top.get_brightness(2)); 
    set_digit(SEGMENT_MAP::DIGIT_FOUR, true, top.get_mapped_output(3), top.get_brightness(3)); 
    set_digit(SEGMENT_MAP::COLON, true, top.get_colon(), top.get_brightness(Lexicon::COLON_BRIGHTNESS)); 

    set_digit(SEGMENT_MAP::DIGIT_ONE, false, bot.get_mapped_output(0), bot.get_brightness(0)); 
    set_digit(SEGMENT_MAP::DIGIT_TWO, false, bot.get_mapped_output(1), bot.get_brightness(1)); 
    set_digit(SEGMENT_MAP::DIGIT_THREE, false, bot.get_mapped_output(2), bot.get_brightness(2)); 
    set_digit(SEGMENT_MAP::DIGIT_FOUR, false, bot.get_mapped_output(3), bot.get_brightness(3)); 
    set_digit(SEGMENT_MAP::COLON, false, bot.get_colon(), bot.get_brightness(Lexicon::COLON_BRIGHTNESS)); 

}