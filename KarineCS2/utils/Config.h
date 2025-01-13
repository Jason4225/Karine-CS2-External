#pragma once
#include <string>
#include <vector>
#include <json/Json.hpp>

#define CONFIG_PATH "C:\\Shitware\\Cfgs\\"

namespace vars
{
	inline bool esp = false;
}

class CConfig
{
private:
	nlohmann::json SaveVars();
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