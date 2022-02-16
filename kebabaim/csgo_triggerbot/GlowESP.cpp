#include "GlowESP.h"
#include "csgo.hpp"
#include <iostream>
#include <thread>

GlowESP::GlowESP(Memory* Mem, LocalEntity* LocEnt): 
	m_Mem(Mem), m_LocEnt(LocEnt), teammates(std::vector<entity_t>(32)), enemies(std::vector<entity_t>(32))
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

int GlowESP::GetEntityHealth(DWORD entity) const
{
	return m_Mem->Read<DWORD>(entity + netvars::m_iHealth);
}

int GlowESP::GetEntityClassID(DWORD entity) const
{
	return m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(entity + 0x8) + 0x8) + 0x1) + 0x14);
}

int GlowESP::GetLifeState(DWORD entity)
{
	return m_Mem->Read<int>(entity + netvars::m_lifeState);
}

void GlowESP::SetupGlowEnteties()
{
	while (true)
	{
		int teammate_cnt = 0;
		int enemy_cnt = 0;
		int local_player_team = m_LocEnt->GetTeamID();

		for (int i = 0; i < 64; i++)
		{
			DWORD entity = GetEntity(i);
			if ((GetEntityClassID(entity) == 40) && (GetLifeState(entity) == 0))
			{
				int entity_team = m_Mem->Read<int>(entity + netvars::m_iTeamNum);

				if (entity_team == local_player_team)
				{
					teammates[teammate_cnt].base = entity;
					teammates[teammate_cnt].glow_index = m_Mem->Read<int>(teammates[teammate_cnt].base + netvars::m_iGlowIndex);
					teammate_cnt++;
				}
				else
				{
					enemies[enemy_cnt].base = entity;
					enemies[enemy_cnt].glow_index = m_Mem->Read<int>(enemies[enemy_cnt].base + netvars::m_iGlowIndex);
					enemy_cnt++;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void GlowESP::run()
{
	if (potatoMode)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	if (!isEnabled) return;

	for (int i = 0; i < enemies.size(); i++)
	{
		bool enemyDormant = m_Mem->Read<bool>(enemies[i].base + signatures::m_bDormant);
		if (!enemyDormant)
		{
			if (hpGlow)
			{
				int hp = GetEntityHealth(enemies[i].base);
				glow_object.red = 1.f - (hp / 100.f);
				glow_object.green = hp / 100.f;
				glow_object.blue = 0.f;
			}
			else
			{
				glow_object.red = 0.5f;
				glow_object.green = 0.5f;
				glow_object.blue = 1.f;
			}


			glow_object.fullBloom = fullBloom;

			WriteGlow(enemies[i].glow_index);
		}
	}

	if (!teamGlow) return;

	for (int i = 0; i < teammates.size(); i++)
	{
		bool teamDormant = m_Mem->Read<bool>(teammates[i].base + signatures::m_bDormant);

		if (!teamDormant)
		{
			glow_object.red =0.f;
			glow_object.green = 0.f;
			glow_object.blue = 1.f;

			glow_object.fullBloom = fullBloom;

			WriteGlow(teammates[i].glow_index);
		}
	}
}
