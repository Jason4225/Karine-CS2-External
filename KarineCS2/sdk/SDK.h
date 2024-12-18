#pragma once
#include <cstdint>
#include <mutex>
#include <vector>

#include "classes/Entity.h"
#include "classes/GameEntitySystem.h"
#include "types/ViewMatrix.h"

namespace sdk
{
    bool Initialize();
    void Update();

    inline uintptr_t clientDLL = NULL;

    inline CGameEntitySystem* entitySystem;
    inline ViewMatrix_t viewMatrix;

    inline std::vector<CEntity> playerList;
    //inline std::mutex playerListMutex;
}