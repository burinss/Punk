#pragma once

#include "PGraphics.hpp"
#include <glm/glm.hpp>

class PVertexArrayBufferGL;
class PGraphicsGL : public PGraphics {
private:
    std::map<PMeshDrawStyle, unsigned int> _drawStyles;
    static void GLFWErrorCallback(int id, const char* description);
public:
	PGraphicsGL();
	~PGraphicsGL();


    void Initialize() override;
    void ClearViewport() override;
    void SwapBuffers() override;
    void EnableStencilTesting() override;
    void EnableDepthTesting() override;
    void DisableDepthTesting() override;
    void EnableStencilBuffer() override;
    void DisableStencilBuffer() override;
    void EnableAlphaBlending() override;
    void DisableAlphaBlending() override;
    void EnableFaceCulling() override;
    void DisableFaceCulling() override;
    void EnableMSAA() override;
    void DisableMSAA() override;
    void CullFrontFaces() override;
    void CullBackFaces() override;
    void ClearDepth() override;

    void UpdateViewport(const glm::vec2& size, const glm::vec2& position = glm::vec2(0.f)) override;

    //void Draw(const unsigned int &vao, const std::vector<unsigned int>& indices,PMeshDrawStyle drawStyle)override;
    void Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer, const PVertex3DDataStruct& vertexData, PMeshDrawStyle drawStyle)override;
    void Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer, const PVertex2DDataStruct& vertexData, PMeshDrawStyle drawStyle)override;
};