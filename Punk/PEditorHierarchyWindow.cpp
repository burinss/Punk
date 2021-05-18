#include "PEditorHierarchyWindow.hpp"
#include "PunkDefines.hpp"
#include "imgui.h"
#include "PGameObject.hpp"
#include "PEditorLayout.hpp"
#include "PEditorScene.hpp"
#include "PLight.hpp"

void PEditorHierarchyWindow::Render()
{
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
		if (ImGui::BeginPopup("Editor Hierarchy Menu"))
		{
			if (ImGui::BeginMenu("Add object"))
			{
				if (ImGui::MenuItem("Light"))
				{
					std::shared_ptr<PGameObject> newObject = std::static_pointer_cast<PGameObject>(std::make_shared<PLight>());
					m_layoutContext->GetContext()->AddGameObject(newObject);
					auto scene = std::static_pointer_cast<PEditorScene>(m_layoutContext->GetContext());
					scene->m_selectionContext = newObject->ID();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}

void PEditorHierarchyWindow::DrawNode(const std::pair<std::string, std::shared_ptr<PGameObject>> node)
{

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto scene = std::static_pointer_cast<PEditorScene>(m_layoutContext->GetContext());
	ImGuiTreeNodeFlags flags = (scene->m_selectionContext==node.first)?ImGuiTreeNodeFlags_Selected: 0 | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)node.second.get(), flags, node.second->Name().c_str());
	
	if (ImGui::IsItemClicked())
	{
		scene->m_selectionContext = node.first;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.f, 1.f));
		ImGui::BeginTooltip();
		//ImGui::LabelText(std::string("ID:").c_str(), node.first.c_str());
		ImGui::Text((std::string("ID:") + node.first).c_str());
		ImGui::Text((std::string("Type:") + std::string(typeid(*node.second.get()).name())).c_str());
		ImGui::EndTooltip();
		ImGui::PopStyleVar(1);
	}
	if (opened)
	{
		ImGui::TreePop();
	}

	ImGui::TableNextColumn();
	ImGui::Text(typeid(*node.second.get()).name());
}