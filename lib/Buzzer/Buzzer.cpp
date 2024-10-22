#include "Buzzer.h"

//WARNING sequence will be made even if not already
SequenceBuzzer::SequenceBuzzer(uint8_t in_pin_no,
    const std::vector<ulong>& sequence,
    uint8_t in_buzzer_vol) : pin_no{in_pin_no}, seq{sequence}, buzz_vol{in_buzzer_vol}
{
    //drop last seq if not even 
    if (seq.size() % 2)
    {
        seq.pop_back(); 
    }
}

void SequenceBuzzer::init() 
{
    pinMode(pin_no,OUTPUT); 
    digitalWrite(pin_no,LOW); 
}

void SequenceBuzzer::start_buzzing()
{
    if (buzzer_status)
    {
        return; 
    }

    buzzer_status = true; 
    //start the buzzer sequence 
    seq_start = millis(); 
    c_seq = 0; 
    buzz_state = true; 
    analogWrite(pin_no,buzz_vol); 

}

void SequenceBuzzer::stop_buzzing()
{
    if (!buzzer_status) 
    {
        return; 
    }
    seq_start = 0; 
    c_seq = 0; 
    buzzer_status = false; 
    //turn buzzer out pin to low 
    buzz_state = false; 
    analogWrite(pin_no,0); 
}

void SequenceBuzzer::update() 
{

    //check for pulse as well 
    if (pulse_state && pulse_start + pulse_length < millis()) 
    {
        pulse_state = false; 
        analogWrite(pin_no,0); 
    }


    //buzzer on
    if (buzzer_status && seq_start + seq[c_seq] < millis())
    {
        seq_start = millis(); 
        buzz_state = !buzz_state; 
        if (buzz_state)
        {
            analogWrite(pin_no,buzz_vol); 
        }
        else
        {
            analogWrite(pin_no,0); 
        }
        c_seq = (c_seq + 1) % seq.size(); 
    }
}

void SequenceBuzzer::pulse(ulong pulse_time)
{
    pulse_start = millis(); 
    pulse_state = true; 
    pulse_length = pulse_time; 
    analogWrite(pin_no, buzz_vol); 
}

bool SequenceBuzzer::get_buzzer_status() const
{
    return buzzer_status; 
}

bool SequenceBuzzer::get_vol_output() const 
{
    return buzz_state; 
}

void SequenceBuzzer::set_buzzer_vol(uint8_t new_vol) 
{
    buzz_vol = new_vol; 
}

void SequenceBuzzer::test_buzz()
{
    analogWrite(pin_no,buzz_vol); 
    delay(500); 
    analogWrite(pin_no,0); 
}