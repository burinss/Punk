#include "PRenderer3D.hpp"
#include "PRenderPass.hpp"

void PRenderer3D::Render(double deltaTime)
{
	for (auto pass : passes_) {
		pass->Perform(deltaTime);
	}
}

void PRenderer3D::AddPass(const std::shared_ptr<PRenderPass>& pass)
{
	pass->Initialize();
	passes_.emplace_back(pass);
}
