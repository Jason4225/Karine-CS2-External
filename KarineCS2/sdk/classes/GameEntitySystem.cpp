#include "GameEntitySystem.h"
#include "../SDK.h"

CGameEntitySystem::CGameEntitySystem()
{
	entityList = ctx::memory.Read<uintptr_t>(sdk::clientDLL + offsets::dwEntityList);
}