#pragma once

#include "memory.h"
#include "localEntity.h"
#include <vector>
#include <memory>

struct entity_t
{
	entity_t() { base = NULL; glow_index = 0; }
	entity_t(DWORD b, int glidx): base(b), glow_index(glidx) {}
	DWORD base;
	int glow_index;
};

class EntityListManager
{
public:
	EntityListManager(std::shared_ptr<LocalEntity> locEnt, std::shared_ptr<Memory> mem);

	entity_t GetEnemy(size_t i) const;
	entity_t GetTeammate(size_t i) const;
	int GetEnemyCount() const;
	int GetTeammateCount() const;
	DWORD GetEntityAddr(int i) const;
	int GetLifeState(DWORD entity);
	int GetEntityHealth(DWORD entity) const;
	int GetEntityClassID(DWORD entity) const;
	void UpdatePlayerList();

private:
	std::shared_ptr<LocalEntity> m_LocEnt;
	std::shared_ptr<Memory> m_Mem;
	std::vector<entity_t> enemies;
	std::vector<entity_t> teammates;
};

