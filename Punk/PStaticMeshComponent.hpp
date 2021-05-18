#pragma once

#include "PComponent.hpp"
#include "PRenderable.hpp"
#include "PMesh.hpp"

class PLight;
class PScene;
class PEditorScene;

class	PStaticMeshComponent : public PComponent, public PRenderable
{
public:
	PStaticMeshComponent() {}
	virtual ~PStaticMeshComponent() {}
	void Initialize() override { PComponent::Initialize(); }
	void Initialize(std::shared_ptr<PMesh> mesh) { m_mesh = mesh; }

	void Render(double deltaTime, const std::shared_ptr<PShader>& shader);
	bool Renderable() override { return true; }

	std::shared_ptr<PMesh> Mesh() { return m_mesh; }
	void SetMesh(std::shared_ptr<PMesh> mesh) { m_mesh = mesh; }
protected:
	void DrawOutlineIfEnabled(const std::shared_ptr<PEditorScene> context, const glm::mat4& model, const glm::mat4& viewProjection);
	std::shared_ptr<PMesh> m_mesh;

	std::map<std::string, std::shared_ptr<PLight>> m_gameLights;
};