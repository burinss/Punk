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
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Scene");
			ImGui::MenuItem("Viewport");
			ImGui::MenuItem("Properties");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}
