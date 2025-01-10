#include "Gui.h"
#include <imgui/imgui.h>

void gui::Render()
{
	ImGui::Begin("Shitware 1337", &open);
	{
		ImGui::Checkbox("ESP", &esp);
		ImGui::End();
	}
}