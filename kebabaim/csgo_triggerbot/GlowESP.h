#pragma once

#include "memory.h"
#include "localEntity.h"
#include <iostream>
#include <Windows.h>
#include <vector>

struct Vector
{
    float x, y, z;
};

struct glow_object_t2
{
    float red = 0.5f;
    float green = 0.5f;
    float blue = 1.f;
    float alpha = 0.7f;
    uint8_t padding[8];
    float unknown = 1.f;
    uint8_t padding2[4];
    BYTE renderOccluded = true;
    BYTE renderUnoccluded = false;
    BYTE fullBloom = false;
};

struct entity_t
{
    DWORD base;
    int glow_index;
};

class GlowESP
{
public:
	GlowESP(Memory* m_Mem, LocalEntity* m_LocEnt);

	DWORD GetGlowObjManager() const;
	DWORD GetEntity(int i) const;
    int GetEntityHealth(DWORD entity) const;
	int GetEntityClassID(DWORD entity) const;
    void SetupGlowEnteties();

	bool isEnabled = true;
    bool teamGlow = false;
    bool fullBloom = false;
    bool potatoMode = false;
    bool hpGlow = true;

	void run();

private:
    int GetLifeState(DWORD entity);

	Memory* m_Mem;
	LocalEntity* m_LocEnt;
    glow_object_t2 glow_object;

    std::vector<entity_t> enemies;
    std::vector<entity_t> teammates;

	void WriteGlow(int glowIndex) const;
};

