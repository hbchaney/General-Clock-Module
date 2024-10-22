#pragma once 

#include <Arduino.h>
#include "DebugLog.h"

namespace utilities
{

class ClockTime
{
    public: 
    ClockTime(uint8_t in_hours, 
              uint8_t in_min, 
              bool is_am,
              uint8_t in_day, 
              uint8_t in_month, 
              uint16_t in_year ); 

    void add_time(uint8_t h, uint8_t m); 
    void sub_time(uint8_t h, uint8_t m); 

    void add_date(uint8_t d, uint8_t m, uint16_t y); 
    void sub_date(uint8_t d, uint8_t m, uint16_t y); 

    void enable_military(bool status); 

    void print_time(); 

    //getters 
    uint8_t get_mins() const; 
    uint8_t get_hours() const; //grabs whatever mode is true
    uint8_t get_hours(bool military); //specify the time mode (rtc needs non military)
    uint8_t get_days() const; 
    uint8_t get_months() const; 
    uint16_t get_year() const; 
    bool is_am() const;  

    //setters 
    bool set_time(uint8_t in_hours, uint8_t in_min, bool is_am, uint8_t in_day, uint8_t in_month, uint16_t in_year); 

    private: 
    bool is_leap_year() const; 
    uint8_t convert_to_military(uint8_t h, bool is_am) const; //convert hours to military time
    std::pair<uint8_t, bool>  convert_to_standard(uint8_t h) const; 
    uint8_t minutes; 
    uint8_t hours; 
    uint8_t day; 
    uint8_t month; 
    uint16_t year; 
    bool am; 
    bool military_time = false; 

    const inline static std::vector<uint8_t> month_days {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
};

}