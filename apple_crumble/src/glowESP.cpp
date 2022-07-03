#include "glowESP.h"
#include "csgo.hpp"
#include <iostream>
#include <thread>

GlowESP::GlowESP(std::shared_ptr<Memory> Mem, std::shared_ptr<LocalEntity> LocEnt, std::shared_ptr<EntityListManager> entListManager) :
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

void GlowESP::run()
{
	if (potatoMode)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	if (!isEnabled) return;

	for (int i = 0; i < m_EntList->GetEnemyCount(); i++)
	{
		entity_t enemy = m_EntList->GetEnemy(i);

		if (enemy.base == NULL) continue;

		bool dormant = m_Mem->Read<bool>(enemy.base + signatures::m_bDormant);
		if (!dormant)
		{
			if (hpGlow)
			{
				int hp = m_EntList->GetEntityHealth(enemy.base);
				glow_object.red = 1.f - (hp / 100.f);
				glow_object.green = hp / 100.f;
				glow_object.blue = 0.f;
			}
			else
			{
				glow_object.red = enemyColor.r;
				glow_object.green = enemyColor.g;
				glow_object.blue = enemyColor.b;
			}

			glow_object.fullBloom = fullBloom;
			WriteGlow(enemy.glow_index);
		}
	}

	if (!teamGlow) 
		return;

	for (int i = 0; i < m_EntList->GetTeammateCount(); i++)
	{
		entity_t teammate = m_EntList->GetTeammate(i);

		if (teammate.base == NULL) continue;

		bool dormant = m_Mem->Read<bool>(teammate.base + signatures::m_bDormant);

		if (!dormant)
		{
			glow_object.red = teamColor.r;
			glow_object.green = teamColor.g;
			glow_object.blue = teamColor.b;

			glow_object.fullBloom = fullBloom;
			WriteGlow(teammate.glow_index);
		}
	}
}

void GlowESP::SetTeamColor(float r, float g, float b)
{
	teamColor.r = r;
	teamColor.g = g;
	teamColor.b = b;
}

void GlowESP::SetEnemyColor(float r, float g, float b)
{
	enemyColor.r = r;
	enemyColor.g = g;
	enemyColor.b = b;
}
