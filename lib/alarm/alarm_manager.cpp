#include "alarm_manager.h"

using namespace alarm; 

AlarmManager::AlarmManager(eeprom::EEPromM24C02& eeprom, uint8_t store_start) : 
eeprom_ref{eeprom}, 
store_addr{store_start}, 
times(3) 
{
    times.push_back(&alarm_0); 
    times.push_back(&alarm_1); 
    times.push_back(&alarm_2); 
}

void AlarmManager::init() 
{
    read_from_eeprom(); 
}

bool AlarmManager::check_alarm(const utilities::ClockTime& time_in)
{
    for (int i = 0; i < times.size(); i++)
    {
        if (times[i]->on && times[i]->compare_time(time_in))
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
    current_time = (current_time + 1) % times.size(); 
}

void AlarmManager::set_alarm(const AlarmTime& time_in) 
{
    auto& t = *times[current_time]; 
    t = time_in; 
    uint8_t ser_out[2]; 
    ser_out[0] = t.mins; 
    ser_out[1] = (t.on & 0x80) | (t.am & 0x40) | (t.mins & 0xf); 
    eeprom_ref.write_bytes(store_addr + (current_time * 2), ser_out, 2); 
}

void AlarmManager::read_from_eeprom()
{
    for (int i = 0; i < times.size(); i++)
    {
        auto& t = *times[i]; 
        uint8_t data[2]; 
        eeprom_ref.read_bytes(store_addr + (i * 2), data, 2); 
        t.mins = data[0]; 
        t.hours = data[1] & 0xf; 
        t.on = data[1] & 0x80; 
        t.am = data[1] & 0x40; 
    }
}