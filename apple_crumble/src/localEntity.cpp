#include "../includes/localEntity.h"
#include "../includes/csgo.hpp"

// shallow copy (why would we need multiple memory managers, the cheat class can provide it for us.
LocalEntity::LocalEntity(std::shared_ptr<Memory> MemoryManager) : m_MemoryManager(MemoryManager)
{
}

DWORD LocalEntity::GetLocalPlayer() const
{
	return m_MemoryManager->Read<DWORD>(m_MemoryManager->Client + signatures::dwLocalPlayer);
}

int LocalEntity::GetiCross() const
{
	return m_MemoryManager->Read<int>(GetLocalPlayer() + netvars::m_iCrosshairId);
}

int LocalEntity::GetTeamID() const
{
	return m_MemoryManager->Read<int>(GetLocalPlayer() + netvars::m_iTeamNum);
}

int LocalEntity::GetFlags() const
{
	return m_MemoryManager->Read<int>(GetLocalPlayer() + netvars::m_fFlags);
}

ViewAngles_t LocalEntity::GetViewAngles() const
{
	int clientState = m_MemoryManager->Read<int>(m_MemoryManager->Engine + signatures::dwClientState);
	return m_MemoryManager->Read<ViewAngles_t>(clientState + signatures::dwClientState_ViewAngles);
}

DWORD LocalEntity::GetCurrWeaponEntity() const
{
	return m_MemoryManager->Read<DWORD>(GetLocalPlayer() + netvars::m_hActiveWeapon) & 0xFFF;
}

DWORD LocalEntity::GetCurrWeapon() const
{
	return m_MemoryManager->Read<DWORD>(m_MemoryManager->Client + signatures::dwEntityList + (GetCurrWeaponEntity() - 1) * 0x10);
}

int LocalEntity::GetCurrWeaponID() const
{
	return m_MemoryManager->Read<int>(GetCurrWeapon() + netvars::m_iItemDefinitionIndex);
}

float LocalEntity::GetAccuracyPenalty() const
{
	return m_MemoryManager->Read<float>(GetCurrWeapon() + netvars::m_fAccuracyPenalty);
}

int LocalEntity::GetHP() const
{
	return m_MemoryManager->Read<int>(GetLocalPlayer() + netvars::m_iHealth);
}
