#pragma once
#include <string>
#include <vector>
#include <json/Json.hpp>

#define CONFIG_PATH "C:\\Shitware\\Cfgs\\"

namespace vars
{
	inline bool espSwitch = false;
	inline bool boxes = false;
	inline bool names = false;
	inline bool healthBar = false;
	inline bool spotted = false;
	inline bool headDot = false;
}

class CConfig
{
private:
	nlohmann::json SaveVars();
	void LoadVars(nlohmann::json& data);
public:
	std::vector<std::string> configs;

	char currentConfigName[256];
	int currentConfig;

	void RefreshConfigs();

	void Save(int id);
	void Load(int id);
	void Delete(int id);
	void Create(std::string name);
};

extern CConfig config;