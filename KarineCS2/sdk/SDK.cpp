#include "SDK.h"
#include "../Context.h"
#include <iostream>
#include <Utils.h>
#include <Psapi.h>
#include "Schema.h"
#include <FNV1A.h>

// timer for timing :)
inline static CTimer timer;
inline static CTimer readTimer;

static const std::vector<int> boneIndexes = { 6 }; // Hardcoded bones cause we only need few of them not all. 6 - Head

bool sdk::Initialize()
{
	clientDLL = ctx::memory.FindModule("client.dll");
	schemaSystemDLL = ctx::memory.FindModule("schemasystem.dll");
	engineDLL = ctx::memory.FindModule("engine2.dll");

	char clientPath[MAX_PATH];
	GetModuleFileNameExA(ctx::memory.GetHandle(), (HMODULE)clientDLL, clientPath, MAX_PATH);
	uintptr_t client = reinterpret_cast<uintptr_t>(LoadLibraryExA(clientPath, NULL, DONT_RESOLVE_DLL_REFERENCES));

	char schemaPath[MAX_PATH];
	GetModuleFileNameExA(ctx::memory.GetHandle(), (HMODULE)schemaSystemDLL, schemaPath, MAX_PATH);
	uintptr_t schemaSys = reinterpret_cast<uintptr_t>(LoadLibraryExA(schemaPath, NULL, DONT_RESOLVE_DLL_REFERENCES));

	PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(client) + reinterpret_cast<PIMAGE_DOS_HEADER>(client)->e_lfanew);

	// @ida: #STR: "gpGlocals->curtime() called while IsInSimulation() is false\n" | "gpGlocals->rendertime() called while IsInSimulation() is true\n" -> xref func -> off_????? = a2
	offsets::dwGlobalVars = ctx::memory.ResolveRVA(ctx::memory.FindSig({ "\x48\x89\x15\x00\x00\x00\x00\x48\x89\x42", "xxx????xxx" }, client, ntHeaders->OptionalHeader.SizeOfImage), 0x3, 0x7) - client;
	
	// @ida: #STR: "client_entities" -> one down -> v3 = qword_181B0CCF8;
	offsets::dwGameEntitySystem = ctx::memory.ResolveRVA(ctx::memory.FindSig({ "\x48\x8B\x1D\x00\x00\x00\x00\x48\x89\x1D", "xxx????xxx" }, client, ntHeaders->OptionalHeader.SizeOfImage), 0x3, 0x7) - client;

	// @ida: CViewRender -> OnRenderStart idx 5 (contains str "C:\\buildworker\\csgo_rel_win64\\build\\src\\game\\client\\view.cpp") -> GetMatricesForView second last param
	offsets::dwViewMatrix = ctx::memory.ResolveRVA(ctx::memory.FindSig({ "\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x89\x4C\x24\x00\x4C\x8D\x0D", "xxx????xxxx?xxxx?xxx" }, client, ntHeaders->OptionalHeader.SizeOfImage), 0x3, 0x7) - client;

	// @ida: CBasePlayerController -> idx 11 -> all down at v13
	offsets::dwLocalPlayerController = ctx::memory.ResolveRVA(ctx::memory.FindSig({ "\x48\x89\x05\x00\x00\x00\x00\x8B\x9E", "xxx????xx" }, client, ntHeaders->OptionalHeader.SizeOfImage), 0x3, 0x7) - client;

	ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(schemaSystemDLL) + reinterpret_cast<PIMAGE_DOS_HEADER>(schemaSystemDLL)->e_lfanew);
	
	offsets::schemaSystemInterface = ctx::memory.ResolveRVA(ctx::memory.FindSig({ "\x48\x89\x05\x00\x00\x00\x00\x4C\x8D\x0D\x00\x00\x00\x00\x0F\xB6\x45\x00\x4C\x8D\x45\x00\x33\xF6", "xxx????xxx????xxx?xxx?xx" }, schemaSys, ntHeaders->OptionalHeader.SizeOfImage), 0x3, 0x7) - schemaSys;

	FreeLibrary((HMODULE)client);
	FreeLibrary((HMODULE)schemaSys);

	entitySystem = ctx::memory.Read<CGameEntitySystem*>(clientDLL + offsets::dwGameEntitySystem);

	// Initialize Schema
	schemaSystem::Initialize();

	return true;
}

void sdk::Update()
{
	while (ctx::running)
	{
		std::vector<CEntity> entities;

		viewMatrix = ctx::memory.Read<ViewMatrix_t>(clientDLL + offsets::dwViewMatrix);

		// Only update every 5 seconds to save reads.
		if (timer.HasElapsed(std::chrono::milliseconds(5000)))
		{
			uintptr_t gVars = ctx::memory.Read<uintptr_t>(clientDLL + offsets::dwGlobalVars);
			globalVars.m_nMaxClients = ctx::memory.Read<int32_t>(gVars + 0x10);
			timer.Reset();
		}

		CCSPlayerController* localCont = ctx::memory.Read<CCSPlayerController*>(clientDLL + offsets::dwLocalPlayerController);
		C_CSPlayerPawn* localPawn = entitySystem->Get<C_CSPlayerPawn>(localCont->GetPawnHandle()); // Maybe search directly for pawn to save reads

		localEntity.m_iHealth = localPawn->GetHealth();
		localEntity.m_entitySpottedState = localPawn->GetSpottedState();
		localEntity.m_vOldOrigin = localPawn->GetOldOrigin();
		localEntity.m_iTeamNum = localPawn->GetTeamNum();

		for (int i = 0; i < globalVars.m_nMaxClients; ++i)
		{
			CCSPlayerController* baseEntity = entitySystem->Get<CCSPlayerController>(i);
			if (!baseEntity)
				continue;

			if (localCont == baseEntity)
				continue;

			if (!baseEntity->IsPawnAlive())
				continue;

			C_CSPlayerPawn* pawn = entitySystem->Get<C_CSPlayerPawn>(baseEntity->GetPawnHandle());

			CEntity entity;

			entity.m_iHealth = pawn->GetHealth();
			entity.m_entitySpottedState = pawn->GetSpottedState();
			entity.m_vOldOrigin = pawn->GetOldOrigin();
			entity.m_iTeamNum = pawn->GetTeamNum();
			entity.gameSceneNode = pawn->GetGameSceneNode();

			// Get bones
			static uint32_t m_modelState = schemaSystem::GetOffset(HASH("CSkeletonInstance->m_modelState"));
			uint64_t boneArray = ctx::memory.Read<uint64_t>(reinterpret_cast<uintptr_t>(entity.gameSceneNode) + m_modelState + 0x80);

			for (int idx : boneIndexes)
			{
				entity.boneArray.insert(std::make_pair(idx, ctx::memory.Read<Vector3_t>(boneArray + (idx * 32))));
			}

			entities.push_back(entity);
		}

		printf("RPS: %i\n", ctx::memory.reads);
		ctx::memory.reads = 0;

		std::lock_guard<std::mutex> lock(entityMutex);
		playerList.clear();
		playerList.assign(entities.begin(), entities.end());

		Sleep(1);
	}
}