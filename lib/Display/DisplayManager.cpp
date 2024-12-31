#include "DisplayManager.h" 

using namespace display; 

DisplayManager::DisplayManager(TwoWire& wire_in, uint8_t v_sync_pin_in) : 
disp{wire_in, v_sync_pin_in}
{
    current_vals.top.set_brightness(200); 
}

void DisplayManager::init()
{
    disp.enable_chip(); 
}

void DisplayManager::update()
{
    update_blinks(); 
    if (vals_changed)
    {
        vals_changed = false; 
        disp.display_lex(current_vals.top, current_vals.bottom); 
    }
}

//if toggled_changed then triggers display to update
DisplayValues& DisplayManager::get_values(bool toggled_changed)
{
    vals_changed = toggled_changed; 
    blink_on = false; 
    return current_vals; 
}


void DisplayManager::set_values(const DisplayValues& val)
{
    current_vals = val; 
    blink_on = false; 
    vals_changed = true; 
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
                uint8_t top_on = (~blink_settings) & 0xf;
                uint8_t bottom_on = (~blink_settings >> 4) & 0xf; 
                
                top_on |=  (top_colon_blink) ? 0 : 0x10;  
                bottom_on |= (bottom_colon_blink) ? 0 : 0x10; 
                current_vals.top.set_on(top_on); 
                current_vals.bottom.set_on(bottom_on); 
            }
        }
    }
}

void DisplayManager::set_blink(uint8_t blnk_set, bool top_col, bool bottom_col)
{
    blink_settings = blnk_set; 
    top_colon_blink = top_col; 
    bottom_colon_blink = bottom_col; 
    blink_on = false; 
    current_vals.top.set_on(0x1f); 
    current_vals.bottom.set_on(0x1f);
    vals_changed = true; 
}

void DisplayManager::reset_blink() 
{
    set_blink(0,false,false); 
}