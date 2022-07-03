#include "cheat.h"
#include "csgo.hpp"
#include "configManager.h"
#include <iostream>
#include <fstream>
#include <thread>

Cheat::Cheat(): 
    m_MemoryManager(std::make_shared<Memory>()), 
    m_locEnt(std::make_shared<LocalEntity>(m_MemoryManager)), 
    m_entList(std::make_shared<EntityListManager>(m_locEnt, m_MemoryManager)),
    triggerbot(std::make_unique<Triggerbot>(m_MemoryManager, m_locEnt)), 
    bhop(std::make_unique<BHop>(m_MemoryManager, m_locEnt)),
    glow(std::make_unique<GlowESP>(m_MemoryManager, m_locEnt, m_entList)),
    chams(std::make_shared<Chams>(m_MemoryManager, m_entList, m_locEnt)), 
    radar(std::make_unique<Radar>(m_MemoryManager, m_entList))
{
    std::ifstream f;
    f.open("config.json");
    if (!f)
    {
        ConfigManager::SaveConfig(settings);
    }
    else
    {
        settings = ConfigManager::LoadConfig();
        LoadSettings();
    }
}

void Cheat::run()
{
    DrawMenu();

    std::thread playerListUpdaterThread(&EntityListManager::UpdatePlayerList, m_entList);
    std::thread settingsThread(&Cheat::menuLoop, this);
    std::thread glowThread(&Cheat::glowLoop, this);
    std::thread bhopThread(&Cheat::bhopLoop, this);
    std::thread triggerThread(&Cheat::triggerLoop, this);
    std::thread isInGameThread(&Cheat::isInGameLoop, this);
    std::thread radarThread(&Cheat::radarLoop, this);
    std::thread chamsThread(&Chams::run, chams);

    chamsThread.detach();
    radarThread.detach();
    playerListUpdaterThread.detach();
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
    std::cout << "                                                Apple Crumble                                                      " << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    /*
     *
     *            VISUALS
     *
     */

    SetConsoleTextAttribute(hConsole, 5);    
    std::cout << " Visuals:" << std::endl;
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

    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F9] Hp glow: ";
    if (glow->hpGlow)
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
    std::cout << "  > [F11] Chams: ";
    if (chams->IsEnabled())
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

    /*
     *
     *            MISC
     *
     */

    SetConsoleTextAttribute(hConsole, 5);
    std::cout << " Misc:" << std::endl;
   
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
    std::cout << "  > [F6] Potato-Mode: ";
    if (glow->potatoMode)
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
    std::cout << "  > [HOME] Radar: ";
    if (radar->isEnabled)
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[ON]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[OFF]" << std::endl;
    }

    /*
     *
     *            Others
     *
     */

    SetConsoleTextAttribute(hConsole, 5);
    std::cout << std::endl << " Others:" << std::endl;

    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F1] Lock menu (can't change settings): ";
    if (lockMenu)
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[LOCKED]" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "[UNLOCKED]" << std::endl;
    }
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "  > [F12] Save current settings " << std::endl;
}

void Cheat::Settings()
{
    if (lockMenu)
    {
        if (GetAsyncKeyState(VK_F1) < 0)
        {
            lockMenu = false;
            system("CLS");
            DrawMenu();
            Sleep(150);
        }
        return;
    }

    if (GetAsyncKeyState(VK_F2) < 0)
    {
        glow->isEnabled = !glow->isEnabled;
        settings.isGlowEnabled = glow->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F3) < 0)
    {
        glow->teamGlow = !glow->teamGlow;
        settings.teamGlow = glow->teamGlow;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F7) < 0)
    {
        glow->fullBloom = !glow->fullBloom;
        settings.fullBloom = glow->fullBloom;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F4) < 0)
    {
        triggerbot->isEnabled = !triggerbot->isEnabled;
        settings.isTriggerEnabled = triggerbot->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F8) < 0)
    {
        triggerbot->teamTrigger = !triggerbot->teamTrigger;
        settings.teamTrigger = triggerbot->teamTrigger;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_UP) < 0)
    {
        if (triggerbot->delay < 150)
        {
            triggerbot->delay += 10;
            settings.triggerDelay = triggerbot->delay;
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
            settings.triggerDelay = triggerbot->delay;
            system("CLS");
            DrawMenu();
            Sleep(150);
        }
    }
    if (GetAsyncKeyState(VK_F5) < 0)
    {
        bhop->isEnabled = !bhop->isEnabled;
        settings.isBhopEnabled = bhop->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F6) < 0)
    {
        glow->potatoMode = !glow->potatoMode;
        settings.isPotatoModeEnabled = glow->potatoMode;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F9) < 0)
    {
        glow->hpGlow = !glow->hpGlow;
        settings.hpGlow = glow->hpGlow;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F11) < 0)
    {
        chams->IsEnabled() ? chams->disable() : chams->enable();
        settings.isChamsEnabled = chams->IsEnabled();
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_HOME) < 0)
    {
        radar->isEnabled = !radar->isEnabled;
        settings.isRadarEnabled = radar->isEnabled;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
    if (GetAsyncKeyState(VK_F12) < 0)
    {
        ConfigManager::SaveConfig(settings);
    }
    if (GetAsyncKeyState(VK_F1) < 0)
    {
        lockMenu = true;
        system("CLS");
        DrawMenu();
        Sleep(150);
    }
}

void Cheat::triggerLoop()
{
    while (true)
    {
        if (isInGame)
            triggerbot->run();
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Cheat::bhopLoop()
{
    while (true)
    {
        if (isInGame) 
            bhop->run();
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Cheat::glowLoop()
{
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

void Cheat::radarLoop()
{
    while (true)
    {
        if (isInGame && radar->isEnabled)
        {
            radar->run();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Cheat::LoadSettings()
{
    glow->isEnabled = settings.isGlowEnabled;
    glow->hpGlow = settings.hpGlow;
    glow->fullBloom = settings.fullBloom;
    glow->potatoMode = settings.isPotatoModeEnabled;
    glow->teamGlow = settings.teamGlow;
    auto enemyColor = settings.enemyGlowColor;
    auto teamColor = settings.teamGlowColor;
    glow->SetEnemyColor(std::get<0>(enemyColor), std::get<1>(enemyColor), std::get<2>(enemyColor));
    glow->SetTeamColor(std::get<0>(teamColor), std::get<1>(teamColor), std::get<2>(teamColor));
    triggerbot->isEnabled = settings.isTriggerEnabled;
    triggerbot->delay = settings.triggerDelay;
    triggerbot->teamTrigger = settings.teamTrigger;
    bhop->isEnabled = settings.isBhopEnabled;
    settings.isChamsEnabled ? chams->enable() : chams->disable();
    chams->brightness = settings.chamsBrightness;
    std::tuple<BYTE, BYTE, BYTE> chamsColors = settings.chamsColor;
    chams->setColors(std::get<0>(chamsColors), std::get<1>(chamsColors), std::get<2>(chamsColors));
    radar->isEnabled = settings.isRadarEnabled;
}