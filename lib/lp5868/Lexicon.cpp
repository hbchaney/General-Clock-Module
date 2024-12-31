#include "Lexicon.h"

using namespace display; 

Lexicon::Lexicon(const std::array<uint8_t,4>& init, uint8_t colon, uint8_t bright)
{
    //set all zero for first four bytes 
    colon_map = colon;
    dot_type_map |= 0x0f; // set first 4 bytes to 1
    for (uint8_t i = 0; i < init.size(); i++) 
    {
        num_chars[i].n = init[i]; 
    }
}

Lexicon::Lexicon(const std::array<char,4>& init, uint8_t colon, uint8_t bright)
{
    colon_map = colon;
    dot_type_map &= 0xf0; // clear first 4 bytes
    for (uint8_t i = 0; i < init.size(); i++) 
    {
        num_chars[i].c = init[i]; 
    }
}

Lexicon& Lexicon::operator=(const std::array<uint8_t,4>& in)
{
    dot_type_map |= 0x0f; // set first 4 bytes to 1
    for (uint8_t i = 0; i < in.size(); i++) 
    {
        num_chars[i].n = in[i]; 
    }
    return *this; 
}

Lexicon& Lexicon::operator=(const std::array<char,4>& in)
{
    dot_type_map &= 0xf0; // clear first 4 bytes
    for (uint8_t i = 0; i < in.size(); i++) 
    {
        num_chars[i].c = in[i]; 
    }
    return *this; 
}

//all non zero padded except for base digit
Lexicon& Lexicon::operator=(const uint16_t in)
{
    num_chars[3].n = in % 10; 
    num_chars[2].n = (in / 10) % 10; 
    num_chars[1].n = (in / 100) % 10;
    num_chars[0].n = (in / 1000) % 10; 
    
    //remove zero padding
    for (uint8_t ind = 0; ind < 3;  ind++)
    {
        if (!num_chars[ind].n)
        {
            num_chars[ind].n = SPACE; 
        }
        else 
        {
            break; 
        }
    }
    set_types(ALL_UINT8); 
    return *this; 
}

Lexicon& Lexicon::operator=(const std::string& in_str)
{
    for (int i = 0; i < in_str.size() && i < 4; i++) 
    {
        num_chars[i].c = in_str[i]; 
    }
    set_types(ALL_CHAR); 
    return *this; 
}

void Lexicon::set(char val, uint8_t ind)
{
    num_chars[ind].c = val; 
    dot_type_map &= ~(0x1 << ind); 
}

void Lexicon::set(uint8_t val, uint8_t ind) 
{
    num_chars[ind].n = val; 
    dot_type_map |= (0x1 << ind); 
}

void Lexicon::set_colon(uint8_t val)
{
    colon_map = val & 0x7; 
}

void Lexicon::set_dots(uint8_t dot)
{
    dot_type_map &= 0xf; 
    dot_type_map |= (dot << 4); 
}

uint8_t Lexicon::get_brightness(uint8_t field) const
{
    return (0x1 << field) & on_status ? brightness : 0x0; 
}

void Lexicon::set_on(uint8_t on)
{
    on_status = on & 0x1f; 
}

uint8_t Lexicon::get_mapped_output(uint8_t ind) const
{
    uint8_t out = ((0x1 << ind) & dot_type_map ? num_map[num_chars[ind].n] : convert_char(num_chars[ind].c)); 
    if ((0x1 << ind << 4) & dot_type_map)
    {
        return out | 0x80; 
    }
    else 
    {
        return out; 
    }
}

void Lexicon::set_types(uint8_t type)
{
    dot_type_map = (dot_type_map & 0xf0) | (type & 0xf); 
}

void Lexicon::set_left(uint8_t num, bool zero_pad)
{
    uint8_t r_val = num % 10; 
    uint8_t l_val = (num % 100) / 10;
    if (!l_val && !zero_pad)
    {
        l_val = SPACE; 
    } 
    num_chars[0].n = l_val; 
    num_chars[1].n = r_val; 
    dot_type_map |= 0b0011; 
}

void Lexicon::set_right(uint8_t num, bool zero_pad)
{
    uint8_t r_val = num % 10; 
    uint8_t l_val = (num % 100) / 10;
    if (!l_val && !zero_pad)
    {
        l_val = SPACE; 
    } 
    num_chars[2].n = l_val; 
    num_chars[3].n = r_val; 
    dot_type_map |= 0b1100; 

}

uint8_t Lexicon::get_colon() const 
{
    return colon_map; 
}

void Lexicon::set_brightness(uint8_t val)
{
    brightness = val; 
}