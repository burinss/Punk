#include "PunkDefines.hpp"
#include "PRenderPass.hpp"
#include "PFramebuffer.hpp"
#include "PMesh2D.hpp"
#include "PRenderable.hpp"
#include "PGlobals.hpp"
#include "PShader.hpp"
#include "PTexture.h"

PRenderPass::PRenderPass(const std::shared_ptr<PRenderable>& root, const std::shared_ptr<PShader>& shader, PRenderOp renderOp, const glm::vec2& size, bool multisample, std::initializer_list<PRenderPass::ptr> dependencies)
	: root_(root), shader_(shader), renderOp_(renderOp), size_(size), multisample_(multisample), dependencies_(dependencies)
{
}

void PRenderPass::Initialize()
{
	switch (renderOp_) {
	case PRenderOp::Post:
		screenQuad_ = PMesh2D::NewQuad();
	case PRenderOp::UI:
	case PRenderOp::Color:
		if (multisample_)
			multisampledFramebuffer_ = PFramebuffer::CreateColor(size_, true);
		framebuffer_ = PFramebuffer::CreateColor(size_);
		break;
	case PRenderOp::Depth:
	case PRenderOp::Shadow:
		framebuffer_ = PFramebuffer::CreateDepth(size_);
		break;
	default:
		framebuffer_ = PFramebuffer::CreateColor(size_);
		break;
	}
}

void PRenderPass::Perform(double deltaTime)
{
	if (!root_->Renderable()) return;

	framebuffer_->Resize(size_.x, size_.y);

	if (multisample_) {
		multisampledFramebuffer_->Resize(size_.x, size_.y, true);
		multisampledFramebuffer_->Bind();
	}
	else {
		framebuffer_->Bind();
	}
	PSystems::GetGraphics()->ClearViewport();
	PSystems::GetGraphics()->UpdateViewport(size_);
	if (renderOp_ == PRenderOp::Depth || renderOp_ == PRenderOp::Shadow) {
		PSystems::GetGraphics()->ClearDepth();
	}
	else if (renderOp_ == PRenderOp::Post) {
		PSystems::GetGraphics()->DisableDepthTesting();
	}

	shader_->Use();
	BindDependencies();

	if (renderOp_ == PRenderOp::Post) {
		screenQuad_->Render(shader_.get());
	}
	else {
		root_->Render(deltaTime, shader_/*, renderOp_*/);
	}

	if (renderOp_ == PRenderOp::Post) {
		PSystems::GetGraphics()->EnableDepthTesting();
	}

	if (multisample_) {
		multisampledFramebuffer_->Blit(framebuffer_);
	}

	// TODO: Use a better method to determine when to blit the current renderpass
	// framebuffer onto the default framebuffer
	if (/*renderOp_ == PRenderOp::Color||*/ renderOp_== PRenderOp::Post || renderOp_ == PRenderOp::UI) {
		framebuffer_->Blit();
	}
}

void PRenderPass::BindDependencies()
{
	unsigned int depthAttachments = 0;
	unsigned int colorAttachments = 0;
	unsigned int shadowAttachments = 0;
	unsigned int index = 0;
	for (auto dep : dependencies_) {
		dep->framebuffer_->Attachment()->Bind(index);
		switch (dep->renderOp_) {
		case PRenderOp::Color:
			shader_->SetInt(std::string("colorSampler") + std::to_string(colorAttachments++), index);
			break;
		case PRenderOp::Shadow:
			shader_->SetInt(std::string("shadowSampler") + std::to_string(shadowAttachments++), index);
			break;
		case PRenderOp::Depth:
			shader_->SetInt(std::string("depthSampler") + std::to_string(depthAttachments++), index);
			break;
		}
		++index;
	}
}

