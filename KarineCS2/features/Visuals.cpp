#include "Features.h"
#include <Config.h>

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

Vector3_t w2s(ViewMatrix_t vm, Vector3_t& in) {

    Vector3_t out;

    out.x = vm[0][0] * in.x + vm[0][1] * in.y + vm[0][2] * in.z + vm[0][3];
    out.y = vm[1][0] * in.x + vm[1][1] * in.y + vm[1][2] * in.z + vm[1][3];

    float width = vm[3][0] * in.x + vm[3][1] * in.y + vm[3][2] * in.z + vm[3][3];

    if (width < 0.01f) {
        return out;
    }

    float inverseWidth = 1.f / width;

    out.x *= inverseWidth;
    out.y *= inverseWidth;

    float x = screenWidth / 2;
    float y = screenHeight / 2;

    x += 0.5f * out.x * screenWidth + 0.5f;
    y -= 0.5f * out.y * screenHeight + 0.5f;

    out.x = x;
    out.y = y;

    return out;
}

void visuals::esp::Render()
{
    if (!vars::espSwitch)
        return;

    std::lock_guard<std::mutex> lock(sdk::entityMutex);
    for (CEntity& entity : sdk::playerList)
    {
        if (entity.m_iTeamNum == sdk::localEntity.m_iTeamNum) // No Team visuals cause what retard uses that
            continue;

        if (vars::spotted && !entity.m_entitySpottedState.m_bSpotted)
            continue;

        if (vars::boxes)
            AddBox(entity);

        if (vars::names)
            AddName(entity);

        if (vars::healthBar)
            AddHealth(entity);

        if (vars::headDot)
            AddHeadDot(entity);
    }
}

void visuals::esp::AddName(CEntity& entity)
{

}

void visuals::esp::AddBox(CEntity& entity)
{

}

void visuals::esp::AddHealth(CEntity& entity)
{

}

void visuals::esp::AddHeadDot(CEntity& entity)
{
    Vector3_t pos = w2s(sdk::viewMatrix, entity.boneArray.at(HEAD));
    ImGui::GetBackgroundDrawList()->AddCircleFilled({ pos.x, pos.y }, 3.f, ImColor(255, 0, 0));
}