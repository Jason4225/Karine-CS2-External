#pragma once
#include "../../Context.h"
#include "../Offsets.h"
#include "../types/Handle.h"
#include "../types/Vector.h"

// https://github.com/perilouswithadollarsign/cstrike15_src
// https://github.com/neverlosecc/source2sdk/blob/cs2/sdk/client.hpp

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
	// client::CGameSceneNode* m_pGameSceneNode
	// uint8_t m_iTeamNum
	// uint32_t m_fFlags
	// Vector m_vecAbsVelocity
	
	int GetHealth()
	{
		return ctx::memory.Read<int>((uintptr_t)this + offsets::m_iHealth);
	}
};

class C_CSPlayerPawn;
class CBasePlayerController : public C_BaseEntity
{
public:
	// uint32_t m_nTickBase
	
	CBaseHandle GetPawnHandle()
	{
		return ctx::memory.Read<CBaseHandle>((uintptr_t)this + offsets::m_hPawn);
	}
};

class CCSPlayerController : public CBasePlayerController
{
public:
	// CUtlString m_sSanitizedPlayerName
	bool IsPawnAlive()
	{
		return ctx::memory.Read<bool>((uintptr_t)this + offsets::m_bPawnIsAlive);
	}
};

class C_BasePlayerPawn : public C_BaseEntity
{
public:
	// float m_flMouseSensitivity
	Vector3_t GetOldOrigin()
	{
		return ctx::memory.Read<Vector3_t>((uintptr_t)this + offsets::m_vOldOrigin);
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
		return ctx::memory.Read<EntitySpottedState_t>((uintptr_t)this + offsets::m_entitySpottedState);
	}
	// client::EntitySpottedState_t m_entitySpottedState;
	// bool m_bIsScoped;
	// bool m_bIsDefusing
};

class CEntity
{
public:
	int m_iHealth;
	EntitySpottedState_t m_entitySpottedState;
	Vector3_t m_vOldOrigin;
};