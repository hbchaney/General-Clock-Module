#pragma once 
#include <Arduino.h>
#include <vector>
#include "update.h"


class SequenceBuzzer : base_utilities::UpdateBase
{
    public: 
    SequenceBuzzer(const SequenceBuzzer&) = delete; 
    SequenceBuzzer(uint8_t in_pin_no, 
    const std::vector<ulong>& sequence, 
    uint8_t in_buzzer_vol); 

    void init() override; 
    void update() override;

    void start_buzzing(); 
    void stop_buzzing(); 
    bool get_buzzer_status() const; 

    void pulse(ulong pulse_time, uint8_t vol); 

    void set_buzzer_vol(uint8_t new_vol);  

    bool get_vol_output() const;  

    void test_buzz(); //do not use in real code (uses delay) 

    private: 
    bool pulse_state{false}; 
    ulong pulse_length; 
    ulong pulse_start; 


    uint8_t pin_no; 
    std::vector<ulong> seq; 
    uint8_t buzz_vol; 

    bool buzz_state{false}; 
    ulong seq_start{0}; 
    uint8_t c_seq{0}; 

    //if true is buzzing
    bool buzzer_status{false}; 
};