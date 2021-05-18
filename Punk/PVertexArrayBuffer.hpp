#pragma once
#include "PunkDefines.hpp"

class PVertexArrayBuffer
{
public:
	PVertexArrayBuffer() {}
	virtual ~PVertexArrayBuffer() {}

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void Load(const PVertex3DDataStruct& vertexData) = 0;
	virtual void Load(const PVertex2DDataStruct& vertexData) = 0;
	virtual void Delete() = 0;

	static std::shared_ptr<PVertexArrayBuffer> Create(const PVertex3DDataStruct& vertexData);
	static std::shared_ptr<PVertexArrayBuffer> Create(const PVertex2DDataStruct& vertexData);
	//static ptr Create(const ZVertex2DDataOptions& options);
protected:

	unsigned int m_vertexBufferObject = 0;
	//unsigned int ivbo_ = 0;
	unsigned int m_elementsBufferObject = 0;
	unsigned int m_vertexArrayObject = 0;
};