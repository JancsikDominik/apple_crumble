#pragma once

#include "memory.h"
#include "localEntity.h"
#include "triggerbot.h"
#include "BHop.h"
#include "GlowESP.h"

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
	~Cheat();
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

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Memory* m_MemoryManager;
	LocalEntity* m_locEnt;
	Triggerbot* triggerbot;
	BHop* bhop;
    GlowESP* glow;

    bool isInGame = false;
};

