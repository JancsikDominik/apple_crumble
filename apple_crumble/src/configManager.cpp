#include "../includes/configManager.h"
#include <fstream>

settings_t ConfigManager::LoadConfig()
{
	using json = nlohmann::json;

	std::ifstream in("config.json");
	
	json cfg;
	in >> cfg;
	
	settings_t settings {
		cfg["isTriggerEnabled"].get<bool>(),
		cfg["triggerDelay"].get<int>(),
		cfg["teamTrigger"].get<bool>(),
		cfg["isBhopEnabled"].get<bool>(),
		cfg["isGlowEnabled"].get<bool>(),
		cfg["teamGlow"].get<bool>(),
		cfg["hpGlow"].get<bool>(),
		cfg["fullBloom"].get<bool>(),
		cfg["enemyGlowColor"].get<std::tuple<float, float, float>>(),
		cfg["teamGlowColor"].get<std::tuple<float, float, float>>(),
		cfg["isPotatoModeEnabled"].get<bool>(),
		cfg["isChamsEnabled"].get<bool>(),
		cfg["chamsColor"].get<std::tuple<int, int, int>>(),
		cfg["chamsBrightness"].get<float>(),
		cfg["isRadarEnabled"].get<bool>()
	};
	in.close();
	return settings;
}

void ConfigManager::SaveConfig(const settings_t& settings)
{
	using json = nlohmann::json;
	
	json cfg = {
		{"isTriggerEnabled", settings.isTriggerEnabled},
		{"triggerDelay", settings.triggerDelay},
		{"teamTrigger", settings.teamTrigger},
		{"isBhopEnabled", settings.isBhopEnabled},
		{"isGlowEnabled", settings.isGlowEnabled},
		{"teamGlow", settings.teamGlow},
		{"hpGlow", settings.hpGlow},
		{"fullBloom", settings.fullBloom},
		{"enemyGlowColor", settings.enemyGlowColor},
		{"teamGlowColor", settings.teamGlowColor},
		{"isPotatoModeEnabled", settings.isPotatoModeEnabled},
		{"isChamsEnabled", settings.isChamsEnabled},
		{"chamsColor", settings.chamsColor},
		{"chamsBrightness", settings.chamsBrightness},
		{"isRadarEnabled", settings.isRadarEnabled}
	};

	std::ofstream off("config.json");
	off << std::setw(4) << cfg << std::endl;
	off.close();
}
