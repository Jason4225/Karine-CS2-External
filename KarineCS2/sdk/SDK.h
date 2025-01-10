#pragma once
#include <cstdint>
#include <mutex>
#include <vector>

#include "classes/Entity.h"
#include "classes/GameEntitySystem.h"
#include "classes/IGlobalVars.h"
#include "types/ViewMatrix.h"

namespace sdk
{
    bool Initialize();
    void Update();

    inline uintptr_t clientDLL = NULL;
    inline uintptr_t schemaSystemDLL = NULL;
    inline uintptr_t engineDLL = NULL;

    inline CGameEntitySystem* entitySystem;
    inline IGlobalVars globalVars;
    inline ViewMatrix_t viewMatrix;
    inline CEntity localEntity;

    inline std::vector<CEntity> playerList;
}