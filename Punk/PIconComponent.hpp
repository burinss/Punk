#pragma once
#include "PComponent.hpp"
#include "PRenderable.hpp"

class PMesh;
class PTexture;

class PIconComponent: public PComponent, public PRenderable
{
public:
	PIconComponent(){}
	~PIconComponent() {}

	void Initialize() override;

	void Render(double deltaTime, const std::shared_ptr<PShader>& shader) override;
	bool Renderable() override { return true; }

protected:
	std::shared_ptr<PMesh> m_mesh;
private:
	std::shared_ptr<PShader>  m_iconShader;
	std::shared_ptr<PTexture> m_icon;
};