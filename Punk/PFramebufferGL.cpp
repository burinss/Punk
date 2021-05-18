#include "PFramebufferGL.hpp"
#include <glad/glad.h>
#include "PTexture.h"


void PFramebufferGL::LoadColor(const glm::vec2& size, const std::shared_ptr<PTexture>& colorTexture, bool multisample)
{
    attachment_ = colorTexture;
    size_ = size;
    glGenFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, colorTexture->id, 0);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    glGenRenderbuffers(1, &rboId_);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId_);

    if (multisample)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, size.x, size.y);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    {
        std::cout << "Framebuffer operation incomplete dimensions" << std::endl;
        //LOG("Framebuffer operation incomplete dimensions", ZSeverity::Error);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PFramebufferGL::LoadDepth(const glm::vec2& size, const std::shared_ptr<PTexture>& depthTexture)
{
    attachment_ = depthTexture;
    size_ = size;
    glGenFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    {
        std::cout << "Framebuffer operation incomplete dimensions" << std::endl;
        //LOG("Framebuffer operation incomplete dimensions", ZSeverity::Error);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PFramebufferGL::LoadCubeMap()
{
    size_ = glm::vec2(1024, 1024);
    glGenFramebuffers(1, &id_);
    glGenRenderbuffers(1, &rboId_);

    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId_);
}

void PFramebufferGL::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glViewport(0, 0, size_.x, size_.y);
}

void PFramebufferGL::BindRenderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, rboId_);    
}

void PFramebufferGL::Resize(unsigned int width, unsigned int height, bool multisample)
{
    if ((size_.x == width && size_.y == height)) return;

    if (attachment_) {
        size_ = glm::vec2(width, height);
        attachment_->Resize(width, height, multisample);
    }

    BindRenderbuffer();

    if (multisample)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    }

    UnbindRenderbuffer();
}

void PFramebufferGL::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, size_.x, size_.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PFramebufferGL::UnbindRenderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void PFramebufferGL::Blit(const std::shared_ptr<PFramebuffer>& destination)
{
    glm::vec2 destinationSize = destination ? destination->Size() : size_;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination ? destination->ID() : 0);
    glBlitFramebuffer(0, 0, size_.x, size_.y, 0, 0, destinationSize.x, destinationSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
