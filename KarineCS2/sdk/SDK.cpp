#include "SDK.h"
#include "../Context.h"
#include <iostream>

bool sdk::Initialize()
{
	clientDLL = ctx::memory.FindModule("client.dll"); // ctx::memory.Read<uintptr_t>(sdk::clientDLL + offsets::dwEntityList);
	entitySystem = ctx::memory.Read<CGameEntitySystem*>(clientDLL + 0x1B37E58);

	return true;
}

void sdk::Update()
{
	while (ctx::running)
	{
		std::vector<CEntity> entities;
		viewMatrix = ctx::memory.Read<ViewMatrix_t>(clientDLL + offsets::dwViewMatrix);

		for (int i = 0; i < 64; ++i)
		{
			CCSPlayerController* baseEntity = entitySystem->Get<CCSPlayerController>(i);
			if (!baseEntity)
				continue;

			if (!baseEntity->IsPawnAlive())
				continue;

			C_CSPlayerPawn* pawn = entitySystem->Get<C_CSPlayerPawn>(baseEntity->GetPawnHandle());

			CEntity entity;

			entity.m_iHealth = pawn->GetHealth();
			entity.m_entitySpottedState = pawn->GetSpottedState();
			entity.m_vOldOrigin = pawn->GetOldOrigin();
			
			entities.push_back(entity);
		}

		playerList.clear();
		playerList.assign(entities.begin(), entities.end());
	}
}