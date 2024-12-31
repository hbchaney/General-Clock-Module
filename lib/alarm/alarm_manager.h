#pragma once
#include "clock_time.h"
#include "eeprom_M24C02.h"

namespace alarm 
{

struct AlarmTime
{   
    uint8_t hours; 
    uint8_t mins; 
    bool am; 
    bool on;

    AlarmTime() = default; 

    utilities::ClockTime to_clock_time(); 
    void set_from_clktime(const utilities::ClockTime& time_in); 
    bool compare_time(const utilities::ClockTime& time_in) const; 
}; 

class AlarmManager
{
    public: 

    AlarmManager(eeprom::EEPromM24C02& eeprom, uint8_t store_start); 

    void init(); 

    //returns true if alarm is triggered
    bool check_alarm(const utilities::ClockTime& time_in); 

    int get_alarm_index() const; 
    void set_alarm_index(int ind); 
    void next_alarm(); 
    void previous_alarm(); 
    void save_alarm(); 
    AlarmTime& get_current_alarm(); 

    private:

    // |   .   |   .   |  ..  | .... |........| 2 bytes
    //  on/off   am/pm   none   hour    mins
    void read_from_eeprom();  

    uint8_t store_addr;
    eeprom::EEPromM24C02& eeprom_ref;  

    int current_time = 0;   
    AlarmTime times[3]; 
}; 

} //eof alarm 