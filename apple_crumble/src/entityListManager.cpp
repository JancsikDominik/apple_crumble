#include "../includes/entityListManager.h"
#include "../includes/csgo.hpp"
#include <thread>

EntityListManager::EntityListManager(std::shared_ptr<LocalEntity> locEnt, std::shared_ptr<Memory> mem):
	m_Mem(mem), m_LocEnt(locEnt), enemies(std::vector<entity_t>()), teammates(std::vector<entity_t>())
{
}

entity_t EntityListManager::GetEnemy(size_t i) const
{
	if (i < enemies.size())
		return enemies[i];
	else
		return entity_t(NULL, NULL);
}

entity_t EntityListManager::GetTeammate(size_t i) const
{
	if (i < teammates.size())
		return teammates[i];
	else
		return entity_t(NULL, NULL);
}

int EntityListManager::GetEnemyCount() const
{
	return enemies.size();
}

int EntityListManager::GetTeammateCount() const
{
	return teammates.size();
}

DWORD EntityListManager::GetEntityAddr(int i) const
{
	return m_Mem->Read<DWORD>(m_Mem->Client + signatures::dwEntityList + (i * 0x10));
}

int EntityListManager::GetLifeState(DWORD entity)
{
	return m_Mem->Read<int>(entity + netvars::m_lifeState);
}

int EntityListManager::GetEntityHealth(DWORD entity) const
{
	return m_Mem->Read<DWORD>(entity + netvars::m_iHealth);
}

int EntityListManager::GetEntityClassID(DWORD entity) const
{
	return m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(m_Mem->Read<int>(entity + 0x8) + 0x8) + 0x1) + 0x14);
}

void EntityListManager::UpdatePlayerList()
{
	while (true)
	{
		int local_player_team = m_LocEnt->GetTeamID();

		enemies.clear();
		teammates.clear();

		for (int i = 0; i < 128; i++)
		{
			DWORD entity = GetEntityAddr(i);
			if ((GetEntityClassID(entity) == 40) && (GetLifeState(entity) == 0))
			{
				int entity_team = m_Mem->Read<int>(entity + netvars::m_iTeamNum);

				if (entity_team == local_player_team)
				{
					teammates.push_back(entity_t(entity, m_Mem->Read<int>(entity + netvars::m_iGlowIndex)));
				}
				else
				{
					enemies.push_back(entity_t(entity, m_Mem->Read<int>(entity + netvars::m_iGlowIndex)));
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
