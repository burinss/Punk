#pragma once

#include "PFramebuffer.hpp"

class PFramebufferGL : public PFramebuffer
{

public:
	virtual void LoadColor(const glm::vec2& size, const std::shared_ptr<PTexture>& colorTexture, bool multisample = false) override;
	virtual void LoadDepth(const glm::vec2& size, const std::shared_ptr<PTexture>& depthTexture) override;
	virtual void LoadCubeMap() override;

    virtual void Bind() override;
    virtual void BindRenderbuffer() override;
    virtual void Resize(unsigned int width, unsigned int height, bool multisample = false) override;
    virtual void Unbind() override;
    virtual void UnbindRenderbuffer() override;
    virtual void Blit(const std::shared_ptr<PFramebuffer>& destination = nullptr) override;

};