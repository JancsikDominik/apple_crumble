#pragma once
#include "memory.h"
#include <memory>

struct ViewAngles_t
{
	ViewAngles_t(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {};
	ViewAngles_t(const ViewAngles_t& viewAngles) : x(viewAngles.x), y(viewAngles.y), z(viewAngles.z) {};

	float x, y, z;
};

class LocalEntity
{
public:
	LocalEntity(std::shared_ptr<Memory> memoryManager);

	// Getters
	DWORD GetLocalPlayer() const;
	int GetiCross() const;
	int GetTeamID() const;
	int GetFlags() const;
	ViewAngles_t GetViewAngles() const;
	DWORD GetCurrWeaponEntity() const;
	DWORD GetCurrWeapon() const;
	int GetCurrWeaponID() const;
	float GetAccuracyPenalty() const;
	int GetHP() const;

private:
	std::shared_ptr<Memory> m_MemoryManager;
};

