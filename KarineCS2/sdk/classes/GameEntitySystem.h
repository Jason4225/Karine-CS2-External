#pragma once
#include <Windows.h>

#include "../../Context.h"
#include "../Offsets.h"
#include "../types/Handle.h"

class C_BaseEntity;
class CGameEntitySystem
{
private:
	uintptr_t entityList = NULL;

	void* GetNigga(int index)
	{
		uintptr_t listEntry = ctx::memory.Read<uintptr_t>(entityList + (0x8 * (index & 0x7FFF) >> 9) + 0x10);
		return ctx::memory.Read<void*>(listEntry + 120 * (index & 0x1FF));
	}

	void* GetNiggaV2(int index)
	{
		uintptr_t listEntry = ctx::memory.Read<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
		return ctx::memory.Read<void*>(listEntry + 120 * (index & 0x1FF));
	}
public:
	CGameEntitySystem();

	template <typename T = C_BaseEntity>
	T* Get(int index)
	{
		return reinterpret_cast<T*>(GetNigga(index));
	}

	template <typename T = C_BaseEntity>
	T* Get(CBaseHandle entityHandle)
	{
		if (!entityHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(GetNiggaV2(entityHandle.GetEntryIndex()));
	}
};