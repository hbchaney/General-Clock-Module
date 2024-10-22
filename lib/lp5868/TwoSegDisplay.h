#pragma once 
#include "lp5868.h"


namespace display
{

enum class SEGMENT_MAP : uint16_t
{
    DIGIT_ONE = 0x200,
    DIGIT_TWO = 0x212, 
    DIGIT_THREE = 0x236, 
    DIGIT_FOUR = 0x248, 
    COLON = 0x224

}; 

//eventually it would be good to updated this to use vsync for better performance 
class TwoSegDisplay
{
    public: 
    TwoSegDisplay(TwoWire& wire_ref, uint8_t v_sync_pin_in = 255); 
    bool enable_chip(DISPLAY_MODE disp_mode = DISPLAY_MODE::INSTANT_UPDATE); 
    uint8_t check_fault(); 
    void toggle_vsync(); 

    //number setting 
    void set_digit(SEGMENT_MAP number, bool top, uint8_t segs, uint8_t value); 

    //add field here for zero heading; 
    void display_number(
        uint8_t hour, 
        uint8_t min, 
        uint8_t month, 
        uint8_t day, 
        uint8_t brightness, 
        bool colon_dot = false, 
        uint8_t digit_on = 0b1111, 
        bool pm_dot = false,
        uint8_t zero_heading = 0b0010
    ); 

    //displays chars instead of numbers
    //TODO finish this
    void display_char(

    ); 

    

    //num map 
    static constexpr uint8_t num_map[] {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7 
        0b01111111, // 8 
        0b01100111, // 9
        0b10111111, // 0. with dot
        0b10000110, // 1.
        0b11011011, // 2.
        0b11001111, // 3.
        0b11100110, // 4.
        0b11101101, // 5.
        0b11111101, // 6.
        0b10000111, // 7.
        0b11111111, // 8.
        0b11100111  // 9.
    }; 

    //asci mapping
    static constexpr uint8_t char_map[] {

    }; 

    static constexpr uint8_t COLON_NO_DOT = 0b11; 
    static constexpr uint8_t COLON_DOT = 0b111; 

    //seg maps
    static constexpr uint16_t top_seg_map[]
    {
        15, //a
        13, 
        16, 
        12,
        11,
        17,
        14,
        10 //dp
    }; 

    static constexpr uint16_t bot_seg_map[]
    {
        6, //a2
        8,
        5,
        4,
        3,
        2,
        7,
        1 //dp
    }; 

    private:
    LP5868 disp_driver; 
    bool vsync_enabled{false};
};


}