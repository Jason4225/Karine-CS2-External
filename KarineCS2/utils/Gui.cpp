#include "Gui.h"
#include <imgui/imgui.h>

void gui::Render()
{
	ImGui::Begin("Ritter88", &open);
	{
		ImGui::Checkbox("ESP", &esp);
		ImGui::End();
	}
}