#include "PEditorContentBrowserWindow.hpp"
#include "imgui.h"
#include <filesystem>

void PEditorContentBrowserWindow::Render()
{
	namespace fs = std::filesystem;

	if (ImGui::Begin("Content Browser"))
	{
		if (ImGui::BeginTable("split",2))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ShowDirectories(fs::current_path());
			ImGui::TableNextColumn();
			ImGui::Text("Hello World");
			ImGui::EndTable();
		}
		ImGui::End();
	}
}

void PEditorContentBrowserWindow::ShowDirectories(std::filesystem::path dir)
{
	namespace fs = std::filesystem;
	for (const auto& entry : fs::recursive_directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			//ImGui::Text(entry.path().u8string().c_str());
			if (ImGui::TreeNode(entry.path().u8string().c_str()))
			{
				ImGui::Text("Hello world");
				ImGui::TreePop();
			}
		}
	}
}
