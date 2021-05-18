#include "PFramebufferGL.hpp"
#include "PTexture.h"

std::shared_ptr<PFramebuffer> PFramebuffer::CreateColor(const glm::vec2& size, bool multisample)
{
    auto framebuffer = std::make_shared<PFramebufferGL>();
    auto attachment = PTexture::CreateColor(size, multisample);
    framebuffer->LoadColor(size, attachment, multisample);
    return framebuffer;
}

std::shared_ptr<PFramebuffer> PFramebuffer::CreateDepth(const glm::vec2& size)
{
    auto framebuffer = std::make_shared<PFramebufferGL>();
    auto attachment = PTexture::CreateDepth(size);
    framebuffer->LoadDepth(size, attachment);
    return framebuffer;
}

std::shared_ptr<PFramebuffer> PFramebuffer::CreateCubeMap()
{
    auto framebuffer = std::make_shared<PFramebufferGL>();
    framebuffer->LoadCubeMap();
    return framebuffer;
}
