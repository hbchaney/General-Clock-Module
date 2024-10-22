#include "update.h"

using namespace base_utilities; 

//by default they do nothing
void UpdateBase::update() {}
void UpdateBase::fix_update() {}
void UpdateBase::init() {} 


UpdateBase::UpdateBase(UpdateCore up_core) {
    if (up_core == UpdateCore::CORE_0)
    {
        updates.push_back(this); 
    }
    else 
    {
        updates_1.push_back(this); 
    }
}


void UpdateBase::run_updates() 
{
    for (auto u : updates) 
    {
        u->update(); 
    }
} 

void UpdateBase::run_updates_1() 
{
    for (auto u : updates_1) 
    {
        u->update(); 
    }
} 

void UpdateBase::run_fixed_updates()
{
    if (millis() > fix_freq + last_update) 
    {
        last_update = millis(); 
        for (auto u : updates) 
        {
            u->fix_update(); 
        }
    }
}

void UpdateBase::run_fixed_updates_1()
{
    if (millis() > fix_freq + last_update_1) 
    {
        last_update_1 = millis(); 
        for (auto u : updates_1) 
        {
            u->fix_update(); 
        }
    }
}

void UpdateBase::run_inits() 
{
    for (auto u : updates) 
    {
        u->init(); 
    }
}

void UpdateBase::run_inits_1() 
{
    for (auto u : updates_1) 
    {
        u->init(); 
    }
}
