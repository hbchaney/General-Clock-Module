#include "DisplayManager.h" 

using namespace display; 

DisplayManager::DisplayManager(TwoWire& wire_in, uint8_t v_sync_pin_in) : 
disp{wire_in, v_sync_pin_in}, 
current_mode{DisplayModes::CLOCK_MODE}
{}

void DisplayManager::init()
{
    disp.enable_chip(); 
}

void DisplayManager::update()
{
    switch (current_mode)
    {
        case DisplayModes::CLOCK_MODE: 
            clock_display();
            break; 
        case DisplayModes::YEAR_DATE_MODE: 
            year_date_display(); 
            break; 
    }
}

void DisplayManager::set_display_mode(DisplayModes mode)
{
    current_mode = mode; 
}

void DisplayManager::set_clock_display(DisplayValues& vals)
{
    current_vals = vals;
    vals_changed = true;  
}

void DisplayManager::update_with_time(utilities::ClockTime& clk_time)
{   
    current_vals.hour = clk_time.get_hours(); 
    current_vals.min = clk_time.get_mins();
    current_vals.day = clk_time.get_days();
    current_vals.month = clk_time.get_months(); 
    current_vals.year = clk_time.get_year(); 
    current_vals.is_am = clk_time.is_am(); 
    vals_changed = true; 
}

//1 for blinking 0 for not
void DisplayManager::update_blink_setting(uint8_t blink_setting)
{
    current_vals.blink_setting = blink_setting; 
    vals_changed = true; 
}


void DisplayManager::clock_display() 
{
    if (!vals_changed && !(current_vals.blink_setting & 0b1111))
    {
        return; 
    } 
    else 
    {
        if (blink_start + blink_time < millis() || vals_changed) 
        {
            blink_status = !blink_status; 
            if (!vals_changed)
            {
                blink_start = millis(); 
            }
            if (blink_status)
            {
                disp.display_number(
                    current_vals.hour,
                    current_vals.min, 
                    current_vals.month, 
                    current_vals.day, 
                    brightness, 
                    current_vals.alarm_on, 
                    0b1111,
                    !current_vals.is_am
                ); 
            }
            else 
            {
                disp.display_number(
                    current_vals.hour,
                    current_vals.min, 
                    current_vals.month, 
                    current_vals.day, 
                    brightness, 
                    current_vals.alarm_on, 
                    ~current_vals.blink_setting , 
                    !current_vals.is_am
                ); 
            }
            vals_changed = false; 
        }
    }
}

void DisplayManager::year_date_display() 
{
    if (!vals_changed && !(current_vals.blink_setting & 0b1111))
    {
        return; 
    } 
    else 
    {
        if (blink_start + blink_time < millis() || vals_changed) 
        {
            blink_status = !blink_status; 
            if (!vals_changed)
            {
                blink_start = millis(); 
            }
            if (blink_status)
            {
                disp.display_number(
                    static_cast<uint8_t>(current_vals.year / 100),
                    static_cast<uint8_t>(current_vals.year % 100), 
                    current_vals.month, 
                    current_vals.day, 
                    brightness, 
                    current_vals.alarm_on, 
                    0b1111
                ); 
            }
            else 
            {
                disp.display_number(
                    static_cast<uint8_t>(current_vals.year / 1000),
                    static_cast<uint8_t>(current_vals.year % 100), 
                    current_vals.month, 
                    current_vals.day, 
                    brightness, 
                    current_vals.alarm_on, 
                    ~current_vals.blink_setting
                ); 
            }
            vals_changed = false; 
        }
    }
}


void DisplayManager::alarm_set_display()
{}