#pragma once

#include "PEditorWindow.hpp"

class PEditorLayout;

class PEditorMenuBar : public PEditorWindow
{
public:
	PEditorMenuBar(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context) {}

	void Render() override;
protected:
private:
};