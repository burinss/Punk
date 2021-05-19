#include "PEditorViewportWindow.hpp"
#include "imgui.h"
#include "PScene.hpp"
#include "PRenderer3D.hpp"
#include "PRenderPass.hpp"
#include "PFramebuffer.hpp"
#include "ImGuizmo.h"
#include "PCamera.hpp"
#include "PTexture.h"

void PEditorViewportWindow::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(640, 480));
	if (ImGui::Begin("Viewport"))
	{
		auto viewportSize = ImGui::GetContentRegionAvail();
		auto scene = m_layoutContext->GetContext();//m_layoutContext->GetContext()->m_renderer3D->Passes().cend()->get()->Framebuffer()->ID();
		auto passes = scene->m_renderer3D->Passes();
	
		if (!passes.empty()) {
			auto colorpass = passes.at(passes.size() - 1);
			auto frame = colorpass->Framebuffer()->ID();
			ImGui::Image((void*)frame, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		
		if (scene->m_playState == PPlayState::NotStarted)
		{
			if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.1f))
			{
				auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right, 0.1f);
				scene->ActiveCamera()->Move(-delta.x, 0.0f, -delta.y);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
			}
			if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.1f))
			{
				auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle, 0.1f);
				scene->ActiveCamera()->Move(-delta.x, delta.y);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
			}
			if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.1f) && !ImGuizmo::IsOver())
			{
				auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.1f);
				scene->ActiveCamera()->Look(delta.y, -delta.x);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
			}

		}
		auto config = scene->GameConfig();
		config.windowSettings.windowSize = glm::vec2(viewportSize.x, viewportSize.y);
		scene->m_gameSettings = config;
		
		ImGui::End();
	}
	ImGui::PopStyleVar(1);
}
