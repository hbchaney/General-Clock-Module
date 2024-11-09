#pragma once 
#include "lp5868.h"
#include <array>
#include <cctype>
#include "DebugLog.h"
#include "Lexicon.h"

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
    void display_lex(const Lexicon& top, const Lexicon& bot); 

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