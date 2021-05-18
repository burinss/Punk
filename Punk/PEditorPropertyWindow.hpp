#pragma once
#include "PunkDefines.hpp"
#include "PEditorWindow.hpp"
#include "PEditorLayout.hpp"
class PGameObject;

enum class PEditorObjectType
{
	CAMERA, LIGHT, OBJECT, UNDEFINED, EDITOR
};

class PEditorPropertyWindow : public PEditorWindow
{
public:
	PEditorPropertyWindow(const std::shared_ptr<PEditorLayout>& context) :PEditorWindow(context) {}

	void Render() override;
protected:
	void ShowObjectProperties(const std::shared_ptr<PGameObject>& gameObject);
	PEditorObjectType DetectObjectType(const std::shared_ptr<PGameObject>& gameObject);
private:
};