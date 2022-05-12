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
	std::tuple<float, float, float> enemyGlowColor = std::make_tuple(0.5f, 0.5f, 1.f);
	std::tuple<float, float, float> teamGlowColor = std::make_tuple(1.f, 0, 0);
	bool isPotatoModeEnabled = false;
	bool isChamsEnabled = false;
	std::tuple<uint32_t, uint32_t, uint32_t> chamsColor = std::make_tuple(255, 255, 255);;
	float chamsBrightness = 80.f;
	bool isRadarEnabled = true;

};

static class ConfigManager
{
public:
	static settings_t LoadConfig();
	static void SaveConfig(const settings_t& settings);
private:
};
