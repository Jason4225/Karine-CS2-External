#include "Config.h"
#include <filesystem>
#include <Windows.h>
#include <fstream>
#include <FNV1A.h>

CConfig config;

void CConfig::RefreshConfigs()
{
	configs.clear();

	for (auto& entry : std::filesystem::directory_iterator((CONFIG_PATH))) {
		if (entry.path().extension() == (".hh")) {
			auto path = entry.path();
			auto filename = path.filename().string();
			configs.push_back(filename);
		}
	}
}

nlohmann::json CConfig::SaveVars()
{
	nlohmann::json data;

	data["visuals"]["enemies"]["enabled"] = vars::espSwitch;
	data["visuals"]["enemies"]["boxes"] = vars::boxes;
	data["visuals"]["enemies"]["names"] = vars::names;
	data["visuals"]["enemies"]["health"] = vars::healthBar;
	data["visuals"]["enemies"]["spotted"] = vars::spotted;
	data["visuals"]["enemies"]["headdot"] = vars::headDot;

	return data;
}

void CConfig::LoadVars(nlohmann::json& data)
{
	vars::espSwitch = data["visuals"]["enemies"]["enabled"];
	vars::boxes = data["visuals"]["enemies"]["boxes"];
	vars::names = data["visuals"]["enemies"]["names"];
	vars::healthBar = data["visuals"]["enemies"]["health"];
	vars::spotted = data["visuals"]["enemies"]["spotted"];
	vars::headDot = data["visuals"]["enemies"]["headdot"];
}

void CConfig::Save(int id)
{
	std::ofstream file(CONFIG_PATH + configs[id]);

	file << SaveVars().dump();

	file.close();
}

void CConfig::Load(int id)
{
	std::ifstream file(CONFIG_PATH + configs[id]);

	nlohmann::json data = nlohmann::json::parse(file);

	LoadVars(data);

	file.close();
}

void CConfig::Delete(int id)
{
	DeleteFileA((CONFIG_PATH + configs[id]).c_str());
	RefreshConfigs();
}

void CConfig::Create(std::string name)
{
	CloseHandle(CreateFileA((CONFIG_PATH + name.append(".hh")).c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL));

	RefreshConfigs();

	// Find id, shit but works
	for (int i = 0; i < configs.size(); i++)
	{
		if (fnv1a::Hash(name.c_str()) == fnv1a::Hash(configs[i].c_str()))
		{
			Save(i);
		}
	}
}