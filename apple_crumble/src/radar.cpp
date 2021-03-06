#include "radar.h"
#include "csgo.hpp"
#include <thread>

Radar::Radar(std::shared_ptr<Memory> _mem, std::shared_ptr<EntityListManager> _entList): 
	mem(_mem), entList(_entList)
{
}

void Radar::run()
{
	for (int i = 0; i < entList->GetEnemyCount(); i++)
	{
		mem->Write(entList->GetEnemy(i).base + netvars::m_bSpotted, true);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
