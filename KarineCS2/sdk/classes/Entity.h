#pragma once
#include "../../Context.h"
#include "../Offsets.h"

#include "../types/Handle.h"
#include "../types/Vector.h"

#include "../Schema.h"

#include <FNV1A.h>
#include <unordered_map>

// Hardcoded bones cause why not
enum EBones
{
	HEAD = 6
};

// https://github.com/perilouswithadollarsign/cstrike15_src
// https://github.com/neverlosecc/source2sdk/blob/cs2/sdk/client.hpp

class CModelState;
class CGameSceneNode;

struct BoneData_t
{
	Vector3_t position;
	char pad[0x14];
};

struct EntitySpottedState_t
{
private:
	[[maybe_unused]] std::uint8_t pad_0x00[0x8]; // 0x0
public:
	bool m_bSpotted; // 0x8        
private:
	[[maybe_unused]] std::uint8_t pad_0x09[0x3]; // 0x9
public:
	uint32_t m_bSpottedByMask[2]; // 0xc        
};

class CEntityIdentity
{
public:
	// uint32_t m_flags
	// CUtlSymbolLarge m_designerName
};

class CEntityInstance
{
public:
	// entity2::CEntityIdentity* m_pEntity;
};

class C_BaseEntity : public CEntityInstance
{
public:
	CGameSceneNode* GetGameSceneNode()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("C_BaseEntity->m_pGameSceneNode"));
		return ctx::memory.Read<CGameSceneNode*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	uint8_t GetTeamNum()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("C_BaseEntity->m_iTeamNum"));
		return ctx::memory.Read<uint8_t>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	int GetHealth()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("C_BaseEntity->m_iHealth"));
		return ctx::memory.Read<int>(reinterpret_cast<uintptr_t>(this) + offset);
	}
};

class C_CSPlayerPawn;
class CBasePlayerController : public C_BaseEntity
{
public:
	uint32_t GetTickBase()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("CBasePlayerController->m_nTickBase"));
		return ctx::memory.Read<uint32_t>(reinterpret_cast<uintptr_t>(this) + offset);
	}
	
	CBaseHandle GetPawnHandle()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("CBasePlayerController->m_hPawn"));
		return ctx::memory.Read<CBaseHandle>(reinterpret_cast<uintptr_t>(this) + offset);
	}
};

class CCSPlayerController : public CBasePlayerController
{
public:
	// CUtlString m_sSanitizedPlayerName

	bool IsPawnAlive()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("CCSPlayerController->m_bPawnIsAlive"));
		return ctx::memory.Read<bool>(reinterpret_cast<uintptr_t>(this) + offset);
	}
};

class C_BasePlayerPawn : public C_BaseEntity
{
public:
	// float m_flMouseSensitivity
	Vector3_t GetOldOrigin()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("C_BasePlayerPawn->m_vOldOrigin"));
		return ctx::memory.Read<Vector3_t>(reinterpret_cast<uintptr_t>(this) + offset);
	}
};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:

};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:
	EntitySpottedState_t GetSpottedState()
	{
		static uintptr_t offset = schemaSystem::GetOffset(HASH("C_CSPlayerPawn->m_entitySpottedState"));
		return ctx::memory.Read<EntitySpottedState_t>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	// bool m_bIsScoped;
	// bool m_bIsDefusing
};

class CEntity
{
public:
	int m_iHealth;
	EntitySpottedState_t m_entitySpottedState;
	Vector3_t m_vOldOrigin;
	uint8_t m_iTeamNum;
	uint32_t m_nTickBase;
	CGameSceneNode* gameSceneNode;
	std::unordered_map<int, Vector3_t> boneArray;
};