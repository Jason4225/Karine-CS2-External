#include "Gui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Config.h>

void gui::DrawAim()
{
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::SetCursorPosY(70);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("1", { 35, 35 }, 0, &aimTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(96, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "Aimassist");
    }

    ImGui::SetCursorPosY(115);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("2", { 35, 35 }, 1, &aimTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(105, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "Triggerbot");
    }

    if (aimTab == 0)
    {

    }
    else if (aimTab == 1)
    {

    }
}

void gui::DrawMisc()
{
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::SetCursorPosY(70);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("1", { 35, 35 }, 0, &miscTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(76, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "Configs");
    }

    ImGui::SetCursorPosY(115);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("2", { 35, 35 }, 1, &miscTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(105, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "Movement");
    }

    if (miscTab == 0)
    {
        ImGui::SetCursorPosY(70);
        ImGui::SetCursorPosX(60);
        ImGui::BeginListBox("##configs", { 100, 100 });
        {
            for (int i = 0; i < config.configs.size(); ++i)
            {
                if (ImGui::Selectable(config.configs[i].c_str()))
                {
                    config.currentConfig = i;
                    ImGui::OpenPopup("config options");
                }
            }

            if (ImGui::BeginPopup(("config options")))
            {
                if (ImGui::Button("Load", { 100, 25 }))
                {
                    config.Load(config.currentConfig);
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::Button("Save", { 100, 25 }))
                {
                    config.Save(config.currentConfig);
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::Button("Delete", { 100, 25 }))
                {
                    config.Delete(config.currentConfig);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::EndListBox();
        }

        ImGui::SetCursorPosX(60);
        ImGui::InputText("##name", config.currentConfigName, sizeof(config.currentConfigName));

        ImGui::SetCursorPosX(60);
        if (ImGui::Button("Create", { 100, 25 }))
        {
            config.Create(config.currentConfigName);
        }

        ImGui::SetCursorPosX(60);
        if (ImGui::Button("Refresh", { 100, 25 }))
        {
            config.RefreshConfigs();
        }
    }
    else if (miscTab == 1)
    {

    }
}

void gui::DrawVisuals()
{
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::SetCursorPosY(70);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("1", { 35, 35 }, 0, &visualsTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(76, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "Enemies");
    }

    ImGui::SetCursorPosY(115);
    ImGui::SetCursorPosX(10);
    ImGui::Tab("2", { 35, 35 }, 1, &visualsTab);
    if (ImGui::IsItemHovered())
    {
        ImGui::GetForegroundDrawList()->AddRectFilled(ImGui::GetMousePos() + ImVec2(20, 0), ImGui::GetMousePos() + ImVec2(105, 25), ImColor(8, 18, 28, 240), 5);
        ImGui::GetForegroundDrawList()->AddText(ImGui::GetMousePos() + ImVec2(25, 2.5f), ImColor(255, 255, 255), "World");
    }

    if (miscTab == 0)
    {
        ImGui::SetCursorPosY(70);
        ImGui::SetCursorPosX(60);
        ImGui::Checkbox("Esp", &vars::esp);
    }
    else if (miscTab == 1)
    {

    }
}

void gui::Render()
{
    ImGui::SetNextWindowSize({ 500, 400 });
    ImGui::Begin("Shitware", &open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    {
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();

        ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + size, ImColor(0, 20, 38, 210));

        ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(size.x, 60), ImColor(0, 14, 28, 210));
        ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(50, size.y), ImColor(0, 14, 28, 210));

        ImGui::GetWindowDrawList()->AddText(pos + ImVec2(35, 20), ImColor(255, 255, 255), "Shitware 1337");

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 335);
        ImGui::Tab("Aim", { 100, 30 }, 2, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 225);
        ImGui::Tab("Visuals", { 100, 30 }, 1, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 115);
        ImGui::Tab("Misc", { 100, 30 }, 0, &currentTab);

        if (currentTab == 0)
        {
            DrawMisc();
        }
        else if (currentTab == 1)
        {
            DrawVisuals();
        }
        else if (currentTab == 2)
        {
            DrawAim();
        }

        ImGui::End();
    }
}