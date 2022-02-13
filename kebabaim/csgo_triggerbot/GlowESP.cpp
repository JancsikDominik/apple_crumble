#include "GlowESP.h"
#include "csgo.hpp"
#include <iostream>

GlowESP::GlowESP(Memory* Mem, LocalEntity* LocEnt): m_Mem(Mem), m_LocEnt(LocEnt)
{
}

void GlowESP::WriteGlow(int glowIndex) const
{
	// glowObject + glowIndex * sizeof(GlowObjectDefinition_t)
	m_Mem->Write<glow_object_t2>(GetGlowObjManager() + (glowIndex * 0x38) + 0x8, glow_object);
}

DWORD GlowESP::GetGlowObjManager() const
{
	return m_Mem->Read<DWORD>(m_Mem->Client + signatures::dwGlowObjectManager);
}

DWORD GlowESP::GetEntity(int i) const
{
	return m_Mem->Read<DWORD>(m_Mem->Client + signatures::dwEntityList + (i * 0x10));
}

int GlowESP::GetEntityClassID(DWORD entity) const
{
	return m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(entity + 0x8) + 0x8) + 0x1) + 0x14);
}

void GlowESP::run()
{
	if (!isEnabled) return;

	for (int i = 0; i < 65; i++)
	{
		DWORD entity = GetEntity(i);
		if (!entity) continue;

		int glow_index = m_Mem->Read<int>(entity + netvars::m_iGlowIndex);
		bool entityDormant = m_Mem->Read<bool>(entity + signatures::m_bDormant);
		int entityTeamID = m_Mem->Read<int>(entity + netvars::m_iTeamNum);

		if (!entityDormant && GetEntityClassID(entity) == 40)
		{
			if (entityTeamID != m_LocEnt->GetTeamID())
			{
				glow_object.red = 0.5f;
				glow_object.green = 0.5f;
				glow_object.blue = 1.f;

				glow_object.fullBloom = fullBloom;

				WriteGlow(glow_index);
			}
			else if (teamGlow)
			{
				glow_object.red = 0.f;
				glow_object.green = 1.f;
				glow_object.blue = 0.f;

				glow_object.fullBloom = fullBloom;

				WriteGlow(glow_index);
			}
		}
	}
}
