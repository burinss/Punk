// Class and Data Structure Definitions

#include "PunkDefines.hpp"

class PTexture;

class PFramebuffer
{

protected:

    glm::vec2 size_;
    unsigned int id_;
    unsigned int rboId_;
    std::shared_ptr<PTexture> attachment_;

public:

    //using ptr = std::shared_ptr<PFramebuffer>;

    PFramebuffer() = default;
    virtual ~PFramebuffer() {}

    unsigned int ID() const { return id_; }
    unsigned int RBID() const { return rboId_; }
    glm::vec2 Size() const { return size_; }
    std::shared_ptr<PTexture> Attachment() { return attachment_; }

    virtual void LoadColor(const glm::vec2& size, const std::shared_ptr<PTexture>& colorTexture, bool multisample = false) = 0;
    virtual void LoadDepth(const glm::vec2& size, const std::shared_ptr<PTexture>& depthTexture) = 0;
    virtual void LoadCubeMap() = 0;

    virtual void Bind() = 0;
    virtual void BindRenderbuffer() = 0;
    virtual void Resize(unsigned int width, unsigned int height, bool multisample = false) = 0;
    virtual void Unbind() = 0;
    virtual void UnbindRenderbuffer() = 0;
    virtual void Blit(const std::shared_ptr<PFramebuffer>& destination = nullptr) = 0;

    static std::shared_ptr<PFramebuffer> CreateColor(const glm::vec2& size, bool multisample = false);
    static std::shared_ptr<PFramebuffer> CreateDepth(const glm::vec2& size);
    static std::shared_ptr<PFramebuffer> CreateCubeMap();

};
