
#include "PEditorContentBrowserWindow.hpp"
#include "imgui.h"
#include <filesystem>
#include "PTexture.h"
#include "PEditorLayout.hpp"

void PEditorContentBrowserWindow::Render()
{
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.2f, 0.2f));

	namespace fs = std::filesystem;
	if (ImGui::Begin("Content Browser",(bool*)0,ImGuiWindowFlags_NoScrollbar))
	{
		auto tableFlags = ImGuiTableFlags_BordersInnerV |ImGuiTableFlags_Resizable;
		if (ImGui::BeginTable("split",2,tableFlags))
		{
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoClip);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoClip);

			ImGui::TableNextRow();
			if (ImGui::TableNextColumn())
			{
				if (ImGui::BeginChild("Content", ImVec2(ImGui::GetColumnWidth(), 0.0f)))
				{
					ShowDirectories(fs::current_path().parent_path().parent_path() / "assets");

					ImGui::EndChild();
				}
			}
			if (ImGui::TableNextColumn())
			{
				if (ImGui::BeginChild("Files", ImVec2(ImGui::GetColumnWidth(), 0.f)))
				{
					ShowFiles();

					ImGui::EndChild();
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}
	ImGui::PopStyleVar(1);
}

void PEditorContentBrowserWindow::ShowDirectories(std::filesystem::path dir)
{
	
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			//ImGui::Text(entry.path().u8string().c_str());
			auto directoryPath = entry.path();
			auto directoryPathString = directoryPath.u8string();
			auto dirName = directoryPathString.substr(directoryPathString.find_last_of("\\")+1);
			auto childFolders = fs::directory_iterator(entry);

			bool hasChild = false;
			for (auto folder : childFolders)
			{
				if (folder.exists() && folder.is_directory())
				{
					hasChild = true;
				}
			}

			auto treeNodeFlags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_SpanAvailWidth;

			bool opened = ImGui::TreeNodeEx(dirName.c_str(), hasChild ? treeNodeFlags : treeNodeFlags | ImGuiTreeNodeFlags_Bullet);
			if (opened)
			{
				if (ImGui::IsItemClicked()&&ImGui::IsItemHovered())
					m_selectedPath = directoryPath;

				if (hasChild)
					ShowDirectories(entry);

				ImGui::TreePop();
			}
			
		}
	}
}

void PEditorContentBrowserWindow::ShowFiles()
{
	int columns=0;

	auto tableFlags =  ImGuiTableFlags_NoBordersInBody;
	auto childWidth = ImGui::GetColumnWidth();
	auto buttonSize = 86.f;
	float cellPadding = 6.f;

	columns = childWidth / (buttonSize + cellPadding);

	if (columns==0||columns>63)
	{
		columns = 1;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(cellPadding, cellPadding));

	if (ImGui::BeginTable("FilesTable", columns, tableFlags))
	{
		int currentCol = 0;
		namespace fs = std::filesystem;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.0, 0.0, 0.0));
		for (const auto& entry : fs::directory_iterator(m_selectedPath))
		{
			if (entry.is_regular_file()|| entry.is_directory())
			{
				if (currentCol == columns)
				{
					currentCol = 0;
					ImGui::TableNextRow();
				}
				ImGui::TableNextColumn();
				ImGui::BeginGroup();
				{
					auto directoryPathString = entry.path().u8string();
					auto fileName = directoryPathString.substr(directoryPathString.find_last_of("\\") + 1);
					


					if (entry.is_regular_file()) 
					{
						auto format = fileName.substr(fileName.find_last_of(".") + 1);
						auto fileIcon = m_layoutContext->GetIcon("File");

						format == "fbx" ? fileIcon = m_layoutContext->GetIcon(format) :
							format == "png" ? fileIcon = m_layoutContext->GetIcon(format) :
							format == "obj" ? fileIcon = m_layoutContext->GetIcon(format) :
							format == "jpg" ? fileIcon = m_layoutContext->GetIcon(format) : fileIcon = m_layoutContext->GetIcon("File");
						ImGui::ImageButton((void*)fileIcon->id, ImVec2(buttonSize - cellPadding, buttonSize - cellPadding));
					}
					else if (entry.is_directory())
					{
						if (ImGui::ImageButton((void*)m_layoutContext->GetIcon("File")->id, ImVec2(buttonSize - cellPadding, buttonSize - cellPadding)))
						{
							m_selectedPath = entry;
						}
					}
					ImGui::Text(fileName.c_str());
					ImGui::EndGroup();
				}
				currentCol++;
			}
		}
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
		ImGui::EndTable();
	}
	ImGui::PopStyleVar(1);
}

//TODO: make global path consts
