#pragma once

#include <memory>
#include "memory.h"
#include "localEntity.h"
#include "triggerbot.h"
#include "BHop.h"
#include "glowESP.h"
#include "entityListManager.h"
#include "chams.h"
#include "radar.h"
#include "configManager.h"

enum class sig_on_state
{
    STATE_NONE = 0,
    CHALLENGE = 1,
    CONNECTED = 2,
    NEW = 3,
    PRESPAWN = 4,
    SPAWN = 5,
    FULL_CONNECTED = 6,
    CHANGELEVEL = 7
};

class Cheat
{
public:
	Cheat();
	void run();

private:
    sig_on_state SigOnState();
    bool IsInGame();
    void DrawMenu();
    void Settings();

    void triggerLoop();
    void bhopLoop();
    void glowLoop();
    void menuLoop();
    void isInGameLoop();
    void radarLoop();
    void LoadSettings();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::shared_ptr<Memory> m_MemoryManager;
	std::shared_ptr<LocalEntity> m_locEnt;
    std::shared_ptr<EntityListManager> m_entList;
    std::unique_ptr<Triggerbot> triggerbot;
    std::unique_ptr<BHop> bhop;
    std::unique_ptr<GlowESP> glow;
    std::shared_ptr<Chams> chams;
    std::unique_ptr<Radar> radar;

    settings_t settings;
    bool lockMenu = false;
    bool isInGame = false;
};

