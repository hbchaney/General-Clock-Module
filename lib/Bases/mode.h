#pragma once 
#include <functional>


namespace base_utilities 
{

template <typename InputType, typename ModeTypeIndex>
class Mode 
{
    public: 
    Mode(ModeTypeIndex mode_type_in, std::function<void(ModeTypeIndex)> mode_switch_callback = [](ModeTypeIndex){}) : 
        mode_switch{mode_switch_callback}, 
        mode_type{mode_type_in}
    {} 

    virtual void process_input(InputType in) = 0; 
    virtual void tick() = 0; 
    virtual void exit_mode() {} //defaults to noop
    virtual void enter_mode() {} //defaults to noop
    ModeTypeIndex get_index() const {return mode_type; }

    protected: 
    std::function<void(ModeTypeIndex)> mode_switch; 
    const ModeTypeIndex mode_type; 

}; 


} //base_utilities 