#pragma once
#include <Windows.h>

#include "../../Context.h"
#include "../Offsets.h"
#include "../types/Handle.h"

class C_BaseEntity;
class CGameEntitySystem
{
private:
	// @ida: #STR: "Ent %3d: %s class %s name %s\n" ->
	// @ida: #STR: "cl_showents" | "Dump entity list to console." -> 1st Func ->
	// do { entity = GetBaseEntityByIndex(CGameEntitySystem, index);
	void* GetBaseEntityByIndex(int index);
public:

	template <typename T = C_BaseEntity>
	T* Get(int index)
	{
		return reinterpret_cast<T*>(GetBaseEntityByIndex(index));
	}

	template <typename T = C_BaseEntity>
	T* Get(CBaseHandle entityHandle)
	{
		if (!entityHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(GetBaseEntityByIndex(entityHandle.GetEntryIndex()));
	}
};