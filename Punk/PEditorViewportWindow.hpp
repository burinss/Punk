#pragma once

#include "PunkDefines.hpp"
#include "PEditorWindow.hpp"
#include "PEditorLayout.hpp"


class PEditorViewportWindow : public PEditorWindow
{
public:
	PEditorViewportWindow(const std::shared_ptr<PEditorLayout> context) :PEditorWindow(context) {}
	~PEditorViewportWindow() {}
	void Render() override;

protected:
private:
};