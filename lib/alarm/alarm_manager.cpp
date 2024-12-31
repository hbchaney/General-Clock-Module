#include "alarm_manager.h"

using namespace alarm; 

AlarmManager::AlarmManager(eeprom::EEPromM24C02& eeprom, uint8_t store_start) : 
eeprom_ref{eeprom}, 
store_addr{store_start}
{}

void AlarmManager::init() 
{
    read_from_eeprom(); 
}

bool AlarmManager::check_alarm(const utilities::ClockTime& time_in)
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
    current_time = (current_time - 1) >= 0 ? current_time - 1 : 0;  
}

void AlarmManager::save_alarm() 
{
    //add check to see if changed before changing ? 

    auto& t = times[current_time]; 
    uint8_t ser_out[2]; 
    ser_out[0] = t.mins; 
    ser_out[1] = (t.on & 0x80) | (t.am & 0x40) | (t.mins & 0xf); 
    eeprom_ref.write_bytes(store_addr + (current_time * 2), ser_out, 2); 
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
        ind = std::size(times) - 1; 
    }
}