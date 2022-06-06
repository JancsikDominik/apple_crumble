#include "../includes/BHop.h"
#include "../includes/csgo.hpp"
#include <iostream>
#include <thread>

BHop::BHop(std::shared_ptr<Memory> Mem, std::shared_ptr<LocalEntity> locEnt): 
    m_Mem(Mem), m_locEnt(locEnt)
{
	isEnabled = true;
}

void BHop::run()
{
    // Checking if spacebar is pressed down
    if (isEnabled && GetAsyncKeyState(VK_SPACE) < 0)
    {
        // on ground
        if ((m_locEnt->GetFlags() & FL_ONGROUND))
        {
            // +jump
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceJump, 5);
        }
        // in air
        else
        {
            // -jump
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceJump, 4);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
 