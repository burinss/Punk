#pragma once

#include "PEditorWindow.hpp"
#include <filesystem>

class PTexture;

class PEditorContentBrowserWindow : public PEditorWindow
{
	
public:
	PEditorContentBrowserWindow(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context)
	{
		m_selectedPath = std::filesystem::current_path();
		LoadIconImages();
	}
	void Render() override;
protected:
	void ShowDirectories(std::filesystem::path);
	void ShowFiles();
	void LoadIconImages();
private:
	std::map < std::string, std::shared_ptr<PTexture>> m_iconMap;
	std::filesystem::path m_selectedPath;
};