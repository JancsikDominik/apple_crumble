#include "triggerbot.h"
#include "csgo.hpp"
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
        // Retrieve the EntityBase, using dwEntityList                                           Getting entity in the crosshair 0x10 == sizeof(entity)
        DWORD TriggerEntityBase = m_Mem->Read<DWORD>(m_Mem->Client + signatures::dwEntityList + ((m_locEnt->GetiCross() - 1) * 0x10));
        int TriggerEntityTeam = m_Mem->Read<int>(TriggerEntityBase + netvars::m_iTeamNum);
        bool TriggerEntityDormant = m_Mem->Read<bool>(TriggerEntityBase + signatures::m_bDormant);

        // delay before shot
        delay == 0 ? std::this_thread::sleep_for(std::chrono::microseconds(10)) : Sleep(delay);
        // checking if entity is a valid entity (a player), and entity team
        if ((m_locEnt->GetiCross() > 0 && m_locEnt->GetiCross() <= 64) && (TriggerEntityBase != NULL) && (TriggerEntityTeam != NULL) && (!TriggerEntityDormant))
        {
            if (teamTrigger)
            {
                // +attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 5);
                Sleep(5);
                // -attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 4);
            }
            else if (TriggerEntityTeam != m_locEnt->GetTeamID())
            {
                // +attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 5);
                Sleep(5);
                // -attack
                m_Mem->Write<int>(m_Mem->Client + signatures::dwForceAttack, 4);
            }
        }
    }
}
