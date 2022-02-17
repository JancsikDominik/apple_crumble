#pragma once

#include "memory.h"
#include "localEntity.h"
#include "entityListManager.h"
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

class GlowESP
{
public:
	GlowESP(Memory* Mem, LocalEntity* LocEnt, EntityListManager* entListManager);

	DWORD GetGlowObjManager() const;

	bool isEnabled = true;
    bool teamGlow = false;
    bool fullBloom = false;
    bool potatoMode = false;
    bool hpGlow = true;

	void enable();

private:
	Memory* m_Mem;
	LocalEntity* m_LocEnt;
    glow_object_t2 glow_object;    
    EntityListManager* m_EntList;

	void WriteGlow(int glowIndex) const;
};

