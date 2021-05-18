#pragma once
#include "PunkDefines.hpp"
class PVertexArrayBuffer;
class PGraphics {
public:
    PGraphics() {};
    virtual ~PGraphics() {}

    virtual void Initialize() = 0;
    virtual void ClearViewport() = 0;
    virtual void SwapBuffers() = 0;
    virtual void EnableStencilTesting() = 0;
    virtual void EnableDepthTesting() = 0;
    virtual void DisableDepthTesting() = 0;
    virtual void EnableStencilBuffer() = 0;
    virtual void DisableStencilBuffer() = 0;
    virtual void EnableAlphaBlending() = 0;
    virtual void DisableAlphaBlending() = 0;
    virtual void EnableFaceCulling() = 0;
    virtual void DisableFaceCulling() = 0;
    virtual void EnableMSAA() = 0;
    virtual void DisableMSAA() = 0;
    virtual void CullFrontFaces() = 0;
    virtual void CullBackFaces() = 0;
    virtual void ClearDepth() = 0;

    virtual void UpdateViewport(const glm::vec2& size, const glm::vec2& position = glm::vec2(0.f)) = 0;

    virtual void Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer, const PVertex3DDataStruct& vertexData, PMeshDrawStyle drawStyle) = 0;
    virtual void Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer, const PVertex2DDataStruct& vertexData, PMeshDrawStyle drawStyle) = 0;
};