#include "glowESP.h"
#include "csgo.hpp"
#include <iostream>
#include <thread>

GlowESP::GlowESP(Memory* Mem, LocalEntity* LocEnt, EntityListManager* entListManager) :
	m_Mem(Mem), m_LocEnt(LocEnt), m_EntList(entListManager)
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

void GlowESP::enable()
{
	if (potatoMode)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	if (!isEnabled) return;

	for (int i = 0; i < m_EntList->GetEnemyCount(); i++)
	{
		bool dormant = m_Mem->Read<bool>(m_EntList->GetEnemy(i).base + signatures::m_bDormant);
		if (!dormant)
		{
			if (hpGlow)
			{
				int hp = m_EntList->GetEntityHealth(m_EntList->GetEnemy(i).base);
				glow_object.red = 1.f - (hp / 100.f);
				glow_object.green = hp / 100.f;
				glow_object.blue = 0.f;
			}
			else
			{
				glow_object.red = 1.f;
				glow_object.green = 0.f;
				glow_object.blue = 0.f;
			}

			glow_object.fullBloom = fullBloom;
			WriteGlow(m_EntList->GetEnemy(i).glow_index);
		}
	}

	if (!teamGlow) return;

	for (int i = 0; i < m_EntList->GetTeammateCount(); i++)
	{
		bool dormant = m_Mem->Read<bool>(m_EntList->GetTeammate(i).base + signatures::m_bDormant);

		if (!dormant)
		{
			glow_object.red = 0.f;
			glow_object.green = 0.f;
			glow_object.blue = 1.f;

			glow_object.fullBloom = fullBloom;
			WriteGlow(m_EntList->GetTeammate(i).glow_index);
		}
	}
}
