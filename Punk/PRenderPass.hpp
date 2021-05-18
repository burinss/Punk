#pragma once
#include "PunkDefines.hpp"
#include "PMesh2D.hpp"
class PShader;
class PRenderable;
class PFramebuffer;

class PRenderPass
{

public:

	using ptr = std::shared_ptr<PRenderPass>;

	template<class ...Args>
	PRenderPass(const std::shared_ptr<PRenderable>& root, const std::shared_ptr<PShader>& shader, PRenderOp renderOp,
		const glm::vec2& size, bool multisample = false, Args... args)
		: PRenderPass(root, shader, renderOp, size, multisample, { args... }) { }
	PRenderPass(const std::shared_ptr<PRenderable>& root, const std::shared_ptr<PShader>& shader, PRenderOp renderOp,
		const glm::vec2& size, bool multisample = false, std::initializer_list<PRenderPass::ptr> dependencies = {});
	virtual ~PRenderPass() { }

	void Initialize();
	void Perform(double deltaTime);

	void SetSize(const glm::vec2& size) { if (!fixedSize_) size_ = size; }
	void SetIsSizeFixed(bool fixed) { fixedSize_ = fixed; }

	std::shared_ptr<PFramebuffer> Framebuffer() { return multisample_? framebuffer_:multisampledFramebuffer_; }
protected:

	std::shared_ptr<PRenderable> root_;
	std::shared_ptr<PShader> shader_;
	glm::vec2 size_;
	std::shared_ptr<PFramebuffer> framebuffer_, multisampledFramebuffer_;
	std::shared_ptr<PMesh2D> screenQuad_;
	std::vector<std::shared_ptr<PRenderPass>> dependencies_;
	PRenderOp renderOp_;
	bool multisample_ = false;
	bool fixedSize_ = false;

	void BindDependencies();

};