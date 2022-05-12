#include "../includes/configManager.h"
#include <fstream>

settings_t ConfigManager::LoadConfig()
{
	using json = nlohmann::json;

	std::ifstream in("config.json");
	
	json cfg;
	in >> cfg;
	
	settings_t settings {
		cfg["isTriggerEnable"].get<bool>(),
		cfg["triggerDelay"].get<int>(),
		cfg["isBhopEnabled"].get<bool>(),
		cfg["isGlowEnabled"].get<bool>(),
		cfg["glowColor"].get<std::tuple<int, int, int>>(),
		cfg["isPotatoModeEnabled"].get<bool>(),
		cfg["isChamsEnabled"].get<bool>(),
		cfg["chamsColor"].get<std::tuple<int, int, int>>(),
		cfg["chamsBrightness"].get<int>(),
		cfg["isRadarEnabled"].get<int>()
	};

	return settings;
}

void ConfigManager::SaveConfig(const settings_t& settings)
{
	using json = nlohmann::json;
	
	json cfg = {
		{"isTriggerEnabled", settings.isTriggerEnabled},
		{"triggerDelay", settings.triggerDelay},
		{"isBhopEnabled", settings.isBhopEnabled},
		{"isGlowEnabled", settings.isGlowEnabled},
		{"glowColor", settings.glowColor},
		{"isPotatoModeEnabled", settings.isPotatoModeEnabled},
		{"isChamsEnabled", settings.isChamsEnabled},
		{"chamsColor", settings.chamsColor},
		{"chamsBrightness", settings.chamsBrightness},
		{"isRadarEnabled", settings.isRadarEnabled}
	};

	std::ofstream off("config.json");
	off << std::setw(4) << cfg << std::endl;
}
