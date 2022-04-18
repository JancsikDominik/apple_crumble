#pragma once

#include "entityListManager.h"
#include "memory.h"


class Radar
{
public:
	Radar(Memory* _mem, EntityListManager* _entList);

	void run();
	bool isEnabled = true;

private:
	Memory* mem;
	EntityListManager* entList;
};

