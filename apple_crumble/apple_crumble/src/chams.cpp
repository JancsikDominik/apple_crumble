#include "../includes/chams.h"
#include <thread>

Chams::Chams(Memory* _mem, EntityListManager* _entList, LocalEntity* _locEnt) : mem(_mem), entList(_entList), locEnt(_locEnt)
{
    r = 255;
    g = 255;
    b = 255;
}

void Chams::run()
{
    while (true)
    {
        if (isEnabled)
        {
            this->enable();
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void Chams::enable()
{
    isEnabled = true;
    float brightness = 80.f;
    int playerteamID = locEnt->GetTeamID();

    for (int i = 1; i < 512; i++)
    {
        DWORD dwEntityList = entList->GetEntityAddr(i);
        int classid = entList->GetEntityClassID(dwEntityList);

        int entTeamID = mem->Read<int>(dwEntityList + netvars::m_iTeamNum);

        if (classid != 40 && classid != 140)
        {
             continue;
        }

        if (classid == 40 && entTeamID != playerteamID) 
        {
            //Model Color
            mem->Write<BYTE>(dwEntityList + 0x70, BYTE(r));
            mem->Write<BYTE>(dwEntityList + 0x71, BYTE(g));
            mem->Write<BYTE>(dwEntityList + 0x72, BYTE(b));
        }
        else 
        {
            //Viewmodel Color
            BYTE NewNormalColor = (brightness == 0) ? 255 : (BYTE)(255.f / (brightness / 10.f));
            mem->Write<BYTE>(dwEntityList + 0x70, BYTE(NewNormalColor));
            mem->Write<BYTE>(dwEntityList + 0x71, BYTE(NewNormalColor));
            mem->Write<BYTE>(dwEntityList + 0x72, BYTE(NewNormalColor));
        }
    }
    //Model Brightness
    DWORD thisPtr = (int)(mem->Engine + signatures::model_ambient_min - 0x2c);
    DWORD xored = *(DWORD*)&brightness ^ thisPtr;
    mem->Write<int>(mem->Engine + signatures::model_ambient_min, xored);
}

void Chams::disable()
{
    isEnabled = false;

    float brightness = 0.5f;

    int playerteamID = locEnt->GetTeamID();

    for (int i = 1; i < 512; i++)
    {
        DWORD dwEntityList = entList->GetEntityAddr(i);
        int classid = entList->GetEntityClassID(dwEntityList);

        if (classid != 40 && classid != 140)
        {
             continue;
        }
        
        mem->Write<BYTE>(dwEntityList + 0x70, BYTE(255));
        mem->Write<BYTE>(dwEntityList + 0x71, BYTE(255));
        mem->Write<BYTE>(dwEntityList + 0x72, BYTE(255));
    }

    //Model Brightness
    DWORD thisPtr = (int)(mem->Engine + signatures::model_ambient_min - 0x2c);
    DWORD xored = *(DWORD*)&brightness ^ thisPtr;
    mem->Write<int>(mem->Engine + signatures::model_ambient_min, xored);
}

bool Chams::IsEnabled()
{
    return isEnabled;
}

void Chams::setColors(BYTE r, BYTE g, BYTE b)
{
    this->r = r; this->g = g; this->b = b;
}
