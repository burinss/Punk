#pragma once

#include "PEditorWindow.hpp"
#include <filesystem>

class PEditorContentBrowserWindow : public PEditorWindow
{
	
public:
	PEditorContentBrowserWindow(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context)
	{}
	void Render() override;
protected:
	void ShowDirectories(std::filesystem::path);
private:
};