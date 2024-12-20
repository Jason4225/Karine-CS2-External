#include "GameEntitySystem.h"
#include "../SDK.h"

void* CGameEntitySystem::GetBaseEntityByIndex(int index)
{
	if (index > 0x7FFE)
		return nullptr;

	if ((unsigned int)((int)index >> 9) > 0x3F)
		return nullptr;

	void* v2 = ctx::memory.Read<void*>(reinterpret_cast<uintptr_t>(this) + 0x8 * (index >> 9) + 0x10);
	if (!v2)
		return nullptr; 

	void* v3 = ctx::memory.Read(120 * (index & 0x1FF) + reinterpret_cast<uintptr_t>(v2));
	if (v3)
		return v3;
	else
		return nullptr;
}