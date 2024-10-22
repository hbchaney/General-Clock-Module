#include "Timer.h"
#include "DebugLog.h"

Timer::Timer(int no, eeprom::EEProm_M24C02& in_eeprom_dev) : timer_no{no} , eeprom_dev{in_eeprom_dev}
{} 

void Timer::init()
{
    timer_minutes = eeprom_dev.read<uint8_t>(static_cast<uint8_t>(timer_no*4)); 
    timer_seconds = eeprom_dev.read<uint8_t>(timer_no*4 + 2); 

    LOG_INFO("MINS : ", timer_minutes); 
    LOG_INFO("SECONDS : ", timer_seconds); 
    current_minutes = timer_minutes; 
    current_seconds = timer_seconds; 
} 


Timer::~Timer() {} 

void Timer::stop() 
{
    // record the pause time 
    pause_time = millis(); 
    current_state = STOPPED; 
    time_cache += pause_time - start_time; 
}

void Timer::start() 
{ 
    start_time = millis(); 
    current_state =  STARTED; 
}

void Timer::update_current_time() 
{ 
    //check for finished
    if (start_time - time_cache + timer_minutes*60*1000 + timer_seconds*1000 < millis())
    { 
        current_state = FINISHED; 
        finished_start_time = millis(); 
        return; 
    }

    unsigned long end_time = start_time - time_cache + timer_minutes*60*1000 + timer_seconds*1000; //end time in millis 
    unsigned long difference = (end_time - millis())/1000; 
    current_minutes = difference/60; 
    current_seconds = difference - current_minutes*60; 
}

void Timer::Toggle_start_stop() 
{ 
    if (current_state == STARTED) 
    { 
        stop();
    }
    else 
    { 
        start();
    }
}

void Timer::save() 
{ 
    //saves the current minute and second settings 
    eeprom_dev.write((timer_no)*4,current_minutes);
    eeprom_dev.write((timer_no)*4 + 2,current_seconds); 

    timer_minutes = current_minutes; 
    timer_seconds = current_seconds; 
}


void Timer::restart() 
{
    time_cache = 0; 
    current_minutes = timer_minutes; 
    current_seconds = timer_seconds; 
    current_state = RESET; 
    current_setting = SECONDS; 
}

void Timer::update() 
{  
    check_input();
    if (current_state == STARTED) 
    { 
        update_current_time();  
    }
    else if (current_state == FINISHED) 
    {
        //update done timeout 
        if (finished_start_time + finished_timeout < millis())
        {
            restart(); 
        }
    }
    
}

void Timer::update_cache(Input n_input)
{ 
    input_cache[1] = input_cache[0]; 
    input_cache[0] = n_input;
}

void Timer::check_input() 
{ 
    if (input_cache[0] == Input::none)
    { 
        return;
    } 
    switch (current_state) 
    {
    case STOPPED: 
        stopped_input(); 
        break; 
    case STARTED: 
        started_input(); 
        break; 
    case RESET: 
        reset_input(); 
        break;
    case TIME_SET: 
        time_set_input(); 
        break; 
    case FINISHED: 
        finished_input(); 
        break; 
    }

}

void Timer::finished_input() 
{ 
    //if any input other than no input reset the timer 
    if (input_cache[0] != Input::none)
    { 
        restart(); 
    }
}

void Timer::stopped_input() 
{ 
    switch (input_cache[0]) 
    { 
        case Input::A: 
            Toggle_start_stop(); 
            break; 
        case Input::B: 
            restart(); 
            break; 
        case Input::C: 
            restart(); 
            current_state = TIME_SET; 
            break; 
    }
}

void Timer::started_input() 
{ 
    switch (input_cache[0])
    { 
        case Input::A: 
            Toggle_start_stop(); 
            break; 
        case Input::B: 
            //nothing for now cant restart while 
            break;
    }
}

void Timer::reset_input() 
{ 
    switch (input_cache[0]) 
    { 
        case Input::A: 
            Toggle_start_stop(); 
            break; 
        case Input::B: 
            //does nothing for now 
            break; 
        case Input::C: 
            current_state = TIME_SET; 
            break; 
    }
}

void Timer::time_set_input() 
{
   
    if (current_setting == SECONDS) 
    { 
        short& ad_no = current_seconds; 
        short& max_val = max_seconds; 
    
    
        switch (input_cache[0]) 
        { 
            case Input::A: 
                ad_no =  (ad_no < max_val) ? ad_no + 1 : 0; 
                break;
            case Input::A_long:
                ad_no =  (ad_no + 10 < max_val) ? ad_no + 10 : 0; 
                break; 
            case Input::B: 
                ad_no = (ad_no > 0) ? ad_no - 1: max_val; 
                break; 
            case Input::B_long: 
                ad_no = (ad_no - 10 > 0) ? ad_no - 10 : max_val; 
                break; 
            case Input::C:
                //break out of the time set
                current_setting = MINUTES; 
                break; 

        }

        return; 
    }

    if (current_setting == MINUTES) 
    { 
        short& ad_no = current_minutes;
        short& max_val = max_minutes; 
    
    
        switch (input_cache[0]) 
        { 
            case Input::A: 
                ad_no =  (ad_no < max_val) ? ad_no + 1 : 0; 
                break;
            case Input::A_long:
                ad_no =  (ad_no + 10 < max_val) ? ad_no + 10 : 0; 
                break; 
            case Input::B: 
                ad_no = (ad_no > 0) ? ad_no - 1: max_val; 
                break; 
            case Input::B_long: 
                ad_no = (ad_no - 10 > 0) ? ad_no - 10 : max_val; 
                break; 
            case Input::C:
                //break out of the time set mode 
                save(); 
                restart(); 
                break; 
        }
    }


}



int Timer::get_minutes() const
{
    return current_minutes; 
}

int Timer::get_seconds() const
{ 
    return current_seconds; 
}

void Timer::manual_save(int _minutes, int _seconds)
{ 
    eeprom_dev.write(timer_no*4,static_cast<short>(_minutes));
    eeprom_dev.write(timer_no*4 + 2,static_cast<short>(_seconds)); 
}

state Timer::get_state() const
{ 
    return current_state; 
}

setting Timer::get_setting() const 
{ 
    return current_setting; 
}