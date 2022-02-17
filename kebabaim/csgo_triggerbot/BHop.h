#pragma once

#include "memory.h"
#include "localEntity.h"

class BHop
{
public:
	BHop(const Memory* Mem, const LocalEntity* locEnt);

	void enable();

	bool isEnabled;

private:
	const Memory* m_Mem;
	const LocalEntity* m_locEnt;
	ViewAngles_t prevViewAngles;
};

