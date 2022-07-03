#pragma once

#include "entityListManager.h"
#include "memory.h"
#include <memory>


class Radar
{
public:
	Radar(std::shared_ptr<Memory> _mem, std::shared_ptr<EntityListManager> _entList);

	void run();
	bool isEnabled = true;

private:
	std::shared_ptr<Memory> mem;
	std::shared_ptr<EntityListManager> entList;
};

