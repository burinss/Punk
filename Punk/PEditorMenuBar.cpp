#include "PEditorMenuBar.hpp"
#include "imgui.h"

void PEditorMenuBar::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
