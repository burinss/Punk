#pragma once
#include "PGameObject.hpp"

class PSceneRoot : public PGameObject
{
private:
	//std::vector<std::shared_ptr<PGameObject>> 
public:
	PSceneRoot(const std::string& name) :PGameObject(name) {};
	virtual ~PSceneRoot() {};
	void Render(double deltaTime, const std::shared_ptr<PShader>& shader/*, ZRenderOp renderOp = ZRenderOp::Color*/) override;
	
	bool Renderable() override { return !m_childrens.empty(); }
	bool IsVisible() override { return true; }
	//PSceneRoot();
	//virtual ~PSceneRoot();
};