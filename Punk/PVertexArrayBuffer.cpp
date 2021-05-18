#include "PVertexArrayBuffer.hpp"
#include "PVertexArrayBufferGL.hpp"

std::shared_ptr<PVertexArrayBuffer> PVertexArrayBuffer::Create(const PVertex3DDataStruct& vertexData)
{
	auto buffer = std::make_shared<PVertexArrayBufferGL>();
	buffer->Load(vertexData);
	return buffer;
}

std::shared_ptr<PVertexArrayBuffer> PVertexArrayBuffer::Create(const PVertex2DDataStruct& vertexData)
{
	auto buffer = std::make_shared<PVertexArrayBufferGL>();
	buffer->Load(vertexData);
	return buffer;
}
