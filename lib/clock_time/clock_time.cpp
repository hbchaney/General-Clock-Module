#include "clock_time.h"
using namespace utilities; 

bool ClockTime::compare_time(const ClockTime& t0, const ClockTime& t1)
{
    if (t0.hours == t1.hours && t0.minutes == t1.minutes && t0.am == t1.am)
    {
        return true; 
    }
    else 
    {
        return false;
    } 
}

ClockTime::ClockTime(uint8_t in_hours, 
                     uint8_t in_min, 
                     bool is_am,
                     uint8_t in_day, 
                     uint8_t in_month, 
                     uint16_t in_year ) : 
    minutes{in_min}, 
    hours{in_hours}, 
    am{is_am}, 
    day{in_day}, 
    month{in_month}, 
    year{in_year}
{
    //check to see that the times are in bounds if not report an 
    //error and set to zero 
    if (minutes > 59 || hours > 12 || hours == 0) 
    {
        LOG_ERROR("time at init was invalid"); 
        minutes = 0; 
        hours = 0; 
    }
    if (day > 32 || month > 12)
    {
        LOG_ERROR("date entered at init was invalid"); 
        day = 1; 
        month = 1; 
    }
}

ClockTime::ClockTime()
{
    ClockTime(1,1,1,1,1,2000); 
}

void ClockTime::add_time(uint8_t h, uint8_t m)
{
    uint8_t adjusted_hours = h % 12; 
    uint8_t current_mil_time = convert_to_military(hours,am); 

    minutes += m % 60; 
    minutes %= 60; 

    current_mil_time = (adjusted_hours + current_mil_time) % 24; 
    auto new_time = convert_to_standard(current_mil_time); 
    hours = new_time.first; 
    am = new_time.second;
}

void ClockTime::sub_time(uint8_t h, uint8_t m)
{
    //first solve for the minutes 
    int min_result = minutes - m; 
    minutes = std::abs(min_result + 60) % 60;  

    //need signed in this case 
    int8_t current_military_hour = convert_to_military(hours,am); 
    current_military_hour -= h; 
    auto result = convert_to_standard((current_military_hour + 24) % 24); 
    hours = result.first; 
    am = result.second; 
}

void ClockTime::add_date(uint8_t d, uint8_t m, uint16_t y)
{
    //year first 
    year += y; 
    
    //next month not zero indexed
    uint8_t month_add = month + (m % 12);  
    if (month_add > 12)
    {
        month_add -= 12; 
    }
    month = month_add; 

    //now day
    uint8_t max_days = month_days[month]; 
    //special case for leap years 
    if (month == 2 && is_leap_year())
    {
        max_days = 29; 
    }
    uint8_t day_add = day + (d % max_days); 
    if (day_add > max_days)
    {
        day_add -= max_days; 
    }
    day = day_add > max_days ? max_days : day_add; 
}

void ClockTime::sub_date(uint8_t d, uint8_t m, uint16_t y)
{
    //year overflow 
    year = year - y > year ? 0 : year - y;  

    //month 
    int month_sub = month - (m % 12); 
    if (month_sub <= 0) 
    {
        month_sub += 12; 
    }
    month = static_cast<uint8_t>(month_sub);

    //now day
    uint8_t max_days = month_days[month]; 
    //special case for leap years 
    if (month == 2 && is_leap_year())
    {
        max_days = 29; 
    }
    int day_sub = day - (d % max_days); 
    if (day_sub <= 0)
    {
        day_sub += max_days; 
    }
    day = static_cast<uint8_t>(day_sub > max_days ? max_days : day_sub); 
}

bool ClockTime::is_leap_year() const 
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}


std::pair<uint8_t, bool> ClockTime::convert_to_standard(uint8_t h) const
{
    //log if not in the right range
    if ( h > 23) 
    {
        LOG_ERROR("cannot convert back to standard time : ", h); 
        return {0,0}; 
    }
    uint8_t result_hour; 
    bool temp_am; 

    if (h < 12) 
    {
        // should be am 
        temp_am = true; 
        result_hour = (h != 0) ? h : 12; //return 12 for am 
    }
    else 
    {
        temp_am = false; 
        result_hour = (h - 12 != 0) ? h - 12 : 12; //dont subtract for 12 pm
    }
    return {result_hour,temp_am}; 
}

uint8_t ClockTime::convert_to_military(uint8_t h, bool is_am) const
{
    //log invalid entries 
    if (h > 12 || h < 1)
    {
        LOG_ERROR("invalid hour entered : ", h);
        LOG_ERROR("when converting to military time");  
        return 0; 
    }
    if (h == 12) 
    {
        return !is_am * 12; 
    }
    else 
    {
        return h + !is_am * 12; 
    }
}

void ClockTime::enable_military(bool status)
{
    military_time = status; 
}

void ClockTime::print_time() 
{
    LOG_INFO(hours, ":", minutes, am ? "am " : "pm ", month, "/", day, "/", year); 
}

//getters 
uint8_t ClockTime::get_mins() const { return minutes; }
uint8_t ClockTime::get_hours() const 
{ 
    return military_time ? convert_to_military(hours, am) : hours;  
}

uint8_t ClockTime::get_hours(bool military)
{
    return military ? convert_to_military(hours, am) : hours; 
}

uint8_t ClockTime::get_days() const { return day; }
uint8_t ClockTime::get_months() const { return month; }
uint16_t ClockTime::get_year() const {return year; }

bool ClockTime::is_am() const {return am; } 

bool ClockTime::set_time(uint8_t in_hours, uint8_t in_min, bool is_am, uint8_t in_day, uint8_t in_month, uint16_t in_year)
{
    if (in_min > 59 || in_hours > 12 || in_hours == 0) 
    {
        LOG_ERROR("invalid time attempt"); 
        return false; 
    }
    if (day > 31 || month > 12)
    {
        LOG_ERROR("date entered at init was invalid"); 
        return false; 
    }
    else 
    {
        minutes = in_min; 
        hours = in_hours; 
        am = is_am; 
        day = in_day; 
        month = in_month; 
        year =  in_year; 
        return true; 
    }
}