#pragma once

#include "csgo.hpp"
#include "localEntity.h"
#include "entityListManager.h"
#include "memory.h"

class Chams
{
public:
	Chams(Memory* mem, EntityListManager* entList, LocalEntity* locEnt);

	void run();
	void enable();
	void disable();
	bool IsEnabled();

	void setColors(BYTE r, BYTE g, BYTE b);
	float brightness = 80.f;

private:
	BYTE r, g, b;
	bool isEnabled = false;
	LocalEntity* locEnt;
	Memory* mem;
	EntityListManager* entList;
};

