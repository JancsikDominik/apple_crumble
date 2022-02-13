#pragma once

#include "memory.h"
#include "localEntity.h"

class BHop
{
public:
	BHop(const Memory* Mem, const LocalEntity* locEnt);

	void run();

	bool isEnabled;
	bool isAutoStrafeOn;
private:
	const Memory* m_Mem;
	const LocalEntity* m_locEnt;
	ViewAngles_t prevViewAngles;
};

