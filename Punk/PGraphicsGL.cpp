#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PunkDefines.hpp"
#include "PGraphicsGL.hpp"
#include "PVertexArrayBufferGL.hpp"

void PGraphicsGL::GLFWErrorCallback(int id, const char* description)
{
    std::cout << description << std::endl;
}

PGraphicsGL::PGraphicsGL()
{
}

PGraphicsGL::~PGraphicsGL()
{
}

void PGraphicsGL::Initialize()
{
    glfwSetErrorCallback(GLFWErrorCallback);
    
    _drawStyles[PMeshDrawStyle::Point] = GL_POINT;
    _drawStyles[PMeshDrawStyle::Line] = GL_LINES;
    _drawStyles[PMeshDrawStyle::LineStrip] = GL_LINE_STRIP;
    _drawStyles[PMeshDrawStyle::Triangle] = GL_TRIANGLES;
    _drawStyles[PMeshDrawStyle::TriangleStrip] = GL_TRIANGLE_STRIP;
    _drawStyles[PMeshDrawStyle::TriangleFan] = GL_TRIANGLE_FAN;
    _drawStyles[PMeshDrawStyle::Quads] = GL_QUADS;
}

void PGraphicsGL::ClearViewport()
{
    //glViewport(0, 0, 800, 600);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00);
    
}

void PGraphicsGL::SwapBuffers()
{
    GLFWwindow* window = glfwGetCurrentContext();
    glfwSwapBuffers(window);
}

void PGraphicsGL::EnableStencilTesting()
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void PGraphicsGL::EnableDepthTesting()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void PGraphicsGL::DisableDepthTesting()
{
    glDisable(GL_DEPTH_TEST);
}

void PGraphicsGL::EnableStencilBuffer()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void PGraphicsGL::DisableStencilBuffer()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
}

void PGraphicsGL::EnableAlphaBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PGraphicsGL::DisableAlphaBlending()
{
    glDisable(GL_BLEND);
}

void PGraphicsGL::EnableFaceCulling()
{
    glEnable(GL_CULL_FACE);
}

void PGraphicsGL::DisableFaceCulling()
{
    glDisable(GL_CULL_FACE);
}

void PGraphicsGL::EnableMSAA()
{
    glEnable(GL_MULTISAMPLE);
}

void PGraphicsGL::DisableMSAA()
{
    glDisable(GL_MULTISAMPLE);
}

void PGraphicsGL::CullFrontFaces()
{
    glCullFace(GL_FRONT);
}

void PGraphicsGL::CullBackFaces()
{
    glCullFace(GL_BACK);
}

void PGraphicsGL::ClearDepth()
{
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void PGraphicsGL::UpdateViewport(const glm::vec2& size, const glm::vec2& position)
{
    glViewport(position.x, position.y, size.x, size.y);
}

void PGraphicsGL::Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer,const PVertex3DDataStruct& vertexData, PMeshDrawStyle drawStyle)
{
    arrayBuffer->Bind();
    if (!vertexData.indices.empty())
	{
		glDrawElements(_drawStyles[drawStyle], vertexData.indices.size(), GL_UNSIGNED_INT, 0);
	}
    else
    {
        glDrawArrays(_drawStyles[drawStyle], 0, vertexData.vertices.size());
    }
    arrayBuffer->Unbind();
}

void PGraphicsGL::Draw(std::shared_ptr<PVertexArrayBuffer>& arrayBuffer, const PVertex2DDataStruct& vertexData, PMeshDrawStyle drawStyle)
{
    arrayBuffer->Bind();

    glDrawArrays(_drawStyles[drawStyle], 0, vertexData.vertices.size());

    arrayBuffer->Unbind();
	glActiveTexture(GL_TEXTURE0);
}

//void PGraphicsGL::Draw(const unsigned int& vao, const std::vector<unsigned int>& indices, PMeshDrawStyle drawStyle)
//{
//    // draw mesh
//    glBindVertexArray(vao);
//    glDrawElements(_drawStyles[drawStyle], indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}

