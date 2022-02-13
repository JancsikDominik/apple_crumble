#pragma once

#include "memory.h"
#include "localEntity.h"
#include <iostream>
#include <Windows.h>

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
	GlowESP(Memory* m_Mem, LocalEntity* m_LocEnt);

	DWORD GetGlowObjManager() const;
	DWORD GetEntity(int i) const;
	int GetEntityClassID(DWORD entity) const;
	bool isEnabled = true;
    bool teamGlow = false;
    bool fullBloom = false;

	void run();

private:
	Memory* m_Mem;
	LocalEntity* m_LocEnt;
    glow_object_t2 glow_object;

	void WriteGlow(int glowIndex) const;
};

