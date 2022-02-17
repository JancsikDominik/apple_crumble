#pragma once

#include "memory.h"
#include "localEntity.h"

class Triggerbot
{
public:
	Triggerbot(const Memory* Mem, const LocalEntity* locEnt);

	void enable();

	bool teamTrigger = false;
	bool isEnabled;
	int delay = 10;

private:
	const Memory* m_Mem;
	const LocalEntity* m_locEnt;
	
};

