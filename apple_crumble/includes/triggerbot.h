#pragma once

#include "memory.h"
#include "localEntity.h"
#include <memory>

class Triggerbot
{
public:
	Triggerbot(std::shared_ptr<Memory> Mem, std::shared_ptr<LocalEntity> locEnt);

	void run();

	bool teamTrigger = false;
	bool isEnabled;
	int delay = 10;

private:
	std::shared_ptr<Memory> m_Mem;
	std::shared_ptr<LocalEntity> m_locEnt;
	
};

