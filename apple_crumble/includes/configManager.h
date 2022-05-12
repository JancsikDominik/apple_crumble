#pragma once
#include <tuple>
#include "json.hpp"

struct settings_t
{
	bool isTriggerEnabled = true;
	int triggerDelay = 0;
	bool teamTrigger = false;
	bool isBhopEnabled = true;
	bool isGlowEnabled = true;
	bool teamGlow = false;
	bool hpGlow = true;
	bool fullBloom = false;
	std::tuple<uint32_t, uint32_t, uint32_t> enemyGlowColor = std::make_tuple(255, 0, 0);
	std::tuple<uint32_t, uint32_t, uint32_t> teamGlowColor = std::make_tuple(255, 0, 0);
	bool isPotatoModeEnabled = false;
	bool isChamsEnabled = false;
	std::tuple<uint32_t, uint32_t, uint32_t> chamsColor = std::make_tuple(255, 255, 255);;
	int chamsBrightness = 80;
	bool isRadarEnabled = true;

};

static class ConfigManager
{
public:
	static settings_t LoadConfig();
	static void SaveConfig(const settings_t& settings);
private:
};
