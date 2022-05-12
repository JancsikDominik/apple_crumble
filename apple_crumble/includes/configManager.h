#pragma once
#include <tuple>
#include "json.hpp"

struct settings_t
{
	bool isTriggerEnabled;
	int triggerDelay;
	bool isBhopEnabled;
	bool isGlowEnabled;
	std::tuple<uint32_t, uint32_t, uint32_t> glowColor;
	bool isPotatoModeEnabled;
	bool isChamsEnabled;
	std::tuple<uint32_t, uint32_t, uint32_t> chamsColor;
	int chamsBrightness;
	bool isRadarEnabled;
};

class ConfigManager
{
public:
	settings_t LoadConfig();
	void SaveConfig(const settings_t& settings);
private:
};
