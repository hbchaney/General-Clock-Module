#pragma once 
#include "Arduino.h"
#include <array>
#include <string>

namespace display {

union NumChar
{
    uint8_t n; 
    char c; 
}; // takes up a total of 8 bits 

/* 
Input type for TwoSegement Display 
converts between chars and numbers has map for mixing both
override get_brightness for custom brightness changing logic
*/
class Lexicon
{  
    public: 
    Lexicon() = default; 
    Lexicon(const Lexicon& in) = default; 
    Lexicon(const std::array<uint8_t,4>& init, uint8_t colon, uint8_t bright = 150);
    Lexicon(const std::array<char,4>& init, uint8_t colon, uint8_t bright = 150);  

    Lexicon& operator=(const Lexicon& in) = default; 
    Lexicon& operator=(const std::array<uint8_t,4>& in); 
    Lexicon& operator=(const std::array<char,4>& in); 
    Lexicon& operator=(const uint16_t in); //for setting one larger number at once (not zero padded)
    Lexicon& operator=(const std::string& in_str); 

    void set(char val, uint8_t ind); 
    void set(uint8_t val, uint8_t ind); 
    void set_colon(uint8_t val); 
    void set_dots(uint8_t dot); //only sets last four bits of dot_type_map 
    void set_on(uint8_t on); 
    void set_left(uint8_t num, bool zero_pad = false); 
    void set_right(uint8_t num, bool zero_pad = false); 

    uint8_t get_mapped_output(uint8_t ind) const; 
    uint8_t get_colon() const; 
    
    void set_brightness(uint8_t val); 
    virtual uint8_t get_brightness(uint8_t field) const; 

    //1 means uint8_t and 0 means char

    static constexpr uint8_t COLON_BRIGHTNESS = 4; 
    static constexpr uint8_t ALL_UINT8 = 0b1111; 
    static constexpr uint8_t ALL_CHAR = 0x0; 

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
        0b00000000 //empty option
    }; 

    static constexpr uint8_t SPACE = std::size(num_map) - 1; 

    //asci mapping
    static constexpr uint8_t char_map[] {
        0b01110111, // A 
        0b01111100, // b
        0b00111001, // C
        0b01011110, // d
        0b01111001, // E
        0b01110001, // F
        0b00111101, // G
        0b01110100, // h
        0b00000110, // I
        0b00011110, // J
        0b01110101, // K
        0b00111000, // L
        0b00010101, // M
        0b01010100, // n
        0b01011100, // o
        0b01110011, // P
        0b01100111, // q
        0b01010000, // r
        0b01101101, // S
        0b01111000, // t
        0b00111110, // U
        0b00111100, // V
        0b00011101, // W
        0b01101010, // X
        0b01101110, // Y
        0b01011011, // Z
    }; 

    static constexpr uint8_t COLON_NO_DOT = 0b11; 
    static constexpr uint8_t COLON_DOT = 0b111; 
    static constexpr uint8_t COLON_BOTTOM = 0b10; 
    static constexpr uint8_t COLON_BOTTOM_DOT = 0b110; 
    static constexpr uint8_t COLON_TOP = 0b1; 
    static constexpr uint8_t COLON_TOP_DOT = 0b101; 

    static constexpr uint8_t convert_char(char input) {
        if (std::isalpha(input))
        {
            input = std::toupper(input); 
            return Lexicon::char_map[input - 65]; 
        }
        else if (std::isdigit(input))
        {
            return Lexicon::num_map[input]; 
        }
        else 
        {
            return 0; //empty space 
        }
    } 

    private:
    void set_types(uint8_t type);


    uint8_t dot_type_map  = 0b1111; // first 4 bytes are types and last four are dots
    uint8_t colon_map = COLON_NO_DOT; 
    std::array<NumChar,4> num_chars = {0,0,0,0}; 
    uint8_t brightness = 150; 
    uint8_t on_status = 0b11111; 
}; 

}