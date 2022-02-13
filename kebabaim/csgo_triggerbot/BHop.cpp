#include "BHop.h"
#include "csgo.hpp"
#include <iostream>

BHop::BHop(const Memory* Mem, const LocalEntity* locEnt): m_Mem(Mem), m_locEnt(locEnt)
{
	isEnabled = true;
    isAutoStrafeOn = false;
}

void BHop::run()
{
    // Checking if spacebar is pressed down
    if (isEnabled && GetAsyncKeyState(VK_SPACE) < 0)
    {
        if ((m_locEnt->GetFlags() & FL_ONGROUND))
        {
            // +jump
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceJump, 5);
            Sleep(10);
            // -jump
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceJump, 4);
        }
    }

    
    if (!isAutoStrafeOn) return;

    // checking if player is in the air
    if (!(m_locEnt->GetFlags() & FL_ONGROUND))
    {
        // if mouse moves left move left
        if (m_locEnt->GetViewAngles().y > prevViewAngles.y)
        {
            // +left
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceLeft, 5);
            Sleep(1);
            // -left
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceLeft, 4);
        }
        // if mouse moves right move right
        else if (m_locEnt->GetViewAngles().y < prevViewAngles.y)
        {
            // +right
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceRight, 5);
            Sleep(1);
            // -right
            m_Mem->Write<int>(m_Mem->Client + signatures::dwForceRight, 4);
        }
        prevViewAngles = m_locEnt->GetViewAngles();
    }
}
