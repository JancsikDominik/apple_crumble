#pragma once

#include "memory.h"
#include "localEntity.h"
#include <vector>

struct entity_t
{
	DWORD base;
	int glow_index;
};

class EntityListManager
{
public:
	EntityListManager(LocalEntity* locEnt, Memory* mem);

	entity_t GetEnemy(int i) const;
	entity_t GetTeammate(int i) const;
	int GetEnemyCount() const;
	int GetTeammateCount() const;
	DWORD GetEntityAddr(int i) const;
	int GetLifeState(DWORD entity);
	int GetEntityHealth(DWORD entity) const;
	int GetEntityClassID(DWORD entity) const;
	void UpdatePlayerList();

private:
	LocalEntity* m_LocEnt;
	Memory* m_Mem;
	std::vector<entity_t> enemies;
	std::vector<entity_t> teammates;
};

