#include "PEditorHierarchyWindow.hpp"
#include "PunkDefines.hpp"
#include "imgui.h"
#include "PGameObject.hpp"
#include "PEditorLayout.hpp"
#include "PEditorScene.hpp"
#include "PLight.hpp"

void PEditorHierarchyWindow::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	if (ImGui::Begin("Scene Hierarchy"))
	{
		auto width = ImGui::GetWindowSize();
		width.x /= 2;
		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV /*| ImGuiTableFlags_BordersOuterH */| ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

		if (ImGui::BeginTable("2ways", 2, flags))
		{
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();

			for (auto object : m_layoutContext->GetContext()->GameObjects())
				DrawNode(object);

			ImGui::EndTable();
		}
		if (ImGui::IsWindowHovered()&&ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Editor Hierarchy Menu");
		}

		ImGui::PopStyleVar(1);

		if (ImGui::BeginPopup("Editor Hierarchy Menu"))
		{
			if (ImGui::BeginMenu("Add object"))
			{
				if (ImGui::MenuItem("Light"))
				{
					auto newObject = std::static_pointer_cast<PGameObject>(std::make_shared<PLight>());
					auto scene = std::static_pointer_cast<PEditorScene>(m_layoutContext->GetContext());

					newObject->SetName("Game Light");
					m_layoutContext->GetContext()->AddGameObject(newObject);

					scene->m_selectionContext = newObject->ID();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
	else
		ImGui::PopStyleVar(1);
}

void PEditorHierarchyWindow::DrawNode(const std::pair<std::string, std::shared_ptr<PGameObject>> node)
{
	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto scene = std::static_pointer_cast<PEditorScene>(m_layoutContext->GetContext());
	auto flags = (scene->m_selectionContext==node.first)?ImGuiTreeNodeFlags_Selected: 0 | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)node.second.get(), flags, node.second->Name().c_str());
	
	if (ImGui::IsItemClicked())
	{
		scene->m_selectionContext = node.first;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		ImGui::Text((std::string("ID:") + node.first).c_str());
		ImGui::Text((std::string("Type:") + std::string(typeid(*node.second.get()).name())).c_str());

		ImGui::EndTooltip();
	}
	if (opened)
	{
		ImGui::TreePop();
	}

	ImGui::TableNextColumn();
	ImGui::TextDisabled(typeid(*node.second.get()).name());
}
