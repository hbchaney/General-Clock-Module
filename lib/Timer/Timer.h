#pragma once 
#include "input.h"
#include <Arduino.h>
#include "eeprom_M24C02.h"

enum state { 
    STOPPED, 
    STARTED,
    RESET,
    TIME_SET,
    FINISHED
}; 

enum setting { 
        SECONDS, 
        MINUTES, 
};

class Timer 
{ 

    private: 

    int timer_no; //dictates where data will be stored in flash for the times 

    short current_minutes{0}; 
    short current_seconds{0};
    short timer_minutes{0}; 
    short timer_seconds{0};  
    bool set_mode = true;

    Input input_cache[2] {Input::none,Input::none}; 

    state current_state = RESET; 

    setting current_setting = SECONDS; 

    short max_minutes = 240;
    short max_seconds = 60; 

    unsigned long pause_time = 0;
    unsigned long start_time = 0; 
    unsigned long time_cache{0}; 
    eeprom::EEPromM24C02& eeprom_dev; 

    ulong finished_start_time{0}; 
    ulong finished_timeout{10000}; 

    //private functions
    void start(); 
    void stop(); 
    void save(); 
    
    void Toggle_start_stop(); 

    void update_current_time(); 

    void stopped_input(); 
    void started_input();
    void reset_input(); 
    void time_set_input(); 
    void finished_input(); 
    void check_input(); 

    public: 
    /// @brief Initializes Timer Object
    Timer(int no, eeprom::EEPromM24C02&); 
    ~Timer(); 

    void init(); 
    /// @brief toggles between the stop and start state also starts after setting the time 
    

    /// @brief  restarts the time
    void restart(); 

    //input processing params 
    void update_cache(Input); 
    
    state get_state() const; 
    setting get_setting() const;

    // display grabbing info 
    int get_minutes() const; 
    int get_seconds() const; 

    void manual_save(int _minutes, int _seconds); 
    
    void update(); 

};