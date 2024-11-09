#include "DisplayManager.h" 

using namespace display; 

DisplayManager::DisplayManager(TwoWire& wire_in, uint8_t v_sync_pin_in) : 
disp{wire_in, v_sync_pin_in}
{}

void DisplayManager::init()
{
    disp.enable_chip(); 
}

void DisplayManager::update()
{
    update_blinks(); 
    if (vals_changed)
    {
        disp.display_lex(current_vals.top, current_vals.bottom); 
    }
}

//if toggled_changed then triggers display to update
DisplayValues& DisplayManager::get_values(bool toggled_changed)
{
    vals_changed = toggled_changed; 
    return current_vals; 
}


void DisplayManager::set_values(const DisplayValues& val)
{
    current_vals = val; 
}

void DisplayManager::update_blinks() 
{
    //check to see that blink enabled on top or bottom 
    if (blink_settings || top_colon_blink || bottom_colon_blink)
    {
        if (blink_start + blink_time < millis())
        {
            vals_changed = true; 
            blink_start = millis(); 
            blink_on = !blink_on;
            if (blink_on) // turn everything on 
            {
                current_vals.top.set_on(0x1f); 
                current_vals.bottom.set_on(0x1f); 
            }
            else 
            {
                uint8_t top_on = top_colon_blink ? ((~blink_settings) & 0xf) : ((~blink_settings) & 0xf) | 0x10; 
                uint8_t bottom_on = bottom_colon_blink ? ((~blink_settings >> 4) & 0xf) : ((~blink_settings >> 4) & 0xf) | 0x10; 
                current_vals.top.set_on(top_on); 
                current_vals.bottom.set_on(bottom_on); 
            }
        }
    }
}