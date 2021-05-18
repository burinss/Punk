#pragma once

#include "PunkDefines.hpp"
#include "PTask.hpp"
#include "PScene.hpp"
#include "PEditorWindow.hpp"
class PGameObject;
class PEditorLayout;
class PEditorHierarchyWindow: public PEditorWindow
{
public:
	PEditorHierarchyWindow(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context)
	{}
	void Render()override;
protected:
	void DrawNode(const std::pair<std::string,std::shared_ptr<PGameObject>> node);
private:
};