#include "alarm_manager.h"

using namespace alarm; 

utilities::ClockTime AlarmTime::to_clock_time()
{
    return {
        hours, mins, am, 1, 1, 1
    };  
}

void AlarmTime::set_from_clktime(const utilities::ClockTime& time_in) 
{
    hours = time_in.get_hours(false); //always not mil because init wont be either 
    mins = time_in.get_mins(); 
    am = time_in.is_am(); 
}

bool AlarmTime::compare_time(const utilities::ClockTime& time_in) const
{
    return (mins == time_in.get_mins() && 
        hours == time_in.get_hours(false) && 
        am == time_in.is_am()); 
}

void AlarmTime::print_time() const 
{
    LOG_INFO("alarm info : ", hours, ":",mins, am ? " am" : " pm", on ? " on" : " off"); 
}


AlarmManager::AlarmManager(eeprom::EEPromM24C02& eeprom, uint8_t store_start) : 
eeprom_ref{eeprom}, 
store_addr{store_start}
{}

void AlarmManager::init() 
{
    read_from_eeprom(); 
}

bool AlarmManager::check_alarm(const utilities::ClockTime& time_in) const
{
    for (int i = 0; i < std::size(times); i++)
    {
        if (times[i].on && times[i].compare_time(time_in))
        {
            return true; 
        }
    }
    return false; 
}

int AlarmManager::get_alarm_index() const 
{
    return current_time; 
}

void AlarmManager::next_alarm() 
{
    current_time = (current_time + 1) % std::size(times); 
}

void AlarmManager::previous_alarm() 
{
    current_time = (current_time - 1) >= 0 ? current_time - 1 : std::size(times) - 1;  
}

void AlarmManager::save_alarm() 
{
    //add check to see if changed before changing ? 

    auto& t = times[current_time]; 
    uint8_t ser_out[2]; 
    ser_out[0] = t.mins; 
    ser_out[1] = (t.on << 7) | (t.am << 6) | (t.hours & 0xf); 
    if (!eeprom_ref.write_bytes(store_addr + (current_time * 2), ser_out, 2)) 
    {
        LOG_ERROR("Could not save alarm"); 
    }
}

void AlarmManager::read_from_eeprom()
{
    for (int i = 0; i < std::size(times); i++)
    {
        auto& t = times[i]; 
        uint8_t data[2]; 
        eeprom_ref.read_bytes(store_addr + (i * 2), data, 2); 
        t.mins = data[0]; 
        t.hours = data[1] & 0xf; 
        t.on = data[1] & 0x80; 
        t.am = data[1] & 0x40; 

        t.print_time(); 

    }
}

AlarmTime& AlarmManager::get_current_alarm() 
{
    return times[current_time]; 
}

void AlarmManager::set_alarm_index(int ind)
{
    //stop potential undefined crashing behaviour just in case
    if (ind >= std::size(times))
    {
        current_time = std::size(times) - 1; 
    }
    else 
    {
        current_time = 0; 
    }
}

void AlarmManager::print_saved() 
{
    LOG_INFO("Alarm eeprom info : "); 
    for (int i = 0; i < std::size(times); i++)
    {
        uint8_t data[2]; 
        eeprom_ref.read_bytes(store_addr + (i * 2), data, 2);
        LOG_INFO("No. : ", i, " 0x", DebugLogBase::HEX, data[0], " 0x", data[1]); 
    }
}