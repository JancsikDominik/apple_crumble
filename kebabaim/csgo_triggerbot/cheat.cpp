#include "cheat.h"
#include "csgo.hpp"
#include <iostream>
#include <thread>

Cheat::Cheat(): 
    m_MemoryManager(new Memory()), m_locEnt(new LocalEntity(m_MemoryManager)), 
    triggerbot(new Triggerbot(m_MemoryManager, m_locEnt)), bhop(new BHop(m_MemoryManager, m_locEnt)),
    glow(new GlowESP(m_MemoryManager, m_locEnt))
{
}

Cheat::~Cheat()
{
    delete glow;
    delete triggerbot;
    delete bhop;
    delete m_locEnt;
	delete m_MemoryManager;
}

void Cheat::run()
{
    DrawMenu();
    /*
    while (true)
    {
        Settings();

        // getting game state
        DWORD state = -1;
        GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
        // if game state isn't active we exit the cheat
        if (state != STILL_ACTIVE)
        {
            exit(0);
        }

        // checking if user is in game
        if (IsInGame())
        {
            glow->run();
            triggerbot->run();
            bhop->run();
        }
    }
    */

    std::thread settingsThread(&Cheat::menuLoop, this);
    std::thread glowThread(&Cheat::glowLoop, this);
    std::thread bhopThread(&Cheat::bhopLoop, this);
    std::thread triggerThread(&Cheat::triggerLoop, this);
    std::thread isInGameThread(&Cheat::isInGameLoop, this);

    isInGameThread.detach();
    triggerThread.detach();
    bhopThread.detach();
    settingsThread.detach();
    glowThread.detach();

    while (true)
    {
        // getting game state
        DWORD state = -1;
        GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
        // if game state isn't active we exit the cheat
        if (state != STILL_ACTIVE)
        {
            exit(0);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

sig_on_state Cheat::SigOnState()
{
    // getting game state
    DWORD clientStatePtr = m_MemoryManager->Read<DWORD>(m_MemoryManager->Engine + signatures::dwClientState);
    DWORD signOnState = m_MemoryManager->Read<DWORD>(clientStatePtr + signatures::dwClientState_State);
    return static_cast<sig_on_state>(signOnState);
}

bool Cheat::IsInGame()
{
    return SigOnState() == sig_on_state::FULL_CONNECTED;
}

void Cheat::DrawMenu()
{
    SetConsoleTextAttribute(hConsole, 4);
    std::cout << "-------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "                                                     KEBABAIM                                                      " << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, 5);    
    std::cout << "Visuals:" << std::endl;
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F2] Glow ESP: ";

    if (glow->isEnabled)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }
    SetConsoleTextAttribute(hConsole, 3);

    std::cout << "  > [F3] Team Glow: ";
    if (glow->teamGlow)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F7] Full bloom: ";
    if (glow->fullBloom)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }
    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, 5);
    std::cout << "Misc:" << std::endl;
   
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F4] Triggerbot (triggerkey: mouse5): ";
    if (triggerbot->isEnabled)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }
    
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [UpArrow/DownArrow] Trigger delay: ";
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "[" << triggerbot->delay << "]" << std::endl;

    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F8] Team trigger: ";
    if (triggerbot->teamTrigger)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }

    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F5] Bhop: ";
    if (bhop->isEnabled)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }

    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F6] Autostrafe (kinda buggy): ";
    if (bhop->isAutoStrafeOn)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }
}

void Cheat::Settings()
{
    if (GetAsyncKeyState(VK_F2) < 0)
    {
        glow->isEnabled = !glow->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F3) < 0)
    {
        glow->teamGlow = !glow->teamGlow;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F7) < 0)
    {
        glow->fullBloom = !glow->fullBloom;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F4) < 0)
    {
        triggerbot->isEnabled = !triggerbot->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F8) < 0)
    {
        triggerbot->teamTrigger = !triggerbot->teamTrigger;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_UP) < 0)
    {
        if (triggerbot->delay < 150)
        {
            triggerbot->delay += 10;
            system("CLS");
            DrawMenu();
            Sleep(150);
        }
    }
    if (GetAsyncKeyState(VK_DOWN) < 0)
    {
        if (triggerbot->delay > 0)
        {
            triggerbot->delay -= 10;
            system("CLS");
            DrawMenu();
            Sleep(150);
        }
    }
    if (GetAsyncKeyState(VK_F5) < 0)
    {
        bhop->isEnabled = !bhop->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F6) < 0)
    {
        bhop->isAutoStrafeOn = !bhop->isAutoStrafeOn;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
}

void Cheat::triggerLoop()
{
    DWORD state = -1;
    GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
    // if game state isn't active we exit the cheat
    if (state != STILL_ACTIVE)
    {
        std::terminate();
    }

    while (true)
    {
        if (isInGame) triggerbot->run();
    }
}

void Cheat::bhopLoop()
{
    DWORD state = -1;
    GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
    // if game state isn't active we exit the cheat
    if (state != STILL_ACTIVE)
    {
        std::terminate();
    }

    while (true)
    {
        if (isInGame) bhop->run();
    }
}

void Cheat::glowLoop()
{
    DWORD state = -1;
    GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
    // if game state isn't active we exit the cheat
    if (state != STILL_ACTIVE)
    {
        std::terminate();
    }

    while (true)
    {
        if (isInGame && glow->isEnabled)
        { 
            glow->run(); 
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Cheat::menuLoop()
{
    DWORD state = -1;
    GetExitCodeProcess(m_MemoryManager->GetProcHandle(), &state);
    // if game state isn't active we exit the cheat
    if (state != STILL_ACTIVE)
    {
        std::terminate();
    }

    while (true)
    {
        Settings();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Cheat::isInGameLoop()
{
    while (true)
    {
        isInGame = IsInGame();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
