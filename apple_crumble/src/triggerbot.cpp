#include "../includes/triggerbot.h"
#include "../includes/csgo.hpp"
#include <iostream>
#include <thread>

Triggerbot::Triggerbot(const Memory* Mem, const LocalEntity* locEnt): m_Mem(Mem), m_locEnt(locEnt)
{
	isEnabled = true;
}

void Triggerbot::run()
{
    if (isEnabled && GetAsyncKeyState(VK_XBUTTON2) < 0)
    {
        int cross = m_locEnt->GetiCross();
        // Retrieve the EntityBase, using dwEntityList                             Getting entity in the crosshair 0x10 == sizeof(entity)
        DWORD TriggerEntityBase = m_Mem->Read<DWORD>(m_Mem->Client + signatures::dwEntityList + ((cross - 1) * 0x10));
        int TriggerEntityTeam = m_Mem->Read<int>(TriggerEntityBase + netvars::m_iTeamNum);
        bool TriggerEntityDormant = m_Mem->Read<bool>(TriggerEntityBase + signatures::m_bDormant);

        // delay before shot
        if (delay != 0)
        {
            // only sleep if it has delay
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        // checking if entity is a valid entity (a player), and entity team
        if ((cross > 0 && cross <= 64) && (TriggerEntityBase != NULL) && (TriggerEntityTeam != NULL) && (!TriggerEntityDormant))
        {
            if (teamTrigger)
            {
                // +attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 5);
                Sleep(5);
                // -attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 4);

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            else if (TriggerEntityTeam != m_locEnt->GetTeamID())
            {
                // +attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 5);
                Sleep(5);
                // -attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 4);

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
    std::this_thread::sleep_for(std::chrono::microseconds(1));
}
