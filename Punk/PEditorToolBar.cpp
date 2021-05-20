#include "PEditorToolBar.hpp"
#include "imgui.h"
#include "PEditorLayout.hpp"
#include "PTexture.h"

void PEditorToolBar::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.f, 2.f));

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.0, 0.0, 0.0));
	if (ImGui::Begin("Toolbar",(bool*)0,ImGuiWindowFlags_NoScrollWithMouse|ImGuiWindowFlags_NoScrollbar))
	{
		
		ImGui::ImageButton((void*)m_layoutContext->GetIcon("Move")->id, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -10, ImVec4(0.0, 0.0, 0.0, 0.0));
		//ImGui::SameLine();
		ImGui::ImageButton((void*)m_layoutContext->GetIcon("Rotate")->id, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -10, ImVec4(0.0, 0.0, 0.0, 0.0));
		ImGui::End();
	}
	ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(1);
}
