#pragma once
#include "PunkDefines.hpp"
#include "PScene.hpp"

class PCamera;
class PShader;
class PStaticMeshComponent;

class PEditorScene : public PScene
{
	friend class PStaticMeshComponent;
public:
	std::string m_selectionContext;

public:
	PEditorScene():PScene(){}

	void Initialize() override;
	void Update(double deltaTime) override;
protected:
	void SetupRenderPasses() override;
	std::shared_ptr<PCamera> m_EditorCamera;
	std::shared_ptr<PShader> m_highlightShader;
};