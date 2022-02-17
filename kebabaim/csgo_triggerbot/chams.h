#pragma once

#include "csgo.hpp"
#include "localEntity.h"
#include "entityListManager.h"
#include "memory.h"

class Chams
{
public:
	Chams(Memory* mem, EntityListManager* entList, LocalEntity* locEnt);

	void enable();
	void disable();
	bool IsEnabled();

	void setColors(BYTE r, BYTE g, BYTE b);

private:
	BYTE r, g, b;
	bool isEnabled = false;
	LocalEntity* locEnt;
	Memory* mem;
	EntityListManager* entList;
};

