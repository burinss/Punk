#pragma once
#include "PunkDefines.hpp"
#include "PTask.hpp"
class PVertexArrayBuffer;
class PShader;
class PMesh : public Task {
public:
	virtual ~PMesh() {};

	virtual void Render(PShader* shader) = 0;
	virtual void Initialize() = 0;
protected:
	std::shared_ptr<PVertexArrayBuffer> m_vertexBuffer;
};