#pragma once

#include <memory>
#include "memory.h"
#include "localEntity.h"

class BHop
{
public:
	BHop(std::shared_ptr<Memory> Mem, std::shared_ptr<LocalEntity> locEnt);

	void run();

	bool isEnabled;

private:
	std::shared_ptr<Memory> m_Mem;
	std::shared_ptr<LocalEntity> m_locEnt;
};

