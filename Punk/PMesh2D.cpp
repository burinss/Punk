#include "PMesh2D.hpp"
#include "PunkDefines.hpp"
#include "PMesh.hpp"
#include "PTexture.h"
#include "PVertexArrayBuffer.hpp"
#include "PGlobals.hpp"

PMesh2D::PMesh2D(PVertex2DDataStruct vertexData)
	:m_vertexData(vertexData)//, _textures(textures)
{
	//Initialize();
}

void PMesh2D::Initialize()
{
	m_vertexBuffer = PVertexArrayBuffer::Create(m_vertexData);
}

void PMesh2D::Render(PShader* shader)
{
	PSystems::GetGraphics()->Draw(m_vertexBuffer, m_vertexData, PMeshDrawStyle::Triangle);
	//glActiveTexture(GL_TEXTURE0);
}

std::shared_ptr<PMesh2D> PMesh2D::NewQuad()
{
	PVertex2DDataStruct options;
	options.vertices = std::vector<PVertex2D>{
		PVertex2D(glm::vec2(-1.f, 1.f), glm::vec2(0.f)),
		PVertex2D(glm::vec2(-1.f, -1.f), glm::vec2(0.f, 1.f)),
		PVertex2D(glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f)),
		PVertex2D(glm::vec2(1.f, -1.f), glm::vec2(1.f)),
		PVertex2D(glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f)),
		PVertex2D(glm::vec2(-1.f, -1.f), glm::vec2(0.f, 1.f)),
	};
	std::shared_ptr<PMesh2D> mesh = std::make_shared<PMesh2D>(options);
	mesh->Initialize();
	return mesh;
}
