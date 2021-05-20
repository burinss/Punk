#pragma once
#include "PEditorWindow.hpp"

class PEditorToolBar: public PEditorWindow
{
public:
	PEditorToolBar(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context) {}

	void Render() override;
protected:
private:
};