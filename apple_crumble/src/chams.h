#pragma once

#include "csgo.hpp"
#include "localEntity.h"
#include "entityListManager.h"
#include "memory.h"
#include <memory>

class Chams
{
public:
	Chams(std::shared_ptr<Memory> mem, std::shared_ptr<EntityListManager> entList, std::shared_ptr<LocalEntity> locEnt);

	void run();
	void enable();
	void disable();
	bool IsEnabled();

	void setColors(BYTE r, BYTE g, BYTE b);
	float brightness = 80.f;

private:
	BYTE r, g, b;
	bool isEnabled = false;
	std::shared_ptr<LocalEntity> locEnt;
	std::shared_ptr<Memory> mem;
	std::shared_ptr<EntityListManager> entList;
};

