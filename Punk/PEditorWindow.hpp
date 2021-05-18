#pragma once
#include "PunkDefines.hpp"
//#include "PEditorLayout.hpp"
class PEditorLayout;

class PEditorWindow
{
public:
	PEditorWindow(const std::shared_ptr<PEditorLayout>& context) :m_layoutContext(context) {};
	virtual ~PEditorWindow() {}

	virtual void Render() = 0;
protected:
	std::string m_title;
	std::shared_ptr<PEditorLayout> m_layoutContext;
private:
};